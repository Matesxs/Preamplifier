#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <debug.h>

#include "settings.h"
#include "pins.h"

namespace TemperatureReader
{
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature temp_sensors(&oneWire);
  float* temperatures = nullptr;
  uint8_t sensor_count = 0;

  [[noreturn]] void temperature_task(void*)
  {
    temp_sensors.begin();
    sensor_count = temp_sensors.getDeviceCount();
    if (sensor_count == 0)
    {
      DEBUG("No temperature sensors found\n");
      vTaskDelete(nullptr);
    }
    DEBUG("Found %d temperature sensors\r\n", sensor_count);

    temperatures = new float[sensor_count]();

    while(true)
    {
      temp_sensors.requestTemperatures();

      for (uint8_t i = 0; i < sensor_count; i++)
      {
        temperatures[i] = temp_sensors.getTempCByIndex(i);
        vTaskDelay(pdMS_TO_TICKS(2));
      }

      vTaskDelay(pdMS_TO_TICKS(UPDATE_TEMP_INTERVAL_MS));
    }
  }

  uint8_t getSensorCount()
  {
    return sensor_count;
  }

  float maxTemp()
  {
    float maxT = -999;
    for (uint8_t i = 0; i < sensor_count; i++)
    {
      maxT = max(maxT, temperatures[i]);
    }
    return maxT;
  }
};

#include "potentiometer_handling.h"

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "settings.h"
#include "pins.h"
#include "debug.h"

namespace TemperatureReader
{
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature temp_sensors(&oneWire);
  float* temperatures = NULL;
  uint8_t sensor_count = 0;

  void temperature_task(void*)
  {
    temp_sensors.begin();
    sensor_count = temp_sensors.getDeviceCount();
    temperatures = new float[sensor_count]();

    if (temperatures == NULL)
    {
      DEBUG("Failed to allocate temperature array of size %d\n", sensor_count);
      vTaskDelete(NULL);
    }
    else if (sensor_count == 0)
    {
      DEBUG("No temperature sensors found\n");
      vTaskDelete(NULL);
    }

    DEBUG("Found %d temperature sensors\r\n", sensor_count);

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

    vTaskDelete(NULL);
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

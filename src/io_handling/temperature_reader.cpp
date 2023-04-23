#include "potentiometer_handling.h"

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "settings.h"
#include "pins.h"

namespace TemperatureReader
{
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature temp_sensors(&oneWire);
  float temperatures[2];

  void temperature_task(void*)
  {
    while(true)
    {
      temp_sensors.requestTemperatures();

      vTaskDelay(pdMS_TO_TICKS(2));
      temperatures[0] = temp_sensors.getTempCByIndex(0);

      vTaskDelay(pdMS_TO_TICKS(2));
      temperatures[1] = temp_sensors.getTempCByIndex(1);

      // DEBUG("Temps: %.3lf %.3lf\n", temps_store.temp1, temps_store.temp2);

      vTaskDelay(pdMS_TO_TICKS(UPDATE_TEMP_INTERVAL_MS));
    }

    vTaskDelete(NULL);
  }
};

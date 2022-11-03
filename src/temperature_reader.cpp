#include "potentiometer_handling.h"

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "settings.h"
#include "pins.h"
#include "global_objects.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp_sensors(&oneWire);
volatile float temps_store[2];

void temperature_task(void*)
{
  while(true)
  {
    temp_sensors.requestTemperatures();

    vTaskDelay(pdMS_TO_TICKS(2));
    temps_store[0] = temp_sensors.getTempCByIndex(0);

    vTaskDelay(pdMS_TO_TICKS(2));
    temps_store[1] = temp_sensors.getTempCByIndex(1);

    // DEBUG("Temps: %.3lf %.3lf\n", temps_store.temp1, temps_store.temp2);

    vTaskDelay(pdMS_TO_TICKS(UPDATE_TEMP_INTERVAL_MS));
  }

  vTaskDelete(NULL);
}

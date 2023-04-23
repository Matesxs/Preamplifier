#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>

namespace OTA
{
  void ota_handle( void * parameter ) 
  {
    while (true) 
    {
      if (WiFi.isConnected())
        ArduinoOTA.handle();
      vTaskDelay(pdMS_TO_TICKS(3000));
    }
  }

  TaskHandle_t otaTaskHandle = NULL;

  void begin()
  {
    ArduinoOTA.begin();

    xTaskCreateUniversal(
      ota_handle,
      "ota",
      8192,
      NULL,
      1,
      &otaTaskHandle,
      1);
  }

  void end()
  {
    if (otaTaskHandle != NULL)
    {
      vTaskDelete(otaTaskHandle);
      otaTaskHandle = NULL;
    }
    ArduinoOTA.end();
  }
}

#include <Arduino.h>

#include "led_handler.h"
#include "settings.h"
#include "pins.h"

namespace LedStrip
{
  StripHandler handler(LED_SEGMENT_COUNT, LED_STRIP_PIN);

  void led_strips_task(void *)
  {
    while (true)
    {
      handler.render();
      vTaskDelay(10);
    }
  }

  void begin()
  {
    handler.begin();
    xTaskCreateUniversal(led_strips_task, "led_strips", 2048, NULL, 1, NULL, 1);
  }

  void applyEffect()
  {
    handler.applyEffect();
  }

  RgbColor getColor()
  {
    return handler.getColor();
  }

  void setColor(RgbColor color)
  {
    handler.setColor(color);
  }

  uint8_t getBrightness()
  {
    return handler.getBrightness();
  }

  void setBrightness(uint8_t brightness)
  {
    handler.setBrighness(brightness);
  }

  void incBrightness()
  {
    handler.incBrightness();
  }
  
  void decBrightness()
  {
    handler.decBrightness();
  }

  uint32_t getEffectIdx()
  {
    return handler.getEffectIdx();
  }

  void setEffectIdx(uint32_t effectIdx)
  {
    handler.setEffectIdx(effectIdx);
  }

  void factoryReset()
  {
    handler.factoryReset();
  }

  void setAlarm()
  {
    handler.setAlarm();
  }

  void disableAlarm()
  {
    handler.disableAlarm();
  }
}

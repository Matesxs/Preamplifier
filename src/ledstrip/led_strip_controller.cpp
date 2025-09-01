#include <Arduino.h>

#include "led_handler.h"
#include "settings.h"
#include "pins.h"

namespace LedStrip
{
  StripHandler handler(LED_SEGMENT_COUNT, LED_STRIP_PIN);

  [[noreturn]] void led_strips_task(void *)
  {
    while (true)
    {
      handler.render();
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }

  void begin()
  {
    handler.begin();
    xTaskCreateUniversal(led_strips_task, "led_strips", 2048, nullptr, 1, nullptr, 1);
  }

  void applyEffect()
  {
    handler.applyEffect();
  }

  RgbColor getColor()
  {
    return handler.getColor();
  }

  void setColor(const RgbColor color)
  {
    handler.setColor(color);
  }

  uint8_t getBrightness()
  {
    return handler.getBrightness();
  }

  void setBrightness(const uint8_t brightness)
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

  void setEffectIdx(const uint32_t effectIdx)
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

#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

namespace LedStrip
{
  void begin();

  void applyEffect();
  RgbColor getColor();
  void setColor(RgbColor color);
  uint8_t getBrightness();
  void setBrightness(uint8_t brightness);
  void incBrightness();
  void decBrightness();
  uint32_t getEffectIdx();
  void setEffectIdx(uint32_t effectIdx);
  void factoryReset();
}
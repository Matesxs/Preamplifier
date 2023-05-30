#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

class LedState;

class BaseEffect
{  
public:
  LedState *state;
  unsigned long start;
  RgbColor rgb = {0, 0, 0};

  BaseEffect()
  {
    start = millis();
  }
  
  BaseEffect(RgbColor color)
  {
    setColor(color);
    start = millis();
  }

  void setColor(RgbColor color)
  {
    for(int i = 0; i < 3; i++)
      rgb[i] = color[i];
  }
  
  virtual void render() = 0;
};

#pragma once

#include <NeoPixelBus.h>

class LedState;

class BaseEffect
{  
public:
  virtual ~BaseEffect() = default;

  LedState *state = nullptr;
  unsigned long start;
  RgbColor rgb = {0, 0, 0};

  BaseEffect()
  {
    start = millis();
  }
  
  explicit BaseEffect(const RgbColor color)
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

#pragma once

#include <NeoPixelBus.h>

#include "time_lerper.h"
#include "base_effect.h"
#include "ledstrip/led_state.h"

class WaveEffect: public BaseEffect
{
public:
  long sinTab[256];

  WaveEffect()
  {
    for(int i = 0; i < 256; i++)
      sinTab[i] = sin(3.1415 / 128 * i) * 0x7fff + 0x8000;
  }

  WaveEffect(RgbColor color) :
    BaseEffect(color)
  {
    for(int i = 0; i < 256; i++)
      sinTab[i] = sin(3.1415 / 128 * i) * 0x7fff + 0x8000;
  }
  
  virtual void render()
  {
    int j = ((millis() - start) / 63) & 255;
    int k = ((millis() - start) / 71) & 255;
    for(int i = 0; i < state->count; i++)
    {
      long s = (sinTab[(i*3 + j) & 255] >> 8) * (sinTab[-(i*4 + k) & 255] >> 8);
      state->setRgb(i, (rgb[0] * s) >> 16, (rgb[1] * s) >> 16, (rgb[2] * s) >> 16);
    }
  }
};

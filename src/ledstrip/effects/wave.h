#pragma once

#include <NeoPixelBus.h>

#include "time_lerper.h"
#include "base_effect.h"
#include "ledstrip/led_state.h"

class WaveEffect final : public BaseEffect
{
public:
  long sinTab[256] = {};

  WaveEffect()
  {
    for(int i = 0; i < 256; i++)
      sinTab[i] = static_cast<long>(sin(3.1415 / 128 * i) * 0x7fff + 0x8000);
  }

  explicit WaveEffect(const RgbColor color) :
    BaseEffect(color)
  {
    for(int i = 0; i < 256; i++)
      sinTab[i] = static_cast<long>(sin(3.1415 / 128 * i) * 0x7fff + 0x8000);
  }
  
  void render() override
  {
    const unsigned long j = ((millis() - start) / 63) & 255;
    const unsigned long k = ((millis() - start) / 71) & 255;
    for(int i = 0; i < state->count; i++)
    {
      const long s = (sinTab[(i*3 + j) & 255] >> 8) * (sinTab[-(i*4 + k) & 255] >> 8);
      state->setRgb(i, (rgb[0] * s) >> 16, (rgb[1] * s) >> 16, (rgb[2] * s) >> 16);
    }
  }
};

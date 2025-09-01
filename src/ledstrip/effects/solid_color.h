#pragma once

#include <NeoPixelBus.h>

#include "base_effect.h"
#include "ledstrip/led_state.h"

class SolidColor final : public BaseEffect
{
public:
  SolidColor() = default;

  explicit SolidColor(const RgbColor color) :
    BaseEffect(color)
  {
  }

  void render() override
  {
    for(int i = 0; i < state->count; i++)
      state->setRgb(i, rgb);
  }
};

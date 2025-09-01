#pragma once

#include <NeoPixelBus.h>

#include "base_effect.h"
#include "ledstrip/led_state.h"

class Kitt final : public BaseEffect
{
public:
  uint16_t _counter_mode_step = 0;
  const uint8_t delay_ms = 100;
  unsigned long last_update = 0;

  Kitt() = default;

  explicit Kitt(const RgbColor color) :
    BaseEffect(color)
  {
  }

  void render() override
  {
    if ((millis() - last_update) >= delay_ms)
    {
      for(int i=0; i < state->count; i++)
      {
        RgbColor px_rgb = state->getColorOfPixel(i);
        px_rgb.R = px_rgb.R >> 1;
        px_rgb.G = px_rgb.G >> 1;
        px_rgb.B = px_rgb.B >> 1;

        state->setRgb(i, px_rgb.R, px_rgb.G, px_rgb.B);
      }

      uint16_t pos = 0;

      if(_counter_mode_step < state->count) {
        pos = _counter_mode_step;
      } else {
        pos = (state->count * 2) - _counter_mode_step - 2;
      }

      state->setRgb(pos, rgb[0], rgb[1], rgb[2]);

      _counter_mode_step = (_counter_mode_step + 1) % ((state->count * 2) - 2);
      last_update = millis();
    }
  }
};
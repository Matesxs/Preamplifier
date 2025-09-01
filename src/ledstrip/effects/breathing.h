#pragma once

#include <NeoPixelBus.h>

#include "time_lerper.h"
#include "base_effect.h"
#include "ledstrip/led_state.h"

class Breath final : public BaseEffect
{
public:
  explicit Breath(const uint32_t duration=20000) :
    m_duration(duration),
    m_lerpProgress(0.0)
  {
    m_lerper.setReference(&m_lerpProgress);
    m_lerper.set(255, duration);
  }

  explicit Breath(const RgbColor color, const uint32_t duration=20000) :
    BaseEffect(color),
    m_duration(duration),
    m_lerpProgress(0.0)
  {
    m_lerper.setReference(&m_lerpProgress);
    m_lerper.set(255, duration);
  }
  
  void render() override
  {
    m_lerper.update();

    for(int i = 0; i < state->count; i++)
      state->setRgb(i, rgb.Dim(m_lerpProgress));

    if (m_lerper.finished())
    {
      if (m_lerper.getTarget() == 0)
        m_lerper.set(255, m_duration);
      else
        m_lerper.set(0, m_duration);
    }
  }

private:
  uint32_t m_duration;
  TimeLerper<uint8_t> m_lerper;
  uint8_t m_lerpProgress;
};
#pragma once

#include <NeoPixelBus.h>

#include "time_lerper.h"
#include "base_effect.h"
#include "ledstrip/led_state.h"
#include "debug.h"

class Breath: public BaseEffect
{
public:
  Breath(uint32_t duration=20000) :
    m_lerpProgress(0.0),
    m_duration(duration)
  {
    m_lerper.setReference(&m_lerpProgress);
    m_lerper.set(255, duration);
  }

  Breath(RgbColor color, uint32_t duration=20000) :
    BaseEffect(color),
    m_lerpProgress(0.0),
    m_duration(duration)
  {
    m_lerper.setReference(&m_lerpProgress);
    m_lerper.set(255, duration);
  }
  
  virtual void render()
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

    DEBUG("%d\n", m_lerpProgress);
  }

private:
  uint32_t m_duration;
  TimeLerper<uint8_t> m_lerper;
  uint8_t m_lerpProgress;
};
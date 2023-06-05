#pragma once

#include <NeoPixelBus.h>

#include "base_effect.h"
#include "io_handling/temperature_reader.h"
#include "ledstrip/led_state.h"
#include "time_lerper.h"
#include "settings.h"
#include "helpers.h"

class Temperature: public BaseEffect
{
public:
  Temperature(uint8_t brightness = 255) :
    m_lerpProgress(0.0),
    m_brightness(brightness)
  {
    float temperature = TemperatureReader::maxTemp();
    m_targetTemperature = temperature;
    rgb = RgbColor(HslColor(temperatureToHue(temperature), 1.0, 0.5));
    m_lerper.setReference(&m_lerpProgress);
  }

  virtual void render()
  {
    float temperature = TemperatureReader::maxTemp();
    if (m_targetTemperature != temperature)
    {
      m_targetColor = RgbColor(HslColor(temperatureToHue(temperature), 1.0, 0.5));
      m_lerper.set(1.0, 500, 0.0);
      m_targetTemperature = temperature;
    }

    if (!m_lerper.finished())
    {
      m_lerper.update();
      rgb = RgbColor::LinearBlend(rgb, m_targetColor, static_cast<uint8_t>(m_lerpProgress * 255));
    }
    
    for (int i = 0; i < state->count; i++)
      state->setRgb(i, rgb.Dim(m_brightness));
  }

private:
  uint8_t m_brightness;
  TimeLerper<float> m_lerper;
  float m_lerpProgress;
  float m_targetTemperature;
  RgbColor m_targetColor = {0, 0, 0};
};

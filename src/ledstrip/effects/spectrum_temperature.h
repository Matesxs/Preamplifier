#pragma once

#include <NeoPixelBus.h>

#include "base_effect.h"
#include "settings.h"
#include "interpolator.h"
#include "ledstrip/led_state.h"
#include "io_handling/spectrum_analyzer.h"
#include "io_handling/temperature_reader.h"
#include "time_lerper.h"
#include "settings.h"
#include "helpers.h"

class VolumeBarsTemperature: public BaseEffect
{
public:
  VolumeBarsTemperature(uint8_t brightness = 255) :
    m_brightness(brightness),
    m_lerpProgress(0.0)
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

    uint16_t maxVal = SpectrumAnalyzer::spectrumRaw[0];
    for (auto& val : SpectrumAnalyzer::spectrumRaw)
      maxVal = max(maxVal, val);

    for (int i = 0; i < state->count; i++)
      state->setRgb(i, {0, 0, 0});

    float rawLedsToLight = min((static_cast<float>(maxVal) / SPECTRUM_MAX_VAL) * EFFECTS_SPECTRUM_GAIN * static_cast<float>(state->count), static_cast<float>(state->count));
    int ledsToLight = static_cast<int>(rawLedsToLight);

    for(int i = 0; i < ledsToLight; i++)
      state->setRgb(i, rgb);

    if (rawLedsToLight > 0)
    {
      float progres = rawLedsToLight - static_cast<float>(ledsToLight);
      state->setRgb(min(state->count - 1, ledsToLight), rgb.Dim(m_brightness).Dim(static_cast<uint8_t>(progres * 255)));
    }
  }

private:
  uint8_t m_brightness;
  TimeLerper<float> m_lerper;
  float m_lerpProgress;
  float m_targetTemperature;
  RgbColor m_targetColor = {0, 0, 0};
};

class SpectrumTemperature: public BaseEffect
{
public:
  SpectrumTemperature(uint8_t brightness = 255) :
    m_brightness(brightness),
    m_lerpProgress(0.0)
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

    m_interpolator.SetData(SpectrumAnalyzer::spectrumRaw);
    m_interpolator.Interpolate();

    for (int i = 0; i < state->count; i++)
    {
      float progres = (static_cast<float>(m_interpolator[i]) / 4095.0f) * 255.0f;
      state->setRgb(i, rgb.Dim(m_brightness).Dim(static_cast<uint8_t>(progres)));
    }
  }

private:
  uint8_t m_brightness;
  Interpolator<uint16_t, SA_BANDS_NUMBER, LED_SEGMENT_COUNT> m_interpolator;
  TimeLerper<float> m_lerper;
  float m_lerpProgress;
  float m_targetTemperature;
  RgbColor m_targetColor = {0, 0, 0};
};

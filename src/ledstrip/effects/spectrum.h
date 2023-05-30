#pragma once

#include <NeoPixelBus.h>

#include "base_effect.h"
#include "settings.h"
#include "interpolator.h"
#include "ledstrip/led_state.h"
#include "io_handling/spectrum_analyzer.h"

class VolumeBars: public BaseEffect
{
public:
  VolumeBars()
  {
  }

  VolumeBars(RgbColor color) : 
    BaseEffect(color)
  {
  }
  
  virtual void render()
  {
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
      state->setRgb(min(state->count - 1, ledsToLight), rgb.Dim(static_cast<uint8_t>(progres * 255)));
    }
  }
};

class Spectrum: public BaseEffect
{
public:
  Spectrum()
  {
  }

  Spectrum(RgbColor color) : 
    BaseEffect(color)
  {
  }

  virtual void render()
  {
    m_interpolator.SetData(SpectrumAnalyzer::spectrumRaw);
    m_interpolator.Interpolate();

    for (int i = 0; i < state->count; i++)
    {
      float progres = (static_cast<float>(m_interpolator[i]) / 4095.0f) * 255.0f;
      state->setRgb(i, rgb.Dim(static_cast<uint8_t>(progres)));
    }
  }

private:
  Interpolator<uint16_t, SA_BANDS_NUMBER, LED_SEGMENT_COUNT> m_interpolator;
};

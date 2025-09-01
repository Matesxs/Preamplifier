#pragma once

#include <NeoPixelBus.h>

#include "base_effect.h"
#include "settings.h"
#include "interpolator.h"
#include "ledstrip/led_state.h"
#include "io_handling/spectrum_analyzer.h"

class VolumeBars final : public BaseEffect
{
public:
  VolumeBars() = default;

  explicit VolumeBars(const RgbColor color) :
    BaseEffect(color)
  {
  }
  
  void render() override
  {
    uint16_t maxVal = SpectrumAnalyzer::spectrumRaw[0];
    for (auto& val : SpectrumAnalyzer::spectrumRaw)
      maxVal = max(maxVal, val);

    for (int i = 0; i < state->count; i++)
      state->setRgb(i, {0, 0, 0});

    const float rawLedsToLight = min((static_cast<float>(maxVal) / SPECTRUM_MAX_VAL) * EFFECTS_SPECTRUM_GAIN * static_cast<float>(state->count), static_cast<float>(state->count));
    const int ledsToLight = static_cast<int>(rawLedsToLight);

    for(int i = 0; i < ledsToLight; i++)
      state->setRgb(i, rgb);

    if (rawLedsToLight > 0)
    {
      const float progres = rawLedsToLight - static_cast<float>(ledsToLight);
      state->setRgb(min(state->count - 1, ledsToLight), rgb.Dim(static_cast<uint8_t>(progres * 255)));
    }
  }
};

class Spectrum final : public BaseEffect
{
public:
  Spectrum() = default;

  explicit Spectrum(const RgbColor color) :
    BaseEffect(color)
  {
  }

  void render() override
  {
    m_interpolator.SetData(SpectrumAnalyzer::spectrumRaw);
    m_interpolator.Interpolate();

    for (int i = 0; i < state->count; i++)
    {
      const float progres = (static_cast<float>(m_interpolator[i]) / 4095.0f) * 255.0f;
      state->setRgb(i, rgb.Dim(static_cast<uint8_t>(progres)));
    }
  }

private:
  Interpolator<uint16_t, SA_BANDS_NUMBER, LED_SEGMENT_COUNT> m_interpolator;
};

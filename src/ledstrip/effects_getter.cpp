#include "effects_getter.h"

#include "effects/solid_color.h"
#include "effects/wave.h"
#include "effects/breathing.h"
#include "effects/kitt.h"
#include "effects/spectrum.h"
#include "effects/temperature.h"
#include "effects/spectrum_temperature.h"

BaseEffect* effectFromData(const uint32_t effIdx, const RgbColor color, const uint8_t brightness)
{
  switch (effIdx)
  {
  case 0:
    return new SolidColor();

  case 1:
    return new SolidColor(color);

  case 2:
    return new Temperature(brightness);

  case 3:
    return new WaveEffect(color);

  case 4:
    return new Breath(color);

  case 5:
    return new Kitt(color);

  case 6:
    return new Spectrum(color);

  case 7:
    return new SpectrumTemperature(brightness);

  case 8:
    return new VolumeBars(color);

  case 9:
    return new VolumeBarsTemperature(brightness);
  
  default:
    break;
  }

  return nullptr;
}

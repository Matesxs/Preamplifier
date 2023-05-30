#include "effects_getter.h"

#include "effects/solid_color.h"
#include "effects/wave.h"
#include "effects/breathing.h"
#include "effects/kitt.h"
#include "effects/spectrum.h"
#include "effects/temperature.h"
#include "effects/spectrum_temperature.h"

BaseEffect* effectFromData(uint32_t effIdx, RgbColor color, uint8_t brightness)
{
  switch (effIdx)
  {
  case 0:
    return new SolidColor();
    break;

  case 1:
    return new SolidColor(color);
    break;

  case 2:
    return new Temperature(brightness);
    break;

  case 3:
    return new WaveEffect(color);
    break;

  case 4:
    return new Breath(color);
    break;

  case 5:
    return new Kitt(color);
    break;

  case 6:
    return new Spectrum(color);
    break;

  case 7:
    return new SpectrumTemperature(brightness);
    break;

  case 8:
    return new VolumeBars(color);
    break;

  case 9:
    return new VolumeBarsTemperature(brightness);
    break;
  
  default:
    break;
  }

  return NULL;
}

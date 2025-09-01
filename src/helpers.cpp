#include "helpers.h"
#include "settings.h"
#include "interpolator.h"

float mapfloat(const float x, const float in_min, const float in_max, const float out_min, const float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

PointInterpolator<float, float, sizeof(temperatureToHueMapping) / sizeof(temperatureToHueMapping[0])>hueIntepolator(temperatureToHueMapping);
float temperatureToHue(const float temperature)
{
  return max(0.0f, min(1.0f, hueIntepolator.Get(temperature)));
}

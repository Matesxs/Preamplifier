#pragma once

#include <NeoPixelBus.h>

#include "effects/base_effect.h"

#define MAX_LED_COUNT 100

class LedState final
{
public:
  RgbColor values[MAX_LED_COUNT] = {{0, 0, 0}};
  int count = 0;
  bool dirty = false;
  NeoPixelBus<NeoBrgFeature, NeoWs2811Method> &pixels;
  BaseEffect *function = nullptr;
  
  explicit LedState(NeoPixelBus<NeoBrgFeature, NeoWs2811Method> &ledPixels)
  :pixels(ledPixels)
  {
    count = pixels.PixelCount();
  }

  void setFunction(BaseEffect *newFunction)
  {
    delete function;
    function = newFunction;

    if(!function)
      return;
    function->state = this;
  }

  void setRgb(const int i, const uint8_t r, const uint8_t g, const uint8_t b)
  {
    values[i][0] = r;
    values[i][1] = g;
    values[i][2] = b;
    dirty = true;
  }

  void setRgb(const int i, const RgbColor color)
  {
    values[i] = color;
    dirty = true;
  }

  RgbColor getColorOfPixel(const int index) const
  {
    return pixels.GetPixelColor(index);
  }

  void render() const
  {
    if(function)
      function->render();
  }

  void setValues(LedState &to)
  {
    for(int i = 0; i < count; i++)
      for(int j = 0; j < 3; j++)
        values[i][j] = to.values[i][j];
    setFunction(to.function);
    to.function = nullptr;
    dirty = true;
  }

  void commit()
  {
    if(!dirty)
      return;
      
    for(int i = 0; i < count; i++)      
      pixels.SetPixelColor(i, values[i]);

    dirty = false;
  }

  void fade(const LedState &to, const double prog)
  {
    for(int i = 0; i < count; i++)
    {
      RgbColor targetColor(RgbColor::LinearBlend(values[i], to.values[i], static_cast<uint8_t>(prog * 255.0)));
      if (pixels.GetPixelColor(i) != targetColor)
        pixels.SetPixelColor(i, targetColor);
    }

    dirty = true;
  }
};

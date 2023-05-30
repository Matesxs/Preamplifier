#pragma once

#include <NeoPixelBus.h>

#include "lerper.h"
#include "base_effect.h"
#include "ledstrip/led_state.h"

class SolidColor: public BaseEffect
{
public:
  SolidColor()
  {
  }

  SolidColor(RgbColor color) : 
    BaseEffect(color)
  {
    
  }
  
  virtual void render()
  {
    for(int i = 0; i < state->count; i++)
      state->setRgb(i, rgb);
  }
};

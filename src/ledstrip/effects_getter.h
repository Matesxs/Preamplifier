#pragma once

#include "Arduino.h"
#include <NeoPixelBus.h>

#include "effects/base_effect.h"

BaseEffect* effectFromData(uint32_t effIdx, RgbColor color, uint8_t brightness);

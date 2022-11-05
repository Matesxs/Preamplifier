#pragma once

#include <RotaryEncoder.h>
#include <JC_Button.h>
#include <U8g2lib.h>

#include "preamp.h"
#include "screen_definitions.h"

extern Button encoder_button;
extern Button channel_switch_button;
extern RotaryEncoder encoder;
extern U8G2_SH1122_256X64_F_4W_HW_SPI display;

extern volatile Screens active_screen;

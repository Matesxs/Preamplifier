#pragma once

#include <Arduino.h>
#include <RotaryEncoder.h>
#include <JC_Button.h>
#include <U8g2lib.h>

#include "preamp.h"
#include "defines.h"

extern Button encoder_button;
extern Button channel_switch_button;
extern RotaryEncoder encoder;
extern U8G2_SH1122_256X64_F_4W_HW_SPI display;

extern volatile Screens active_screen;

extern volatile float temps_store[];
extern volatile uint16_t potentiomater_values[];

extern bool shortPush;
extern bool longPush;

extern Preamp preamp;

#pragma once

// Rotary encoder
#define ROT_A 2
#define ROT_B 4
#define ROT_PUSH 15

// Temperature probe
#define ONE_WIRE_BUS 19

// Display
#define DISPLAY_DATA_COMMAND_PIN 16
#define DISPLAY_RESET_PIN 17
#define DISPLAY_CS_PIN 5
#define DISPLAY_CLOCK_PIN 18
#define DISPLAY_DATA_PIN 23

// Channel switch
#define CHANNEL_SW_BTN 26

// Potentiometers
#define POT1 32
#define POT2 33
#define POT3 13

#define POT4 35

#ifndef POT4
#define POT_COUNT 3
#else
#define POT_COUNT 4
#endif

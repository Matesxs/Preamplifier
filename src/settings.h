#pragma once

#include <Arduino.h>
#include <tuple>

// Temperature
// PS. Max number of sensors for individual values is 2!
#define ENABLE_TEMPERATURE_MONITORING
#define UPDATE_TEMP_INTERVAL_MS 5000
// #define ONLY_MAX_TEMPERATURE
#define OVERHEAT_TEMPERATURE 80.0

// Display settings
#define DISPLAY_UPDATE_INTERVAL_MS 12

#define BACK_TO_MAIN_SCREEN_TIMEOUT_S 120
#define BACK_TO_MAIN_SCREEN_POPUP_TIMEOUT_S 5

const String channel_names[] = { "1", "2", "3", "BT" }; // Need to be same length as input mapings!
const int input_mapping[] = { 2, 3, 0, 1 }; // There must be always atlast one input! Number of channels is determined from this!
const int number_of_channels = max((int)(sizeof(input_mapping) / sizeof(input_mapping[0])), 1);

// Input settings
#define ENCODER_PULLING_RATE_MS 12

#define DEBOUNCE_BUTTONS_MS 60
#define BUTTONS_PULLING_RATE_MS 10

#define ENCODER_SHORT_CLICK_DUR_MS 70
#define ENCODER_LONG_CLICK_DUR_MS 300

#define ENABLE_ENCODER_PUSH_LENGTH_INDICATOR
#define ENCODER_SHORT_PUSH_INDICATOR_LENGTH 8
#define ENCODER_LONG_PUSH_INDICATOR_LENGTH 16

//////////////////////////////// SELECT /////////////////////////////////////
// #define INPUT_SWITCH_IN_MENU
// #define INPUT_SWITCH_BUTTON BUTTON1
#define CHANNEL_ROTARY_SWITCH // Dont combina with the other input switching methods
#define AUTO_SWITCHING // TODO: Implement // Dont combine with rotary switch
/////////////////////////////////////////////////////////////////////////////

#ifdef CHANNEL_ROTARY_SWITCH
  #define DEBOUNCE_ROTARY_SWITCH 300
  #define ROTARY_CHANNEL1_PIN ROT1_PIN
  #define ROTARY_CHANNEL2_PIN ROT2_PIN
  #define ROTARY_CHANNEL3_PIN ROT3_PIN
  #define ROTARY_CHANNEL4_PIN ROT4_PIN
#endif

#define POT_MIN_VAL 70
#define POT_MAX_VAL 4025
#define POT_FILTER_SAMPLES 8 // 2, 4, 8, 16, 32, 64 ...
#define BASS_GAIN_POTENTIOMETER POT1
#define MIDDLE_GAIN_POTENTIOMETER POT2
#define TREBLE_GAIN_POTENTIOMETER POT3
// #define INPUT_GAIN_POTENTIOMETER POT4
#define POTENTIOMETER_PULLING_RATE_MS 20
#define POTENTIOMETER_MIN_CHANGE 50
#define POTENTIOMETER_MIN_DB_CHANGE_FOR_POPUP 2

// Settings saving
#define SETTING_SAVE_INTERVAL_MS 10000

// Spectrum analyzer settings
#define SPECTRUM_MINIMALISTIC_CLIPPING_DETECTION // TODO: Implement

#define SPECTRUM_ADC_ADDRESS 0x48
#define SPECTRUM_AVG_BUFFER 8 // 2, 4, 8, 16, 32, 64 ...
#define SPECTRUM_MAX_VAL 4095
#define SPECTRUM_UPDATE_INTERVAL_MS 10
#define SPECTRUM_DISPLAY_BANDS 19

// Led strip
#define ENABLE_LED_STRIP
#define LED_SEGMENT_COUNT 4
#define EFFECTS_SPECTRUM_GAIN 2
const std::tuple<float, float>temperatureToHueMapping[] = { {20.0f, 250.0f / 360.0f}, { 35.0f, 175.0f / 360.0f }, { 60.0f, 60.0f / 360.0f }, { OVERHEAT_TEMPERATURE, 0.0f } };

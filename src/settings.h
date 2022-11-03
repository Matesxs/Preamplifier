#pragma once

#include <Arduino.h>

#define ENABLE_DEBUG

#define SHORT_CLICK_DUR_MS 70
#define LONG_CLICK_DUR_MS 500
#define ENABLE_PUSH_LENGTH_INDICATOR
#define SHORT_PUSH_INDICATOR_LENGTH 8
#define LONG_PUSH_INDICATOR_LENGTH 16

#define BACK_TO_MAIN_SCREEN_TIMEOUT_S 120
#define BACK_TO_MAIN_SCREEN_POPUP_TIMEOUT_S 5

#define ENABLE_SCREEN_SAVER
#define SCREEN_SAVER_TIMEOUT_S 300

#define UPDATE_TEMP_INTERVAL_MS 2000

#define POTENTIOMETER_PULLING_RATE_MS 50
#define POTENTIOMETER_MIN_CHANGE 150

#define SETTING_SAVE_INTERVAL_MS 5000

#ifdef ENABLE_DEBUG
#define DEBUG(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif


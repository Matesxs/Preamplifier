#pragma once

#include <Arduino.h>

#include "settings.h"

enum Screens
{
  SCREEN_SAVER,
  MAIN_SCREEN,
  MAIN_MENU,
  MUTE_SCREEN,

  // Popups
  TREBLE_GAIN,
  MIDDLE_GAIN,
  BASS_GAIN,
  INPUT_GAIN,
  MASTER_VOLUME,
  /////////

  // MAIN MENU
  INPUT_SWITCH,
  SPECTRUM,
  LOUDNESS,
  ATTENUATION,
  BASS_FILTER,
  MIDDLE_FILTER,
  TREBLE_FILTER,
  SUBWOOFER,
  SOFT_STEPS,
  SOFT_MUTE_TIME,
  LED_STRIP,
  SETTINGS,
  /////////////

  // Loudness menu
  LOUDNESS_ATT,
  LOUDNESS_FREQ,
  LOUDNESS_HIGH_BOOST,
  ////////////////

  // Attenuations menu
  ATTENUATION_LEFT,
  ATTENUATION_RIGHT,
  ATTENUATION_SUB,
  ////////////////////

  // Bass menu
  BASS_QFACTOR,
  BASS_FREQ,
  BASS_DC_MODE,
  ////////////

  // Middle menu
  MIDDLE_QFACTOR,
  MIDDLE_FREQ,
  //////////////

  // Treble menu
  TREBLE_FREQ,
  //////////////

  // Sub menu
  SUB_CUTOFF_FREQ,
  SUB_ATT,
  ///////////

  // Soft Steps
  SOFT_STEPS_TIME,
  SOFT_STEPS_VOLUME,
  SOFT_STEPS_LOUDNESS,
  SOFT_STEPS_MIDDLE,
  SOFT_STEPS_BASS,
  SOFT_STEPS_LEFT,
  SOFT_STEPS_RIGHT,
  SOFT_STEPS_SUB,
  /////////////

  // LED Strip menu
  LED_STRIP_EFFECT,
  LED_STRIP_COLOR,
  /////////////////

  // LED Strip color submenu
  LED_STRIP_COLOR_RED,
  LED_STRIP_COLOR_GREEN,
  LED_STRIP_COLOR_BLUE,
  LED_STRIP_COLOR_BRIGHTNESS,

  // Settings menu
  SETTINGS_SCREENSAVER,
  SETTINGS_CLIPPING_DETECTION,
  SETTINGS_OVERHEAT,
  SETTINGS_BRIGHTNESS,
  SETTINGS_FACT_RESET,

  // Settings screen saver submenu
  SETTINGS_SCREENSAVER_ENABLE,
  SETTINGS_SCREENSAVER_DELAY,
  SETTINGS_SCREENSAVER_SPECTRUM,

  // Settings overheat detection submenu
  SETTINGS_OVERHEAT_ENABLE,
  SETTINGS_OVERHEAT_MUTE,
  SETTINGS_OVERHEAT_BLINK,
};

#ifdef INPUT_GAIN_POTENTIOMETER
  #ifndef INPUT_SWITCH_IN_MENU
    #ifdef ENABLE_LED_STRIP
      const String main_menu_names[] = { "Spectrum", "Loudness", "Balance", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute Time", "LED Strip", "Settings", "Turn off Screen" };
      const Screens main_menu_index_to_screen[] = { Screens::SPECTRUM, Screens::LOUDNESS, Screens::ATTENUATION, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE_TIME, Screens::LED_STRIP, Screens::SETTINGS, Screens::SCREEN_SAVER };
    #else
      const String main_menu_names[] = { "Spectrum", "Loudness", "Balance", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute Time", "Settings", "Turn off Screen" };
      const Screens main_menu_index_to_screen[] = { Screens::SPECTRUM, Screens::LOUDNESS, Screens::ATTENUATION, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE_TIME, Screens::SETTINGS, Screens::SCREEN_SAVER };
    #endif
  #else
    #ifdef ENABLE_LED_STRIP
      const String main_menu_names[] = { "Input", "Spectrum", "Loudness", "Balance", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute Time", "LED Strip", "Settings", "Turn off Screen" };
      const Screens main_menu_index_to_screen[] = { Screens::INPUT_SWITCH, Screens::SPECTRUM, Screens::LOUDNESS, Screens::ATTENUATION, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE_TIME, Screens::LED_STRIP, Screens::SETTINGS, Screens::SCREEN_SAVER };
    #else
      const String main_menu_names[] = { "Input", "Spectrum", "Loudness", "Balance", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute Time", "Settings", "Turn off Screen" };
      const Screens main_menu_index_to_screen[] = { Screens::INPUT_SWITCH, Screens::SPECTRUM, Screens::LOUDNESS, Screens::ATTENUATION, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE_TIME, Screens::SETTINGS, Screens::SCREEN_SAVER };
    #endif
  #endif
#else
  #ifndef INPUT_SWITCH_IN_MENU
    #ifdef ENABLE_LED_STRIP
      const String main_menu_names[] = { "Spectrum", "Loudness", "Balance", "Input Gain", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute Time", "LED Strip", "Settings", "Turn off Screen" };
      constexpr Screens main_menu_index_to_screen[] = {Screens::SPECTRUM, Screens::LOUDNESS, Screens::ATTENUATION, Screens::INPUT_GAIN, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE_TIME, Screens::LED_STRIP, Screens::SETTINGS, Screens::SCREEN_SAVER };
    #else
      const String main_menu_names[] = { "Spectrum", "Loudness", "Balance", "Input Gain", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute Time", "Settings", "Turn off Screen" };
      const Screens main_menu_index_to_screen[] = {Screens::SPECTRUM, Screens::LOUDNESS, Screens::ATTENUATION, Screens::INPUT_GAIN, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE_TIME, Screens::SETTINGS, Screens::SCREEN_SAVER };
    #endif
  #else
    #ifdef ENABLE_LED_STRIP
      const String main_menu_names[] = { "Input", "Spectrum", "Loudness", "Balance", "Input Gain", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute Time", "LED Strip", "Settings", "Turn off Screen" };
      const Screens main_menu_index_to_screen[] = { Screens::INPUT_SWITCH, Screens::SPECTRUM, Screens::LOUDNESS, Screens::ATTENUATION, Screens::INPUT_GAIN, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE_TIME, Screens::LED_STRIP, Screens::SETTINGS, Screens::SCREEN_SAVER };
    #else
      const String main_menu_names[] = { "Input", "Spectrum", "Loudness", "Balance", "Input Gain", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute Time", "Settings", "Turn off Screen" };
      const Screens main_menu_index_to_screen[] = { Screens::INPUT_SWITCH, Screens::SPECTRUM, Screens::LOUDNESS, Screens::ATTENUATION, Screens::INPUT_GAIN, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE_TIME, Screens::SETTINGS, Screens::SCREEN_SAVER };
    #endif
  #endif
#endif
constexpr int number_of_menu_items = sizeof(main_menu_names) / sizeof(main_menu_names[0]);

const String loudness_menu_names[] = { "Attenuation", "Center Frequency", "High Boost" };
constexpr Screens loudness_menu_screens[] = { Screens::LOUDNESS_ATT, Screens::LOUDNESS_FREQ, Screens::LOUDNESS_HIGH_BOOST };
constexpr int number_of_loudness_menu_items = sizeof(loudness_menu_names) / sizeof(loudness_menu_names[0]);
constexpr int loudness_center_freqs[] = { 0 , 400, 800, 2400 };

const String attenuations_menu_names[] = { "Left", "Right", "Sub" };
constexpr Screens attenuations_menu_screens[] = { Screens::ATTENUATION_LEFT, Screens::ATTENUATION_RIGHT, Screens::ATTENUATION_SUB };
constexpr int number_of_attenuations_menu_items = sizeof(attenuations_menu_names) / sizeof(attenuations_menu_names[0]);

#ifdef BASS_GAIN_POTENTIOMETER
  const String bass_menu_names[] = { "Q Factor", "Center Frequency", "DC Mode" };
  constexpr Screens bass_menu_screens[] = { Screens::BASS_QFACTOR, Screens::BASS_FREQ, Screens::BASS_DC_MODE };
#else
  const String bass_menu_names[] = { "Gain", "Q Factor", "Center Frequency", "DC Mode" };
  const Screens bass_menu_screens[] = { Screens::BASS_GAIN, Screens::BASS_QFACTOR, Screens::BASS_FREQ, Screens::BASS_DC_MODE };
#endif
constexpr int number_of_bass_menu_items = sizeof(bass_menu_names) / sizeof(bass_menu_names[0]);
constexpr float bass_q_factors[] = { 1.0f, 1.25f, 1.5f, 2.0f };
constexpr int bass_center_freqs[] = { 60, 80, 100, 200 };

#ifdef MIDDLE_GAIN_POTENTIOMETER
  const String middle_menu_names[] = { "Q Factor", "Center Frequency" };
  constexpr Screens middle_menu_screens[] = { Screens::MIDDLE_QFACTOR, Screens::MIDDLE_FREQ };
#else
  const String middle_menu_names[] = { "Gain", "Q Factor", "Center Frequency" };
  const Screens middle_menu_screens[] = { Screens::MIDDLE_GAIN, Screens::MIDDLE_QFACTOR, Screens::MIDDLE_FREQ };
#endif
constexpr int number_of_middle_menu_items = sizeof(middle_menu_names) / sizeof(middle_menu_names[0]);
constexpr float middle_q_factors[] = { 0.5f, 0.75f, 1.0f, 1.25f };
constexpr int middle_center_freqs[] = { 500, 1000, 1500, 2500 };

#ifdef TREBLE_GAIN_POTENTIOMETER
  const String treble_menu_names[] = { "Center Frequency" };
  constexpr Screens treble_menu_screens[] = { Screens::TREBLE_FREQ };
#else
  const String treble_menu_names[] = { "Gain", "Center Frequency" };
  const Screens treble_menu_screens[] = { Screens::TREBLE_GAIN, Screens::TREBLE_FREQ };
#endif
constexpr int number_of_treble_menu_items = sizeof(treble_menu_names) / sizeof(treble_menu_names[0]);
constexpr float treble_center_freqs[] = { 10.0f, 12.5f, 15.0f, 17.5f };

const String sub_menu_names[] = { "Cutoff Frequency", "Attenuation" };
constexpr Screens sub_menu_screens[] = { Screens::SUB_CUTOFF_FREQ, Screens::SUB_ATT };
constexpr int number_of_sub_menu_items = sizeof(sub_menu_names) / sizeof(sub_menu_names[0]);
constexpr int sub_cutoff_freqs[] = { 0, 80, 120, 160 };


const String soft_steps_menu_names[] = { "Step Time", "Volume", "Loudness", "Middle", "Bass", "Left Att", "Right Att", "Sub Att" };
constexpr Screens soft_steps_menu_screens[] = { Screens::SOFT_STEPS_TIME, Screens::SOFT_STEPS_VOLUME, Screens::SOFT_STEPS_LOUDNESS, Screens::SOFT_STEPS_MIDDLE, Screens::SOFT_STEPS_BASS, Screens::SOFT_STEPS_LEFT, Screens::SOFT_STEPS_RIGHT, Screens::SOFT_STEPS_SUB };
constexpr int number_of_soft_steps_menu_items = sizeof(soft_steps_menu_names) / sizeof(soft_steps_menu_names[0]);
constexpr float soft_step_times[] = {0.160f, 0.321f, 0.642f, 1.28f, 2.56f, 5.12f, 10.24f, 20.48f};
constexpr float soft_mute_times[] = { 0.48f, 0.96f, 123.0f };


const String led_strip_menu_names[] = { "Effect", "Color" };
constexpr Screens led_strip_menu_screens[] = { Screens::LED_STRIP_EFFECT, Screens::LED_STRIP_COLOR };
constexpr int number_of_led_strip_menu_items = sizeof(led_strip_menu_names) / sizeof(led_strip_menu_names[0]);

const String led_strip_effect_menu_names[] = { "Off", "Solid", "Temperature", "Wave", "Breathing", "KITT", "Spectrum", "Spectrum Temp", "Volume Bars", "Volume Bars Temp" };
constexpr int number_of_led_strip_effect_menu_items = sizeof(led_strip_effect_menu_names) / sizeof(led_strip_effect_menu_names[0]);

const String led_strip_color_menu_names[] = { "Red", "Green", "Blue", "Brightness" };
constexpr Screens led_strip_color_menu_screens[] = { Screens::LED_STRIP_COLOR_RED, Screens::LED_STRIP_COLOR_GREEN, Screens::LED_STRIP_COLOR_BLUE, Screens::LED_STRIP_COLOR_BRIGHTNESS };
constexpr int number_of_led_strip_color_menu_items = sizeof(led_strip_color_menu_names) / sizeof(led_strip_color_menu_names[0]);


#ifdef ENABLE_TEMPERATURE_MONITORING
  const String settings_menu_names[] = { "Screen Saver", "Clip Detection", "Overheat Detection", "Brightness", "Factory Reset" };
  constexpr Screens settings_menu_screens[] = { Screens::SETTINGS_SCREENSAVER, Screens::SETTINGS_CLIPPING_DETECTION, Screens::SETTINGS_OVERHEAT, Screens::SETTINGS_BRIGHTNESS, Screens::SETTINGS_FACT_RESET };
#else
  const String settings_menu_names[] = { "Screen Saver", "Clip Detection", "Brightness", "Factory Reset" };
  const Screens settings_menu_screens[] = { Screens::SETTINGS_SCREENSAVER, Screens::SETTINGS_CLIPPING_DETECTION, Screens::SETTINGS_BRIGHTNESS, Screens::SETTINGS_FACT_RESET };
#endif
constexpr int number_of_settings_menu_items = sizeof(settings_menu_names) / sizeof(settings_menu_names[0]);

const String settings_screensaver_menu_names[] = { "Enable", "Delay", "Use spectrum" };
constexpr Screens settings_screensaver_menu_screens[] = { Screens::SETTINGS_SCREENSAVER_ENABLE, Screens::SETTINGS_SCREENSAVER_DELAY, Screens::SETTINGS_SCREENSAVER_SPECTRUM };
constexpr int number_of_settings_screensaver_menu_items = sizeof(settings_screensaver_menu_names) / sizeof(settings_screensaver_menu_names[0]);

#ifdef ENABLE_LED_STRIP
  const String settings_overheat_menu_names[] = { "Enable", "Mute on OH", "Blink on OH" };
  constexpr Screens settings_overheat_menu_screens[] = { Screens::SETTINGS_OVERHEAT_ENABLE, Screens::SETTINGS_OVERHEAT_MUTE, Screens::SETTINGS_OVERHEAT_BLINK };
#else
  const String settings_overheat_menu_names[] = { "Enable", "Mute on OH" };
  const Screens settings_overheat_menu_screens[] = { Screens::SETTINGS_OVERHEAT_ENABLE, Screens::SETTINGS_OVERHEAT_MUTE };
#endif
constexpr int number_of_settings_overheat_menu_items = sizeof(settings_overheat_menu_names) / sizeof(settings_overheat_menu_names[0]);

#pragma once

#include <Arduino.h>

#include "pins.h"

typedef enum
{
  ENC_CW,
  ENC_CCW,
  ENC_PUSH,
  ENC_LPUSH,
  POT1_CH,
  POT2_CH,
  POT3_CH,
  POT4_CH,
  CH_SW_PUSH,
} InputType;

typedef enum
{
  SCREEN_SAVER,
  MAIN_SCREEN,
  MAIN_MENU,
  MUTE_SCREEN,

  // Popups
  TREBLE_GAIN,
  MIDDLE_GAIN,
  BASS_GAIN,
  MASTER_VOLUME,
  /////////

  // MAIN MENU
  LOUDNESS,
  ATTENUATION,
  INPUT_GAIN,
  BASS_FILTER,
  MIDDLE_FILTER,
  TREBLE_FILTER,
  SUBWOOFER,
  FACT_RESET,
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
} Screens;

#ifdef POT4
const String main_menu_names[] = { "Loudness", "Attenuations", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Reset" };
const Screens main_menu_index_to_screen[] = {Screens::LOUDNESS, Screens::ATTENUATION, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::FACT_RESET};
#else
const String main_menu_names[] = { "Loudness", "Attenuations", "Input Gain", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Reset" };
const Screens main_menu_index_to_screen[] = {Screens::LOUDNESS, Screens::ATTENUATION, Screens::INPUT_GAIN, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::FACT_RESET};
#endif
const int number_of_menu_items = sizeof(main_menu_names) / sizeof(main_menu_names[0]);

const String loudness_menu_names[] = { "Attenuation", "Center Frequency", "High Boost" };
const int number_of_loudness_menu_items = sizeof(loudness_menu_names) / sizeof(loudness_menu_names[0]);
const Screens loudness_menu_screens[] = { Screens::LOUDNESS_ATT, Screens::LOUDNESS_FREQ, Screens::LOUDNESS_HIGH_BOOST };
const int loudness_center_freqs[] = { 0 , 400, 800, 2400 };

const String attenuations_menu_names[] = { "Left", "Right", "Sub" };
const int number_of_attenuations_menu_items = sizeof(attenuations_menu_names) / sizeof(attenuations_menu_names[0]);
const Screens attenuations_menu_screens[] = { Screens::ATTENUATION_LEFT, Screens::ATTENUATION_RIGHT, Screens::ATTENUATION_SUB };

const String bass_menu_names[] = { "Q Factor", "Center Frequency", "DC Mode" };
const int number_of_bass_menu_items = sizeof(bass_menu_names) / sizeof(bass_menu_names[0]);
const Screens bass_menu_screens[] = { Screens::BASS_QFACTOR, Screens::BASS_FREQ, Screens::BASS_DC_MODE };
const float bass_q_factors[] = { 1.0f, 1.25f, 1.5f, 2.0f };
const int bass_center_freqs[] = { 60, 80, 100, 200 };

const String middle_menu_names[] = { "Q Factor", "Center Frequency" };
const int number_of_middle_menu_items = sizeof(middle_menu_names) / sizeof(middle_menu_names[0]);
const Screens middle_menu_screens[] = { Screens::MIDDLE_QFACTOR, Screens::MIDDLE_FREQ };
const float middle_q_factors[] = { 0.5f, 0.75f, 1.0f, 1.25f };
const int middle_center_freqs[] = { 500, 1000, 1500, 2500 };

const String treble_menu_names[] = { "Center Frequency" };
const int number_of_treble_menu_items = sizeof(treble_menu_names) / sizeof(treble_menu_names[0]);
const Screens treble_menu_screens[] = { Screens::TREBLE_FREQ };
const float treble_center_freqs[] = { 10.0f, 12.5f, 15.0f, 17.5f };

const String sub_menu_names[] = { "Cutoff Frequency", "Attenuation" };
const int number_of_sub_menu_items = sizeof(middle_menu_names) / sizeof(middle_menu_names[0]);
const Screens sub_menu_screens[] = { Screens::SUB_CUTOFF_FREQ, Screens::SUB_ATT };
const int sub_cutoff_freqs[] = { 0, 80, 120, 160 };

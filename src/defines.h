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
  SOFT_STEPS,
  SOFT_MUTE,
  FACT_RESET,
  /////////////

  // Loudness menu
  LOUDNESS_ATT,
  LOUDNESS_FREQ,
  LOUDNESS_HIGH_BOOST,
  LOUDNESS_SOFT_STEP,
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

  // Soft Mute
  SOFT_MUTE_ENABLE,
  SOFT_MUTE_TIME,
  ////////////
} Screens;

#ifdef POT4
const String main_menu_names[] = { "Loudness", "Attenuations", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute", "Reset" };
const Screens main_menu_index_to_screen[] = {Screens::LOUDNESS, Screens::ATTENUATION, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE, Screens::FACT_RESET};
#else
const String main_menu_names[] = { "Loudness", "Attenuations", "Input Gain", "Bass Filter", "Middle Filter", "Treble Filter", "Subwoofer", "Soft Steps", "Soft Mute", "Reset" };
const Screens main_menu_index_to_screen[] = {Screens::LOUDNESS, Screens::ATTENUATION, Screens::INPUT_GAIN, Screens::BASS_FILTER, Screens::MIDDLE_FILTER, Screens::TREBLE_FILTER, Screens::SUBWOOFER, Screens::SOFT_STEPS, Screens::SOFT_MUTE, Screens::FACT_RESET};
#endif
const int number_of_menu_items = sizeof(main_menu_names) / sizeof(main_menu_names[0]);

const String loudness_menu_names[] = { "Attenuation", "Center Frequency", "High Boost" /*, "Soft Step"*/ };
const int number_of_loudness_menu_items = sizeof(loudness_menu_names) / sizeof(loudness_menu_names[0]);
const Screens loudness_menu_screens[] = { Screens::LOUDNESS_ATT, Screens::LOUDNESS_FREQ, Screens::LOUDNESS_HIGH_BOOST /*, Screens::LOUDNESS_SOFT_STEP */ };
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
const int number_of_sub_menu_items = sizeof(sub_menu_names) / sizeof(sub_menu_names[0]);
const Screens sub_menu_screens[] = { Screens::SUB_CUTOFF_FREQ, Screens::SUB_ATT };
const int sub_cutoff_freqs[] = { 0, 80, 120, 160 };

const String soft_steps_menu_names[] = { "Step Time", "Volume", "Loudness", "Middle", "Bass", "Left Att", "Right Att", "Sub Att" };
const int number_of_soft_steps_menu_items = sizeof(soft_steps_menu_names) / sizeof(soft_steps_menu_names[0]);
const Screens soft_steps_menu_screens[] = { Screens::SOFT_STEPS_TIME, Screens::SOFT_STEPS_VOLUME, Screens::SOFT_STEPS_LOUDNESS, Screens::SOFT_STEPS_MIDDLE, Screens::SOFT_STEPS_BASS, Screens::SOFT_STEPS_LEFT, Screens::SOFT_STEPS_RIGHT, Screens::SOFT_STEPS_SUB };
const float soft_step_times[] = {0.160f, 0.321f, 0.642f, 1.28f, 2.56f, 5.12f, 10.24f, 20.48f};

const String soft_mute_menu_names[] = { "Enable", "Time" };
const int number_of_soft_mute_menu_items = sizeof(soft_mute_menu_names) / sizeof(soft_mute_menu_names[0]);
const Screens soft_mute_menu_screens[] = { Screens::SOFT_MUTE_ENABLE, Screens::SOFT_MUTE_TIME };
const float soft_mute_times[] = { 0.48f, 0.96f, 123.0f };

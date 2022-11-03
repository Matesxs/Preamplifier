#include "ui_controller.h"

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <stopWatch.h>

#include "settings.h"
#include "display_handler.h"
#include "screens.h"
#include "global_objects.h"
#include "preamp.h"
#include "helpers.h"

volatile Screens active_screen = Screens::MAIN_SCREEN;

volatile int main_menu_index = 0;
volatile int laudness_menu_index = 0;
volatile int attenuations_menu_index = 0;
volatile int bass_filter_menu_index = 0;
volatile int middle_filter_menu_index = 0;
volatile int treble_filter_menu_index = 0;
volatile int sub_menu_index = 0;

void handle_controll(InputType type)
{
  // DEBUG("Received controll signal %d\n", type);

  if (type == InputType::POT1_CH)
  {
    // DEBUG("POT1 val: %d\n", potentiomater_values[0]);

    int newGain = (int)map(potentiomater_values[0], 0, 4095, -15, 15);

    if (newGain != preamp.getTrebleGain())
    {
      // DEBUG("Mapped treble gain: %d\n", newGain);
      restartStopwatch();
      
      active_screen = Screens::TREBLE_GAIN;
      preamp.setTrebleGain(newGain);
    }
  }
  else if (type == InputType::POT2_CH)
  {
    // DEBUG("POT2 val: %d\n", potentiomater_values[1]);

    int newGain = (int)map(potentiomater_values[1], 0, 4095, -15, 15);

    if (newGain != preamp.getMiddleGain())
    {
      // DEBUG("Mapped middle gain: %d\n", newGain);
      restartStopwatch();
      
      active_screen = Screens::MIDDLE_GAIN;
      preamp.setMiddleGain(newGain);
    }
  }
  else if (type == InputType::POT3_CH)
  {
    // DEBUG("POT3 val: %d\n", potentiomater_values[2]);

    int newGain = (int)map(potentiomater_values[2], 0, 4095, -15, 15);

    if (newGain != preamp.getBassGain())
    {
      // DEBUG("Mapped bass gain: %d\n", newGain);
      restartStopwatch();
      
      active_screen = Screens::BASS_GAIN;
      preamp.setBassGain(newGain);
    }
  }
  else if (type == InputType::POT4_CH)
  {
    // DEBUG("POT4 val: %d\n", potentiomater_values[3]);

    int newInputGain = (int)map(potentiomater_values[3], 0, 4095, 0, 15);

    if (newInputGain != preamp.getGain())
    {
      // DEBUG("Mapped input gain: %d\n", newInputGain);
      restartStopwatch();
      
      active_screen = Screens::INPUT_GAIN;
      preamp.setInputGain(newInputGain);
    }
  }
  else if (type == InputType::CH_SW_PUSH)
  {
    preamp.rotateInput();
    DEBUG("Switching input to %d\n", preamp.getInput() + 1);
  }
  else
  {
    restartStopwatch();

    switch (active_screen)
    {
    case Screens::MUTE_SCREEN:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_CW)
      {
        DEBUG("Unmutting\n");
        active_screen = Screens::MAIN_SCREEN;
        preamp.unmute();
      }
      break;

    case Screens::MAIN_SCREEN:
      if (type == InputType::ENC_PUSH)
      {
        DEBUG("Going to menu\n");
        active_screen = Screens::MAIN_MENU;
        main_menu_index = 0;
      }
      else if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Mutting\n");
        active_screen = Screens::MUTE_SCREEN;
        preamp.mute();
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Poping master volume settings\n");
        active_screen = Screens::MASTER_VOLUME;
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Poping master volume settings\n");
        active_screen = Screens::MASTER_VOLUME;
      }
      break;

    case Screens::MAIN_MENU:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main screen\n");
        active_screen = Screens::MAIN_SCREEN;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = main_menu_index_to_screen[main_menu_index];
        DEBUG("Menu selected submenu: %d\n", active_screen);

        // Restarting submenu indexes
        if (active_screen == Screens::LOUDNESS)
          laudness_menu_index = 0;
        else if (active_screen == Screens::ATTENUATION)
          attenuations_menu_index = 0;
        else if (active_screen == Screens::BASS_FILTER)
          bass_filter_menu_index = 0;
        else if (active_screen == Screens::MIDDLE_FILTER)
          middle_filter_menu_index = 0;
        else if (active_screen == Screens::TREBLE_FILTER)
          treble_filter_menu_index = 0;
        else if (active_screen == Screens::SUBWOOFER)
          sub_menu_index = 0;
        /////////////////////////////
      }
      else if (type == InputType::ENC_CW)
      {
        if (main_menu_index < (number_of_menu_items - 1)) main_menu_index++;
        else main_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (main_menu_index > 0) main_menu_index--;
        else main_menu_index = (number_of_menu_items - 1);
      }
      break;

    case Screens::LOUDNESS:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = loudness_menu_screens[laudness_menu_index];
        DEBUG("Loudness menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (laudness_menu_index < (number_of_loudness_menu_items - 1)) laudness_menu_index++;
        else laudness_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (laudness_menu_index > 0) laudness_menu_index--;
        else laudness_menu_index = (number_of_loudness_menu_items - 1);
      }
      break;

    case Screens::ATTENUATION:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = attenuations_menu_screens[attenuations_menu_index];
        DEBUG("Attenuation menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (attenuations_menu_index < (number_of_attenuations_menu_items - 1)) attenuations_menu_index++;
        else attenuations_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (attenuations_menu_index > 0) attenuations_menu_index--;
        else attenuations_menu_index = (number_of_attenuations_menu_items - 1);
      }
      break;

    case Screens::BASS_FILTER:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = bass_menu_screens[bass_filter_menu_index];
        DEBUG("Bass filter menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (bass_filter_menu_index < (number_of_bass_menu_items - 1)) bass_filter_menu_index++;
        else bass_filter_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (bass_filter_menu_index > 0) bass_filter_menu_index--;
        else bass_filter_menu_index = (number_of_bass_menu_items - 1);
      }
      break;

    case Screens::MIDDLE_FILTER:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = middle_menu_screens[middle_filter_menu_index];
        DEBUG("Middle filter menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (middle_filter_menu_index < (number_of_middle_menu_items - 1)) middle_filter_menu_index++;
        else middle_filter_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (middle_filter_menu_index > 0) middle_filter_menu_index--;
        else middle_filter_menu_index = (number_of_middle_menu_items - 1);
      }
      break;

    case Screens::TREBLE_FILTER:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = treble_menu_screens[treble_filter_menu_index];
        DEBUG("Treble filter menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (treble_filter_menu_index < (number_of_treble_menu_items - 1)) treble_filter_menu_index++;
        else treble_filter_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (treble_filter_menu_index > 0) treble_filter_menu_index--;
        else treble_filter_menu_index = (number_of_treble_menu_items - 1);
      }
      break;

    case Screens::SUBWOOFER:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = sub_menu_screens[sub_menu_index];
        DEBUG("Subwoofer menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (sub_menu_index < (number_of_sub_menu_items - 1)) sub_menu_index++;
        else sub_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (sub_menu_index > 0) sub_menu_index--;
        else sub_menu_index = (number_of_sub_menu_items - 1);
      }
      break;

    case Screens::LOUDNESS_ATT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving loudness att submenu\n");
        active_screen = Screens::LOUDNESS;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing loudness attenuation\n");
        preamp.incLoudnessAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing loudness attenuation\n");
        preamp.decLoudnessAttenuation();
      }
      break;

    case Screens::LOUDNESS_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving loudness center freq submenu\n");
        active_screen = Screens::LOUDNESS;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing loudness center freq\n");
        preamp.incLoudnessCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing loudness center freq\n");
        preamp.decLoudnessCenterFreq();
      }
      break;

    case Screens::LOUDNESS_HIGH_BOOST:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving loudness high boost submenu\n");
        active_screen = Screens::LOUDNESS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching loudness high boost\n");
        preamp.switchLoudnessHighBoost();
      }
      break;

    case Screens::ATTENUATION_LEFT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving attenuation left submenu\n");
        active_screen = Screens::ATTENUATION;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing attenuation left\n");
        preamp.incLeftAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing attenuation left\n");
        preamp.decLeftAttenuation();
      }
      break;

    case Screens::ATTENUATION_RIGHT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving attenuation right submenu\n");
        active_screen = Screens::ATTENUATION;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing attenuation right\n");
        preamp.incRightAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing attenuation right\n");
        preamp.decRightAttenuation();
      }
      break;

    case Screens::ATTENUATION_SUB:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving attenuation sub submenu\n");
        active_screen = Screens::ATTENUATION;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing attenuation sub\n");
        preamp.incSubAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing attenuation sub\n");
        preamp.decSubAttenuation();
      }
      break;

    case Screens::BASS_QFACTOR:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving bass q factor submenu\n");
        active_screen = Screens::BASS_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing bass q factor\n");
        preamp.incBassQFactor();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing bass q factor\n");
        preamp.decBassQFactor();
      }
      break;

    case Screens::BASS_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving bass center freq submenu\n");
        active_screen = Screens::BASS_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing bass center freq\n");
        preamp.incBassCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing bass center freq\n");
        preamp.decBassCenterFreq();
      }
      break;

    case Screens::BASS_DC_MODE:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving bass dc submenu\n");
        active_screen = Screens::BASS_FILTER;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching bass dc\n");
        preamp.switchBassDC();
      }
      break;

    case Screens::MIDDLE_QFACTOR:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving middle q factor submenu\n");
        active_screen = Screens::MIDDLE_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing middle q factor\n");
        preamp.incMiddleQFactor();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing middle q factor\n");
        preamp.decMiddleQFactor();
      }
      break;

    case Screens::MIDDLE_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving middle center freq submenu\n");
        active_screen = Screens::MIDDLE_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing middle center freq\n");
        preamp.incMiddleCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing middle center freq\n");
        preamp.decMiddleCenterFreq();
      }
      break;

    case Screens::TREBLE_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving treble center freq submenu\n");
        active_screen = Screens::TREBLE_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing treble center freq\n");
        preamp.incTrebleCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing treble center freq\n");
        preamp.decTrebleCenterFreq();
      }
      break;

    case Screens::SUB_CUTOFF_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving sub cutoff freq submenu\n");
        active_screen = Screens::SUBWOOFER;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing sub cutoff freq\n");
        preamp.incSubCutoffFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing sub cutoff freq\n");
        preamp.decSubCutoffFreq();
      }
      break;

    case Screens::SUB_ATT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving sub attenuation submenu\n");
        active_screen = Screens::SUBWOOFER;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing sub attenuation\n");
        preamp.incSubAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing sub attenuation\n");
        preamp.decSubAttenuation();
      }
      break;

    case Screens::MASTER_VOLUME:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving master volume\n");
        active_screen = Screens::MAIN_SCREEN;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing master volume\n");
        preamp.incVolume();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing master volume\n");
        preamp.decVolume();
      }
      break;

    case Screens::INPUT_GAIN:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
#ifndef POT4
        active_screen = Screens::MAIN_MENU;
#else
        active_screen = Screens::MAIN_SCREEN;
#endif
#ifndef POT4
      else if (type == InputType::ENC_CW)
        preamp.incGain();
      else if (type == InputType::ENC_CCW)
        preamp.decGain();
#else
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
        active_screen = Screens::MASTER_VOLUME;
#endif
      break;

    // Potentiometer popups
    case Screens::TREBLE_GAIN:
    case Screens::MIDDLE_GAIN:
    case Screens::BASS_GAIN:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
        active_screen = Screens::MAIN_SCREEN;
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
        active_screen = Screens::MASTER_VOLUME;
      break;
    ///////////////////////

    case Screens::SCREEN_SAVER:
      // Any input
      if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
        active_screen = Screens::MASTER_VOLUME;
      else
        active_screen = Screens::MAIN_SCREEN;

      display.setPowerSave(false);

      DEBUG("Screen saver: OFF\n");
      break;

    default:
      break;
    }
  }
}

void check_timeouts()
{
  unsigned long long current_stopwatch_time = getStopwatchVal();

  switch (active_screen)
  {
  case Screens::MAIN_SCREEN:
#ifdef ENABLE_SCREEN_SAVER
    if (current_stopwatch_time > SCREEN_SAVER_TIMEOUT_S)
    {
      active_screen = Screens::SCREEN_SAVER;
      display.setPowerSave(true);
      
      restartStopwatch();

      DEBUG("Screen saver: ON\n");
    }
#endif
    break;

  case Screens::SCREEN_SAVER:
    // No timeouts for screen saver
    break;

  // Popups timeout
#ifdef POT4
  case Screens::INPUT_GAIN:
#endif
  case Screens::TREBLE_GAIN:
  case Screens::MIDDLE_GAIN:
  case Screens::BASS_GAIN:
  case Screens::MASTER_VOLUME:
    if (current_stopwatch_time > BACK_TO_MAIN_SCREEN_POPUP_TIMEOUT_S)
    {
      active_screen = Screens::MAIN_SCREEN;
      restartStopwatch();
      DEBUG("Leaving popup to main screen\n");
    }
    break;
  /////////////////
  
  // Default timeout to main screen
  default:
    if (current_stopwatch_time > BACK_TO_MAIN_SCREEN_TIMEOUT_S)
    {
      active_screen = Screens::MAIN_SCREEN;
      restartStopwatch();
      DEBUG("Leaving menu to main screen\n");
    }
    break;
  //////////////////////////////////
  }
}

void handle_display()
{
  // DEBUG("Handling display %d\n", active_screen);

#ifdef ENABLE_PUSH_LENGTH_INDICATOR
  push_indicator();
#endif

  check_timeouts();

  switch (active_screen)
  {
  // Primary screens
  case Screens::MUTE_SCREEN:
    mute_screen();
    break;

  case Screens::MAIN_SCREEN:
    main_screen();
    break;

  case Screens::MAIN_MENU:
    main_menu_selector(main_menu_index);
    break;
  //////////////////

  // Main Menu
  case Screens::LOUDNESS:
    loudness_menu_selector(laudness_menu_index);
    break;

  case Screens::ATTENUATION:
    attenuations_menu_selector(attenuations_menu_index);
    break;

  case Screens::INPUT_GAIN:
    input_gain_settings();
    break;

  case Screens::BASS_FILTER:
    bass_filter_menu_selector(bass_filter_menu_index);
    break;

  case Screens::MIDDLE_FILTER:
    middle_filter_menu_selector(middle_filter_menu_index);
    break;

  case Screens::TREBLE_FILTER:
    treble_filter_menu_selector(treble_filter_menu_index);
    break;

  case Screens::SUBWOOFER:
    sub_menu_selector(sub_menu_index);
    break;
  ////////////

  // Loudness menu
  case Screens::LOUDNESS_ATT:
    loudness_att_settings();
    break;

  case Screens::LOUDNESS_FREQ:
    loudness_center_freq_settings();
    break;

  case Screens::LOUDNESS_HIGH_BOOST:
    loudness_high_boost_settings();
    break;
  ////////////////

  // Attenuation menu
  case Screens::ATTENUATION_LEFT:
    attenuation_left_settings();
    break;

  case Screens::ATTENUATION_RIGHT:
    attenuation_right_setting();
    break;

  case Screens::SUB_ATT:
  case Screens::ATTENUATION_SUB:
    attenuation_sub_setting();
    break;
  ///////////////////

  // Bass menu
  case Screens::BASS_QFACTOR:
    bass_qfactor_settings();
    break;

  case Screens::BASS_FREQ:
    bass_center_freq_settings();
    break;

  case Screens::BASS_DC_MODE:
    bass_dc_settings();
    break;
  ////////////

  // Middle menu
  case Screens::MIDDLE_QFACTOR:
    middle_qfactor_settings();
    break;

  case Screens::MIDDLE_FREQ:
    middle_center_freq_settings();
    break;
  //////////////

  // Treble menu
  case Screens::TREBLE_FREQ:
    treble_center_freq_settings();
    break;
  //////////////

  // Subwoofer menu
  case Screens::SUB_CUTOFF_FREQ:
    sub_cutoff_freq_settings();
    break;
  /////////////////

  // Popups
  case Screens::MASTER_VOLUME:
    master_volume_settings();
    break;

  case Screens::TREBLE_GAIN:
    treble_gain_settings();
    break;

  case Screens::MIDDLE_GAIN:
    middle_gain_settings();
    break;

  case Screens::BASS_GAIN:
    bass_gain_settings();
    break;
  /////////

  case Screens::SCREEN_SAVER:
    // Empty because nothing will be displayed
    break;

  default:
    DEBUG("Invalid screen, setting to main screen\n");
    active_screen = Screens::MAIN_SCREEN;
    main_menu_index = 0;
    break;
  }
}

void display_draw_task(void*)
{
  startStopwatch(1000000);

  while (true)
  {
    display.firstPage();

    do
    {
      handle_display();
    } while (display.nextPage());

    vTaskDelay(pdMS_TO_TICKS(2));
  }

  vTaskDelete(NULL);
}

void auto_saver_task(void*)
{
  while (true)
  {
    preamp.saveChanged();

    vTaskDelay(pdMS_TO_TICKS(SETTING_SAVE_INTERVAL_MS));
  }

  vTaskDelete(NULL);
}

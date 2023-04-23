#include "ui_controller.h"

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <stopWatch.h>

#include "settings.h"
#include "debug.h"
#include "display_handler.h"
#include "screens.h"
#include "global_objects.h"
#include "io_handling/potentiometer_handling.h"
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
volatile int soft_steps_menu_index = 0;

void handle_controll(InputType type)
{
  // DEBUG("Received controll signal %d\n", type);

  if (type == InputType::POT1_CH)
  {
    // DEBUG("POT1 val: %d\n", PotentiometerHandling::values[0]);

    int newGain = (int)map(PotentiometerHandling::values[0], POT_MIN_VAL, POT_MAX_VAL, -15, 15);
    int settingDiff = abs(newGain - Preamp::getTrebleGain());

    if ((settingDiff >= POTENTIOMETER_MIN_DB_CHANGE_FOR_POPUP && active_screen != Screens::TREBLE_GAIN) ||
        (settingDiff > 0 && active_screen == Screens::TREBLE_GAIN))
    {
      // DEBUG("Mapped treble gain: %d\n", newGain);
      restartStopwatch();
      
      active_screen = Screens::TREBLE_GAIN;
      Preamp::setTrebleGain(newGain);
    }
  }
  else if (type == InputType::POT2_CH)
  {
    // DEBUG("POT2 val: %d\n", PotentiometerHandling::values[1]);

    int newGain = (int)map(PotentiometerHandling::values[1], POT_MIN_VAL, POT_MAX_VAL, -15, 15);
    int settingDiff = abs(newGain - Preamp::getMiddleGain());

    if ((settingDiff >= POTENTIOMETER_MIN_DB_CHANGE_FOR_POPUP && active_screen != Screens::MIDDLE_GAIN) ||
        (settingDiff > 0 && active_screen == Screens::MIDDLE_GAIN))
    {
      // DEBUG("Mapped middle gain: %d\n", newGain);
      restartStopwatch();
      
      active_screen = Screens::MIDDLE_GAIN;
      Preamp::setMiddleGain(newGain);
    }
  }
  else if (type == InputType::POT3_CH)
  {
    // DEBUG("POT3 val: %d\n", PotentiometerHandling::values[2]);

    int newGain = (int)map(PotentiometerHandling::values[2], POT_MIN_VAL, POT_MAX_VAL, -15, 15);
    int settingDiff = abs(newGain - Preamp::getBassGain());

    if ((settingDiff >= POTENTIOMETER_MIN_DB_CHANGE_FOR_POPUP && active_screen != Screens::BASS_GAIN) ||
        (settingDiff > 0 && active_screen == Screens::BASS_GAIN))
    {
      // DEBUG("Mapped bass gain: %d\n", newGain);
      restartStopwatch();
      
      active_screen = Screens::BASS_GAIN;
      Preamp::setBassGain(newGain);
    }
  }
  else if (type == InputType::POT4_CH)
  {
    // DEBUG("POT4 val: %d\n", PotentiometerHandling::values[3]);

    int newInputGain = (int)map(PotentiometerHandling::values[3], POT_MIN_VAL, POT_MAX_VAL, 0, 15);
    int settingDiff = abs(newInputGain - Preamp::getInputGain());

    if ((settingDiff >= POTENTIOMETER_MIN_DB_CHANGE_FOR_POPUP && active_screen != Screens::INPUT_GAIN) ||
        (settingDiff > 0 && active_screen == Screens::INPUT_GAIN))
    {
      // DEBUG("Mapped input gain: %d\n", newInputGain);
      restartStopwatch();
      
      active_screen = Screens::INPUT_GAIN;
      Preamp::setInputGain(newInputGain);
    }
  }
  else if (type == InputType::CH_SW_PUSH)
  {
    Preamp::rotateInput();
    DEBUG("Switching input to %s\n", String(channel_names[Preamp::getInput()]).c_str());
  }
  else if (type == InputType::SW_CH1)
  {
    Preamp::setInput(0);
    DEBUG("Switching input to %s\n", String(channel_names[Preamp::getInput()]).c_str());
  }
  else if (type == InputType::SW_CH2)
  {
    Preamp::setInput(1);
    DEBUG("Switching input to %s\n", String(channel_names[Preamp::getInput()]).c_str());
  }
  else if (type == InputType::SW_CH3)
  {
    Preamp::setInput(2);
    DEBUG("Switching input to %s\n", String(channel_names[Preamp::getInput()]).c_str());
  }
  else if (type == InputType::SW_CH4)
  {
    Preamp::setInput(3);
    DEBUG("Switching input to %s\n", String(channel_names[Preamp::getInput()]).c_str());
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
        Preamp::unmute();
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
        Preamp::mute();
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
        else if (active_screen == Screens::SOFT_STEPS)
          soft_steps_menu_index = 0;
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

    case Screens::INPUT_SWITCH:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_CW)
      {
        Preamp::incInput();
      }
      else if (type == InputType::ENC_CCW)
      {
        Preamp::decInput();
      }
      break;

    case Screens::SPECTRUM:
      if (type == InputType::ENC_PUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main screen\n");
        active_screen = Screens::MAIN_SCREEN;
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

    case Screens::SOFT_STEPS:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = soft_steps_menu_screens[soft_steps_menu_index];
        DEBUG("Soft steps menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (soft_steps_menu_index < (number_of_soft_steps_menu_items - 1)) soft_steps_menu_index++;
        else soft_steps_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (soft_steps_menu_index > 0) soft_steps_menu_index--;
        else soft_steps_menu_index = (number_of_soft_steps_menu_items - 1);
      }
      break;

    case Screens::SOFT_MUTE_TIME:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft mute time submenu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing soft mute time\n");
        Preamp::incSoftMuteTime();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing soft mute time\n");
        Preamp::decSoftMuteTime();
      }
      break;

    case Screens::FACT_RESET:
      if (type == InputType::ENC_PUSH)
      {
        DEBUG("Factory reset\n");
        Preamp::resetMemory();
        vTaskDelay(pdMS_TO_TICKS(50));
        ESP.restart();
      }
      else if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
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
        Preamp::incLoudnessAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing loudness attenuation\n");
        Preamp::decLoudnessAttenuation();
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
        Preamp::incLoudnessCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing loudness center freq\n");
        Preamp::decLoudnessCenterFreq();
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
        Preamp::switchLoudnessHighBoost();
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
        Preamp::incLeftAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing attenuation left\n");
        Preamp::decLeftAttenuation();
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
        Preamp::incRightAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing attenuation right\n");
        Preamp::decRightAttenuation();
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
        Preamp::incSubAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing attenuation sub\n");
        Preamp::decSubAttenuation();
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
        Preamp::incBassQFactor();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing bass q factor\n");
        Preamp::decBassQFactor();
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
        Preamp::incBassCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing bass center freq\n");
        Preamp::decBassCenterFreq();
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
        Preamp::switchBassDC();
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
        Preamp::incMiddleQFactor();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing middle q factor\n");
        Preamp::decMiddleQFactor();
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
        Preamp::incMiddleCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing middle center freq\n");
        Preamp::decMiddleCenterFreq();
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
        Preamp::incTrebleCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing treble center freq\n");
        Preamp::decTrebleCenterFreq();
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
        Preamp::incSubCutoffFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing sub cutoff freq\n");
        Preamp::decSubCutoffFreq();
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
        Preamp::incSubAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing sub attenuation\n");
        Preamp::decSubAttenuation();
      }
      break;

    case Screens::SOFT_STEPS_TIME:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft steps time submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW)
      {
        // DEBUG("Increasing soft steps time\n");
        Preamp::incSoftStepsTime();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing soft steps time\n");
        Preamp::decSoftStepsTime();
      }
      break;

    case Screens::SOFT_STEPS_VOLUME:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft steps volume submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching soft steps volume\n");
        Preamp::switchVolumeSoftStep();
      }
      break;

    case Screens::SOFT_STEPS_LOUDNESS:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft steps loudness submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching soft steps loudness\n");
        Preamp::switchLoudnessSoftStep();
      }
      break;

    case Screens::SOFT_STEPS_MIDDLE:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft steps middle submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching soft steps middle\n");
        Preamp::switchMiddleSoftStep();
      }
      break;

    case Screens::SOFT_STEPS_BASS:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft steps bass submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching soft steps bass\n");
        Preamp::switchBassSoftStep();
      }
      break;

    case Screens::SOFT_STEPS_LEFT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft steps left att submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching soft steps left att\n");
        Preamp::switchLeftSoftSteps();
      }
      break;

    case Screens::SOFT_STEPS_RIGHT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft steps right att submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching soft steps right att\n");
        Preamp::switchRightSoftSteps();
      }
      break;

    case Screens::SOFT_STEPS_SUB:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        // DEBUG("Leaving soft steps sub att submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        // DEBUG("Switching soft steps sub att\n");
        Preamp::switchSubSoftSteps();
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
        Preamp::incVolume();
      }
      else if (type == InputType::ENC_CCW)
      {
        // DEBUG("Decreasing master volume\n");
        Preamp::decVolume();
      }
      break;

    // Potentiometer popups
    case Screens::INPUT_GAIN:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
#ifdef INPUT_GAIN_POTENTIOMETER
        active_screen = Screens::MAIN_SCREEN;
#else
        active_screen = Screens::MAIN_MENU;
#endif
#ifndef INPUT_GAIN_POTENTIOMETER
      else if (type == InputType::ENC_CW)
        Preamp::incInputGain();
      else if (type == InputType::ENC_CCW)
        Preamp::decInputGain();
#else
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
        active_screen = Screens::MASTER_VOLUME;
#endif
      break;

    case Screens::TREBLE_GAIN:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
#ifdef TREBLE_GAIN_POTENTIOMETER
        active_screen = Screens::MAIN_SCREEN;
#else
        active_screen = Screens::MAIN_MENU;
#endif
#ifndef TREBLE_GAIN_POTENTIOMETER
      else if (type == InputType::ENC_CW)
        Preamp::incTrebleGain();
      else if (type == InputType::ENC_CCW)
        Preamp::decTrebleGain();
#else
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
        active_screen = Screens::MASTER_VOLUME;
#endif
      break;

    case Screens::MIDDLE_GAIN:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
#ifdef MIDDLE_GAIN_POTENTIOMETER
        active_screen = Screens::MAIN_SCREEN;
#else
        active_screen = Screens::MAIN_MENU;
#endif
#ifndef MIDDLE_GAIN_POTENTIOMETER
      else if (type == InputType::ENC_CW)
        Preamp::incMiddleGain();
      else if (type == InputType::ENC_CCW)
        Preamp::decMiddleGain();
#else
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
        active_screen = Screens::MASTER_VOLUME;
#endif
      break;

    case Screens::BASS_GAIN:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
#ifdef BASS_GAIN_POTENTIOMETER
        active_screen = Screens::MAIN_SCREEN;
#else
        active_screen = Screens::MAIN_MENU;
#endif
#ifndef BASS_GAIN_POTENTIOMETER
      else if (type == InputType::ENC_CW)
        Preamp::incBassGain();
      else if (type == InputType::ENC_CCW)
        Preamp::decBassGain();
#else
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
        active_screen = Screens::MASTER_VOLUME;
#endif
      break;
    ///////////////////////

    case Screens::SCREEN_SAVER:
      // Any input
      if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
        active_screen = Screens::MASTER_VOLUME;
      else
        active_screen = Screens::MAIN_SCREEN;

#ifndef SPECTRUM_AS_SCREEN_SAVER
      display.setPowerSave(false);
#endif

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

#ifndef SPECTRUM_AS_SCREEN_SAVER
      display.setPowerSave(true);
#endif
      
      restartStopwatch();

      DEBUG("Screen saver: ON\n");
    }
#endif
    break;

  case Screens::SPECTRUM:
  case Screens::SCREEN_SAVER:
    // No timeouts for screen saver
    break;

  // Popups timeout
#ifdef INPUT_GAIN_POTENTIOMETER
  case Screens::INPUT_GAIN:
#endif
#ifdef TREBLE_GAIN_POTENTIOMETER
  case Screens::TREBLE_GAIN:
#endif
#ifdef MIDDLE_GAIN_POTENTIOMETER
  case Screens::MIDDLE_GAIN:
#endif
#ifdef BASS_GAIN_POTENTIOMETER
  case Screens::BASS_GAIN:
#endif
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

#ifdef ENABLE_ENCODER_PUSH_LENGTH_INDICATOR
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
  case Screens::INPUT_SWITCH:
    input_selector();
    break;

  case Screens::SPECTRUM:
    spectrum();
    break;

  case Screens::LOUDNESS:
    loudness_menu_selector(laudness_menu_index);
    break;

  case Screens::ATTENUATION:
    attenuations_menu_selector(attenuations_menu_index);
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

  case Screens::SOFT_STEPS:
    soft_steps_menu_selector(soft_steps_menu_index);
    break;

  case Screens::SOFT_MUTE_TIME:
    soft_mute_time_settings();
    break;

  case Screens::FACT_RESET:
    factory_reset_configmation();
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

  // Soft steps menu
  case Screens::SOFT_STEPS_TIME:
    soft_steps_time_settings();
    break;

  case Screens::SOFT_STEPS_VOLUME:
    soft_steps_volume_settings();
    break;

  case Screens::SOFT_STEPS_LOUDNESS:
    soft_steps_loudness_settings();
    break;

  case Screens::SOFT_STEPS_MIDDLE:
    soft_steps_middle_settings();
    break;

  case Screens::SOFT_STEPS_BASS:
    soft_steps_bass_settings();
    break;

  case Screens::SOFT_STEPS_LEFT:
    soft_steps_left_att_settings();
    break;

  case Screens::SOFT_STEPS_RIGHT:
    soft_steps_right_att_settings();
    break;

  case Screens::SOFT_STEPS_SUB:
    soft_steps_sub_att_settings();
    break;
  //////////////////

  // Popups
  case Screens::MASTER_VOLUME:
    master_volume_settings();
    break;

  case Screens::INPUT_GAIN:
    input_gain_settings();
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
#ifdef SPECTRUM_AS_SCREEN_SAVER
    spectrum();
#endif
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

    vTaskDelay(pdMS_TO_TICKS(DISPLAY_UPDATE_INTERVAL_MS));
  }

  vTaskDelete(NULL);
}

void auto_saver_task(void*)
{
  while (true)
  {
    Preamp::saveChanged();

    vTaskDelay(pdMS_TO_TICKS(SETTING_SAVE_INTERVAL_MS));
  }

  vTaskDelete(NULL);
}

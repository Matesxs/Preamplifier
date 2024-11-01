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
#include "io_handling/temperature_reader.h"
#include "ledstrip/led_strip_controller.h"
#include "app_settings_store.h"

bool hightTempWarning = false;
volatile Screens active_screen = Screens::MAIN_SCREEN;

volatile int main_menu_index = 0;
volatile int laudness_menu_index = 0;
volatile int attenuations_menu_index = 0;
volatile int bass_filter_menu_index = 0;
volatile int middle_filter_menu_index = 0;
volatile int treble_filter_menu_index = 0;
volatile int sub_menu_index = 0;
volatile int soft_steps_menu_index = 0;
volatile int led_strip_menu_index = 0;
volatile int led_strip_effect_menu_index = 0;
volatile int led_strip_color_menu_index = 0;
volatile int settings_menu_index = 0;
volatile int settings_screeensaver_menu_index = 0;
volatile int settings_overheat_menu_index = 0;

SemaphoreHandle_t handleControlSemaphore = xSemaphoreCreateMutex();

void handle_controll(InputType type)
{
  xSemaphoreTake(handleControlSemaphore, portMAX_DELAY);

  // DEBUG("Received controll signal %d\n", type);

  if (type == InputType::POT1_CH)
  {
    // DEBUG("POT1 val: %d\n", PotentiometerHandling::values[0]);

    restartStopwatch();
      
    active_screen = Screens::TREBLE_GAIN;
    Preamp::setTrebleGain(PotentiometerHandling::values[0]);
  }
  else if (type == InputType::POT2_CH)
  {
    // DEBUG("POT2 val: %d\n", PotentiometerHandling::values[1]);

    restartStopwatch();
      
    active_screen = Screens::MIDDLE_GAIN;
    Preamp::setMiddleGain(PotentiometerHandling::values[1]);
  }
  else if (type == InputType::POT3_CH)
  {
    // DEBUG("POT3 val: %d\n", PotentiometerHandling::values[2]);

    restartStopwatch();
      
    active_screen = Screens::BASS_GAIN;
    Preamp::setBassGain(PotentiometerHandling::values[2]);
  }
  else if (type == InputType::POT4_CH)
  {
    // DEBUG("POT4 val: %d\n", PotentiometerHandling::values[3]);

    restartStopwatch();
      
    active_screen = Screens::INPUT_GAIN;
    Preamp::setInputGain(PotentiometerHandling::values[3]);
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
        else if (active_screen == Screens::LED_STRIP)
          led_strip_menu_index = 0;
        else if (active_screen == Screens::SETTINGS)
          settings_menu_index = 0;
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
        DEBUG("Leaving soft mute time submenu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing soft mute time\n");
        Preamp::incSoftMuteTime();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing soft mute time\n");
        Preamp::decSoftMuteTime();
      }
      break;

    case Screens::LED_STRIP:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = led_strip_menu_screens[led_strip_menu_index];
        DEBUG("Led strip menu selected submenu: %d\n", active_screen);

        if (active_screen == Screens::LED_STRIP_EFFECT)
          led_strip_effect_menu_index = static_cast<int>(LedStrip::getEffectIdx());
      }
      else if (type == InputType::ENC_CW)
      {
        if (led_strip_menu_index < (number_of_led_strip_menu_items - 1)) led_strip_menu_index++;
        else led_strip_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (led_strip_menu_index > 0) led_strip_menu_index--;
        else led_strip_menu_index = (number_of_led_strip_menu_items - 1);
      }
      break;

    case Screens::SETTINGS:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to main menu\n");
        active_screen = Screens::MAIN_MENU;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = settings_menu_screens[settings_menu_index];
        DEBUG("Settings menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (settings_menu_index < (number_of_settings_menu_items - 1)) settings_menu_index++;
        else settings_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (settings_menu_index > 0) settings_menu_index--;
        else settings_menu_index = (number_of_settings_menu_items - 1);
      }
      break;

    case Screens::LOUDNESS_ATT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving loudness att submenu\n");
        active_screen = Screens::LOUDNESS;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing loudness attenuation\n");
        Preamp::incLoudnessAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing loudness attenuation\n");
        Preamp::decLoudnessAttenuation();
      }
      break;

    case Screens::LOUDNESS_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving loudness center freq submenu\n");
        active_screen = Screens::LOUDNESS;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing loudness center freq\n");
        Preamp::incLoudnessCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing loudness center freq\n");
        Preamp::decLoudnessCenterFreq();
      }
      break;

    case Screens::LOUDNESS_HIGH_BOOST:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving loudness high boost submenu\n");
        active_screen = Screens::LOUDNESS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching loudness high boost\n");
        Preamp::switchLoudnessHighBoost();
      }
      break;

    case Screens::ATTENUATION_LEFT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving attenuation left submenu\n");
        active_screen = Screens::ATTENUATION;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing attenuation left\n");
        Preamp::incLeftAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing attenuation left\n");
        Preamp::decLeftAttenuation();
      }
      break;

    case Screens::ATTENUATION_RIGHT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving attenuation right submenu\n");
        active_screen = Screens::ATTENUATION;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing attenuation right\n");
        Preamp::incRightAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing attenuation right\n");
        Preamp::decRightAttenuation();
      }
      break;

    case Screens::ATTENUATION_SUB:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving attenuation sub submenu\n");
        active_screen = Screens::ATTENUATION;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing attenuation sub\n");
        Preamp::incSubAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing attenuation sub\n");
        Preamp::decSubAttenuation();
      }
      break;

    case Screens::BASS_QFACTOR:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving bass q factor submenu\n");
        active_screen = Screens::BASS_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing bass q factor\n");
        Preamp::incBassQFactor();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing bass q factor\n");
        Preamp::decBassQFactor();
      }
      break;

    case Screens::BASS_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving bass center freq submenu\n");
        active_screen = Screens::BASS_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing bass center freq\n");
        Preamp::incBassCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing bass center freq\n");
        Preamp::decBassCenterFreq();
      }
      break;

    case Screens::BASS_DC_MODE:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving bass dc submenu\n");
        active_screen = Screens::BASS_FILTER;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching bass dc\n");
        Preamp::switchBassDC();
      }
      break;

    case Screens::MIDDLE_QFACTOR:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving middle q factor submenu\n");
        active_screen = Screens::MIDDLE_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing middle q factor\n");
        Preamp::incMiddleQFactor();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing middle q factor\n");
        Preamp::decMiddleQFactor();
      }
      break;

    case Screens::MIDDLE_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving middle center freq submenu\n");
        active_screen = Screens::MIDDLE_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing middle center freq\n");
        Preamp::incMiddleCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing middle center freq\n");
        Preamp::decMiddleCenterFreq();
      }
      break;

    case Screens::TREBLE_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving treble center freq submenu\n");
        active_screen = Screens::TREBLE_FILTER;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing treble center freq\n");
        Preamp::incTrebleCenterFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing treble center freq\n");
        Preamp::decTrebleCenterFreq();
      }
      break;

    case Screens::SUB_CUTOFF_FREQ:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving sub cutoff freq submenu\n");
        active_screen = Screens::SUBWOOFER;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing sub cutoff freq\n");
        Preamp::incSubCutoffFreq();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing sub cutoff freq\n");
        Preamp::decSubCutoffFreq();
      }
      break;

    case Screens::SUB_ATT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving sub attenuation submenu\n");
        active_screen = Screens::SUBWOOFER;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing sub attenuation\n");
        Preamp::incSubAttenuation();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing sub attenuation\n");
        Preamp::decSubAttenuation();
      }
      break;

    case Screens::SOFT_STEPS_TIME:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving soft steps time submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing soft steps time\n");
        Preamp::incSoftStepsTime();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing soft steps time\n");
        Preamp::decSoftStepsTime();
      }
      break;

    case Screens::SOFT_STEPS_VOLUME:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving soft steps volume submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching soft steps volume\n");
        Preamp::switchVolumeSoftStep();
      }
      break;

    case Screens::SOFT_STEPS_LOUDNESS:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving soft steps loudness submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching soft steps loudness\n");
        Preamp::switchLoudnessSoftStep();
      }
      break;

    case Screens::SOFT_STEPS_MIDDLE:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving soft steps middle submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching soft steps middle\n");
        Preamp::switchMiddleSoftStep();
      }
      break;

    case Screens::SOFT_STEPS_BASS:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving soft steps bass submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching soft steps bass\n");
        Preamp::switchBassSoftStep();
      }
      break;

    case Screens::SOFT_STEPS_LEFT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving soft steps left att submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching soft steps left att\n");
        Preamp::switchLeftSoftSteps();
      }
      break;

    case Screens::SOFT_STEPS_RIGHT:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving soft steps right att submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching soft steps right att\n");
        Preamp::switchRightSoftSteps();
      }
      break;

    case Screens::SOFT_STEPS_SUB:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving soft steps sub att submenu\n");
        active_screen = Screens::SOFT_STEPS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        DEBUG("Switching soft steps sub att\n");
        Preamp::switchSubSoftSteps();
      }
      break;

    case Screens::LED_STRIP_EFFECT:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving led strip effects submenu\n");
        active_screen = Screens::LED_STRIP;
        led_strip_effect_menu_index = 0;
      }
      else if (type == InputType::ENC_PUSH)
      {
        DEBUG("Led strip setting effect %d\n", led_strip_effect_menu_index);
        LedStrip::setEffectIdx(led_strip_effect_menu_index);
        LedStrip::applyEffect();
      }
      else if (type == InputType::ENC_CW)
      {
        if (led_strip_effect_menu_index < (number_of_led_strip_effect_menu_items - 1)) led_strip_effect_menu_index++;
        else led_strip_effect_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (led_strip_effect_menu_index > 0) led_strip_effect_menu_index--;
        else led_strip_effect_menu_index = (number_of_led_strip_effect_menu_items - 1);
      }
      break;

    case Screens::LED_STRIP_COLOR:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving led color submenu\n");
        active_screen = Screens::LED_STRIP;
        led_strip_color_menu_index = 0;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = led_strip_color_menu_screens[led_strip_color_menu_index];
        DEBUG("Led strip color menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (led_strip_color_menu_index < (number_of_led_strip_color_menu_items - 1)) led_strip_color_menu_index++;
        else led_strip_color_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (led_strip_color_menu_index > 0) led_strip_color_menu_index--;
        else led_strip_color_menu_index = (number_of_led_strip_color_menu_items - 1);
      }
      break;

    case Screens::LED_STRIP_COLOR_RED:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving led color red settings\n");
        active_screen = Screens::LED_STRIP_COLOR;
        LedStrip::applyEffect();
      }
      else if (type == InputType::ENC_CW)
      {
        RgbColor color = LedStrip::getColor();
        if (color.R < 255)
        {
          color.R++;
          LedStrip::setColor(color);
        }
      }
      else if (type == InputType::ENC_CCW)
      {
        RgbColor color = LedStrip::getColor();
        if (color.R > 0)
        {
          color.R--;
          LedStrip::setColor(color);
        }
      }
      break;

    case Screens::LED_STRIP_COLOR_GREEN:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving led color green settings\n");
        active_screen = Screens::LED_STRIP_COLOR;
        LedStrip::applyEffect();
      }
      else if (type == InputType::ENC_CW)
      {
        RgbColor color = LedStrip::getColor();
        if (color.G < 255)
        {
          color.G++;
          LedStrip::setColor(color);
        }
      }
      else if (type == InputType::ENC_CCW)
      {
        RgbColor color = LedStrip::getColor();
        if (color.G > 0)
        {
          color.G--;
          LedStrip::setColor(color);
        }
      }
      break;

    case Screens::LED_STRIP_COLOR_BLUE:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving led color blue settings\n");
        active_screen = Screens::LED_STRIP_COLOR;
        LedStrip::applyEffect();
      }
      else if (type == InputType::ENC_CW)
      {
        RgbColor color = LedStrip::getColor();
        if (color.B < 255)
        {
          color.B++;
          LedStrip::setColor(color);
        }
      }
      else if (type == InputType::ENC_CCW)
      {
        RgbColor color = LedStrip::getColor();
        if (color.B > 0)
        {
          color.B--;
          LedStrip::setColor(color);
        }
      }
      break;

    case Screens::LED_STRIP_COLOR_BRIGHTNESS:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving led color brightness settings\n");
        active_screen = Screens::LED_STRIP_COLOR;
        LedStrip::applyEffect();
      }
      else if (type == InputType::ENC_CW)
      {
        LedStrip::incBrightness();
      }
      else if (type == InputType::ENC_CCW)
      {
        LedStrip::decBrightness();
      }
      break;

    case Screens::SETTINGS_SCREENSAVER:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving screensaver submenu\n");
        active_screen = Screens::SETTINGS;
        settings_screeensaver_menu_index = 0;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = settings_screensaver_menu_screens[settings_screeensaver_menu_index];
        DEBUG("Screensaver menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (settings_screeensaver_menu_index < (number_of_settings_screensaver_menu_items - 1)) settings_screeensaver_menu_index++;
        else settings_screeensaver_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (settings_screeensaver_menu_index > 0) settings_screeensaver_menu_index--;
        else settings_screeensaver_menu_index = (number_of_settings_screensaver_menu_items - 1);
      }
      break;

    case Screens::SETTINGS_SCREENSAVER_ENABLE:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving screensaver enable submenu\n");
        active_screen = Screens::SETTINGS_SCREENSAVER;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        AppSettingsStore::toggleScreenSaverEnabled();
      }
      break;

    case Screens::SETTINGS_SCREENSAVER_DELAY:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving screensaver delay submenu\n");
        active_screen = Screens::SETTINGS_SCREENSAVER;
      }
      else if (type == InputType::ENC_CW)
      {
        if (AppSettingsStore::getScreensaverDelay() < 3600)
          AppSettingsStore::setScreensaverDelay(AppSettingsStore::getScreensaverDelay() + 30);
      }
      else if (type == InputType::ENC_CCW)
      {
        if (AppSettingsStore::getScreensaverDelay() > 30)
          AppSettingsStore::setScreensaverDelay(AppSettingsStore::getScreensaverDelay() - 30);
      }
      break;

    case Screens::SETTINGS_SCREENSAVER_SPECTRUM:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving screensaver use spectrum submenu\n");
        active_screen = Screens::SETTINGS_SCREENSAVER;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        AppSettingsStore::toggleScreensaverSpectrum();
      }
      break;

    case Screens::SETTINGS_BRIGHTNESS:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to settings\n");
        active_screen = Screens::SETTINGS;
      }
      else if (type == InputType::ENC_CW)
      {
        AppSettingsStore::incDisplayBrightness();
        display.setContrast(AppSettingsStore::getDisplayBrightness());
      }
      else if (type == InputType::ENC_CCW)
      {
        AppSettingsStore::decDisplayBrightness();
        display.setContrast(AppSettingsStore::getDisplayBrightness());
      }
      break;

    case Screens::SETTINGS_FACT_RESET:
      if (type == InputType::ENC_PUSH)
      {
        DEBUG("Factory reset\n");
        Preamp::resetMemory();
        LedStrip::factoryReset();
        AppSettingsStore::reset();
        vTaskDelay(pdMS_TO_TICKS(50));
        ESP.restart();
      }
      else if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Returning to settings\n");
        active_screen = Screens::SETTINGS;
      }
      break;

    case Screens::SETTINGS_CLIPPING_DETECTION:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving clip detection submenu\n");
        active_screen = Screens::SETTINGS;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        AppSettingsStore::toggleClipDetection();
      }
      break;

    case Screens::SETTINGS_OVERHEAT:
      if (type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving overheat submenu\n");
        active_screen = Screens::SETTINGS;
        settings_overheat_menu_index = 0;
      }
      else if (type == InputType::ENC_PUSH)
      {
        active_screen = settings_overheat_menu_screens[settings_overheat_menu_index];
        DEBUG("Overheat menu selected submenu: %d\n", active_screen);
      }
      else if (type == InputType::ENC_CW)
      {
        if (settings_overheat_menu_index < (number_of_settings_overheat_menu_items - 1)) settings_overheat_menu_index++;
        else settings_overheat_menu_index = 0;
      }
      else if (type == InputType::ENC_CCW)
      {
        if (settings_overheat_menu_index > 0) settings_overheat_menu_index--;
        else settings_overheat_menu_index = (number_of_settings_overheat_menu_items - 1);
      }
      break;

    case Screens::SETTINGS_OVERHEAT_ENABLE:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving overheat enable submenu\n");
        active_screen = Screens::SETTINGS_OVERHEAT;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        AppSettingsStore::toggleOverheatDetection();
        hightTempWarning = false;
        LedStrip::disableAlarm();
      }
      break;

    case Screens::SETTINGS_OVERHEAT_MUTE:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving overheat mute submenu\n");
        active_screen = Screens::SETTINGS_OVERHEAT;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        AppSettingsStore::toggleOverheatMute();
        hightTempWarning = false;
      }
      break;

    case Screens::SETTINGS_OVERHEAT_BLINK:
      if (type == InputType::ENC_LPUSH || type == InputType::ENC_PUSH)
      {
        DEBUG("Leaving overheat blink lights submenu\n");
        active_screen = Screens::SETTINGS_OVERHEAT;
      }
      else if (type == InputType::ENC_CW || type == InputType::ENC_CCW)
      {
        AppSettingsStore::toggleOverheatFlashLights();
        LedStrip::disableAlarm();
        hightTempWarning = false;
      }
      break;

    case Screens::MASTER_VOLUME:
      if (type == InputType::ENC_PUSH || type == InputType::ENC_LPUSH)
      {
        DEBUG("Leaving master volume\n");
        active_screen = Screens::MAIN_SCREEN;
      }
      else if (type == InputType::ENC_CW)
      {
        DEBUG("Increasing master volume\n");
        Preamp::incVolume();
      }
      else if (type == InputType::ENC_CCW)
      {
        DEBUG("Decreasing master volume\n");
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

      if (!AppSettingsStore::getScreensaverSpectrum())
        display.setPowerSave(false);

      DEBUG("Screen saver: OFF\n");
      break;

    default:
      break;
    }
  }

  xSemaphoreGive(handleControlSemaphore);
}

void check_timeouts()
{
  unsigned long long current_stopwatch_time = getStopwatchVal();

  switch (active_screen)
  {
  case Screens::MAIN_SCREEN:
    if (AppSettingsStore::getScreensaverEnabled())
    {
      if (current_stopwatch_time > AppSettingsStore::getScreensaverDelay())
      {
        active_screen = Screens::SCREEN_SAVER;

        if (!AppSettingsStore::getScreensaverSpectrum())
          display.setPowerSave(true);
        
        restartStopwatch();

        DEBUG("Screen saver: ON\n");
      }
    }
    break;

  case Screens::MUTE_SCREEN:
  case Screens::SPECTRUM:
  case Screens::SCREEN_SAVER:
    // No timeouts for screen saver, mute screen and spectrum
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

  case Screens::LED_STRIP:
    led_strip_menu_selector(led_strip_menu_index);
    break;

  case Screens::SETTINGS:
    settings_menu_selector(settings_menu_index);
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

  // LED Strip menu
  case Screens::LED_STRIP_EFFECT:
    led_strip_effect_menu_selector(led_strip_effect_menu_index);
    break;

  case Screens::LED_STRIP_COLOR:
    led_strip_color_menu_selector(led_strip_color_menu_index);
    break;

  case Screens::LED_STRIP_COLOR_RED:
    led_strip_color_red_settings();
    break;

  case Screens::LED_STRIP_COLOR_GREEN:
    led_strip_color_green_settings();
    break;

  case Screens::LED_STRIP_COLOR_BLUE:
    led_strip_color_blue_settings();
    break;

  case Screens::LED_STRIP_COLOR_BRIGHTNESS:
    led_strip_color_brightness_settings();
    break;
  /////////////////

  // Settings menu
  case Screens::SETTINGS_SCREENSAVER:
    settings_screensaver_menu_selector(settings_screeensaver_menu_index);
    break;

  case Screens::SETTINGS_CLIPPING_DETECTION:
    settings_clip_detection();
    break;

  case Screens::SETTINGS_OVERHEAT:
    settings_overheat_menu_selector(settings_overheat_menu_index);
    break;

  case Screens::SETTINGS_BRIGHTNESS:
    settings_brightness();
    break;

  case Screens::SETTINGS_FACT_RESET:
    factory_reset_confirmation();
    break;
  ////////////////

  // Settings screensaver submenu
  case Screens::SETTINGS_SCREENSAVER_ENABLE:
    settings_screensaver_enable();
    break;

  case Screens::SETTINGS_SCREENSAVER_DELAY:
    settings_screensaver_delay();
    break;

  case Screens::SETTINGS_SCREENSAVER_SPECTRUM:
    settings_screensaver_use_spectrum();
    break;
  ///////////////////////////////

  // Settings overheat submenu
  case Screens::SETTINGS_OVERHEAT_ENABLE:
    settings_overheat_detection_enable();
    break;

  case Screens::SETTINGS_OVERHEAT_MUTE:
    settings_overheat_mute();
    break;

  case Screens::SETTINGS_OVERHEAT_BLINK:
    settings_overheat_blink();
    break;
  ////////////////////////////

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
    if (AppSettingsStore::getScreensaverSpectrum())
      spectrum();
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
#ifdef ENABLE_TEMPERATURE_MONITORING
    if (AppSettingsStore::getOverheatDetection() && active_screen != Screens::SETTINGS_OVERHEAT && active_screen != Screens::SETTINGS_OVERHEAT_BLINK && active_screen != Screens::SETTINGS_OVERHEAT_ENABLE && active_screen != Screens::SETTINGS_OVERHEAT_MUTE)
    {
      if (TemperatureReader::maxTemp() >= OVERHEAT_TEMPERATURE)
      {
        if (!hightTempWarning)
        {
          if (AppSettingsStore::getOverheatMute())
          {
            if (active_screen != Screens::MUTE_SCREEN)
            {
              active_screen = Screens::MUTE_SCREEN;
              Preamp::mute();
            }
          }

          if (AppSettingsStore::getOverheatFlashLights())
            LedStrip::setAlarm();

          DEBUG("Too high temperature\n");
          hightTempWarning = true;
        }
      }
      else
      {
        if (hightTempWarning)
        {
          LedStrip::disableAlarm();

          DEBUG("High temperature cleared\n");
          hightTempWarning = false;
        }
      }
    }
#endif

    display.firstPage();

    do
    {
      handle_display();
    } while (display.nextPage());

    vTaskDelay(pdMS_TO_TICKS(DISPLAY_UPDATE_INTERVAL_MS));
  }

  vTaskDelete(NULL);
}

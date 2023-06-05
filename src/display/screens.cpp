#include "screens.h"

#include <Arduino.h>

#include "global_objects.h"
#include "preamp.h"
#include "io_handling/input_handlers.h"
#include "io_handling/temperature_reader.h"
#include "display_handler.h"
#include "settings.h"
#include "io_handling/spectrum_analyzer.h"
#include "ledstrip/led_strip_controller.h"
#include "debug.h"
#include "interpolator.h"
#include "app_settings_store.h"

Interpolator<uint16_t, SA_BANDS_NUMBER, SPECTRUM_DISPLAY_BANDS> rawInterpolatedSpectrum;
Interpolator<uint16_t, SA_BANDS_NUMBER, SPECTRUM_DISPLAY_BANDS> avgInterpolatedSpectrum;

bool overtemperature = false;

void push_indicator()
{
  uint16_t screenCenter = display.getDisplayWidth() / 2;

  if (InputHandling::longPush)
    display.drawLine(screenCenter - ENCODER_LONG_PUSH_INDICATOR_LENGTH, display.getDisplayHeight() - 1, screenCenter + ENCODER_LONG_PUSH_INDICATOR_LENGTH, display.getDisplayHeight() - 1);
  else if (InputHandling::shortPush)
    display.drawLine(screenCenter - ENCODER_SHORT_PUSH_INDICATOR_LENGTH, display.getDisplayHeight() - 1, screenCenter + ENCODER_SHORT_PUSH_INDICATOR_LENGTH, display.getDisplayHeight() - 1);
}

void spectrum()
{
  static const uint16_t freqWidth = display.getDisplayWidth() / SPECTRUM_DISPLAY_BANDS;

  rawInterpolatedSpectrum.SetData(SpectrumAnalyzer::spectrumRaw);
  avgInterpolatedSpectrum.SetData(SpectrumAnalyzer::spectrum);

  rawInterpolatedSpectrum.Interpolate();
  avgInterpolatedSpectrum.Interpolate();

  for (int i = 0; i < SPECTRUM_DISPLAY_BANDS; i++)
  {
    uint16_t avgHeight = max((uint16_t)max((uint16_t)0, min((uint16_t)map(avgInterpolatedSpectrum[i], 0, SPECTRUM_MAX_VAL, 0, display.getDisplayHeight() - 1), (uint16_t)(display.getDisplayHeight() - 1))), (uint16_t)1);
    uint16_t rawHeight = max((uint16_t)0, min((uint16_t)map(rawInterpolatedSpectrum[i], 0, SPECTRUM_MAX_VAL, 0, display.getDisplayHeight() - 1), (uint16_t)(display.getDisplayHeight() - 1)));

    display.drawFrame(i * freqWidth, display.getDisplayHeight() - avgHeight, freqWidth, avgHeight);

    if (rawHeight > 0)
      display.drawBox(i * freqWidth, display.getDisplayHeight() - rawHeight, freqWidth, rawHeight);
  }
}

void main_screen()
{
  display.setFont(u8g2_font_ncenB14_tr);
  if (number_of_channels > 1)
    display_draw_center(String(String("INPUT ") + String(channel_names[Preamp::getInput()])).c_str(), 0);
  
  String volumeString = String(String("VOL: ") + String(Preamp::getVolume()));
  String gainString = String(String("GAIN: ") + String(Preamp::getInputGain()));

  uint16_t bottomRowYOffset = display.getDisplayHeight() - display.getMaxCharHeight();

  display.setFont(u8g2_font_ncenB12_tr);
  display.drawStr(15, bottomRowYOffset, volumeString.c_str());
  display.drawStr(display.getDisplayWidth() - display.getStrWidth(gainString.c_str()) - 15, bottomRowYOffset, gainString.c_str());

  display.setFont(u8g2_font_ncenB10_tr);

#ifdef ENABLE_TEMPERATURE_MONITORING
#ifndef ONLY_MAX_TEMPERATURE
  uint8_t sensor_count = TemperatureReader::getSensorCount();
  if (sensor_count >= 1)
  {
    String tempStr = String(String(TemperatureReader::temperatures[0], 1) + String("C"));
    display.drawStr(5, 0, tempStr.c_str());
  }
  if (sensor_count >= 2)
  {
    String tempStr = String(String(TemperatureReader::temperatures[1], 1) + String("C"));
    display.drawStr(display.getDisplayWidth() - (display.getStrWidth(tempStr.c_str()) + 5), 0, tempStr.c_str());
  }
#else
  String tempStr = String(String(TemperatureReader::maxTemp(), 1) + String("C"));
  display.drawStr(0, 0, tempStr.c_str());
#endif
#endif

  if (AppSettingsStore::getOverheatDetection() && TemperatureReader::maxTemp() >= OVERHEAT_TEMPERATURE)
  {
    display.setDrawColor(2);
    display.setFont(u8g2_font_ncenB18_tr);
    display_draw_center("High temperature!");
    display.setDrawColor(1);
  }
  else if (AppSettingsStore::getClipDetection())
  {
    for (auto& val : SpectrumAnalyzer::spectrumRaw)
    {
      if (val >= SPECTRUM_MAX_VAL)
      {
        display.setDrawColor(2);
        display.setFont(u8g2_font_ncenB18_tr);
        display_draw_center("Clipping");
        display.setDrawColor(1);
        break;
      }
    }
  }
}

void mute_screen()
{
  display.setFont(u8g2_font_ncenB18_tr);
#ifdef ENABLE_TEMPERATURE_MONITORING
  if (AppSettingsStore::getOverheatDetection() && TemperatureReader::maxTemp() >= OVERHEAT_TEMPERATURE)
  {
    display.setDrawColor(2);
    display.setFont(u8g2_font_ncenB18_tr);
    display_draw_center("High temperature!");
    display.setDrawColor(1);
  }
  else
  {
    display_draw_center("MUTED");
  }
#else
  display_draw_center("MUTED");
#endif

  display.setFont(u8g2_font_ncenB10_tr);

#ifdef ENABLE_TEMPERATURE_MONITORING
  String temp1Str = String(String(TemperatureReader::temperatures[0], 1) + String("C"));
  String temp2Str = String(String(TemperatureReader::temperatures[1], 1) + String("C"));
  display.drawStr(0, 0, temp1Str.c_str());
  display.drawStr(display.getDisplayWidth() - display.getStrWidth(temp2Str.c_str()), 0, temp2Str.c_str());
#endif
}

void main_menu_selector(int index)
{
  draw_menu("Menu", main_menu_names[index].c_str());
}

void input_gain_settings()
{
  draw_amount_selector("Input Gain", Preamp::getInputGain(), 0, 15);
}

void master_volume_settings()
{
  draw_amount_selector("Volume", Preamp::getVolume(), -80, 15);
}

void treble_gain_settings()
{
  draw_amount_selector("Treble Gain", Preamp::getTrebleGain(), -15, 15);
}

void middle_gain_settings()
{
  draw_amount_selector("Middle Gain", Preamp::getMiddleGain(), -15, 15);
}

void bass_gain_settings()
{
  draw_amount_selector("Bass Gain", Preamp::getBassGain(), -15, 15);
}

void input_selector()
{
  draw_centered_desc_and_val("Input", String(channel_names[Preamp::getInput()]).c_str());
}

void loudness_menu_selector(int index)
{
  draw_menu("Loudness", loudness_menu_names[index].c_str());
}

void loudness_att_settings()
{
  draw_amount_selector("Attenuation", Preamp::getLoudnessAttenuation(), -15, 0);
}

void loudness_center_freq_settings()
{
  int loudnessCenterFreqIndex = Preamp::getLoudnessCenterFreq();
  String stringToDisplay = loudnessCenterFreqIndex != 0 ? String(String(loudness_center_freqs[loudnessCenterFreqIndex]) + String("Hz")) : String("Flat");
  draw_centered_desc_and_val("Center Frequency", stringToDisplay.c_str());
}

void loudness_high_boost_settings()
{
  draw_bool_selector("High Boost", Preamp::getLoudnessHighBoost());
}

void attenuations_menu_selector(int index)
{
  draw_menu("Attenuations", attenuations_menu_names[index].c_str());
}

void attenuation_left_settings()
{
  draw_amount_selector("Left", Preamp::getLeftAttenuation(), -80, 15);
}

void attenuation_right_setting()
{
  draw_amount_selector("Right", Preamp::getRightAttenuation(), -80, 15);
}

void attenuation_sub_setting()
{
  draw_amount_selector("Sub", Preamp::getSubAttenuation(), -80, 15);
}

void bass_filter_menu_selector(int index)
{
  draw_menu("Bass Filter", bass_menu_names[index].c_str());
}

void bass_qfactor_settings()
{
  draw_centered_desc_and_val("Q Factor", String(bass_q_factors[Preamp::getBassQFactor()]).c_str());
}

void bass_center_freq_settings()
{
  draw_centered_desc_and_val("Center Frequency", String(String(bass_center_freqs[Preamp::getBassCenterFreq()]) + String("Hz")).c_str());
}

void bass_dc_settings()
{
  draw_bool_selector("Bass DC", Preamp::getBassDC());
}

void middle_filter_menu_selector(int index)
{
  draw_menu("Middle Filter", middle_menu_names[index].c_str());
}

void middle_qfactor_settings()
{
  draw_centered_desc_and_val("Q Factor", String(middle_q_factors[Preamp::getMiddleQFactor()]).c_str());
}

void middle_center_freq_settings()
{
  draw_centered_desc_and_val("Center Frequency", String(String(middle_center_freqs[Preamp::getMiddleCenterFreq()]) + String("Hz")).c_str());
}

void treble_filter_menu_selector(int index)
{
  draw_menu("Treble Filter", treble_menu_names[index].c_str());
}

void treble_center_freq_settings()
{
  draw_centered_desc_and_val("Center Frequency", String(String(treble_center_freqs[Preamp::getTrebleCenterFreq()]) + String("kHz")).c_str());
}

void sub_menu_selector(int index)
{
  draw_menu("Subwoofer", sub_menu_names[index].c_str());
}

void sub_cutoff_freq_settings()
{
  int freqIndex = Preamp::getSubCutoffFreq();
  String descString = freqIndex == 0 ? String("Flat") : String(String(sub_cutoff_freqs[freqIndex]) + String("Hz"));
  draw_centered_desc_and_val("Cutoff Frequency", descString.c_str());
}

void soft_steps_menu_selector(int index)
{
  draw_menu("Soft Steps", soft_steps_menu_names[index].c_str());
}

void soft_steps_time_settings()
{
  draw_centered_desc_and_val("Soft Step Time", String(String(soft_step_times[Preamp::getSoftStepsTime()], 3) + "ms").c_str());
}

void soft_steps_volume_settings()
{
  draw_bool_selector("SS Volume", Preamp::getVolumeSoftStep());
}

void soft_steps_loudness_settings()
{
  draw_bool_selector("SS Loudness", Preamp::getLoudnessSoftStep());
}

void soft_steps_middle_settings()
{
  draw_bool_selector("SS Middle", Preamp::getMiddleSoftStep());
}

void soft_steps_bass_settings()
{
  draw_bool_selector("SS Bass", Preamp::getBassSoftStep());
}

void soft_steps_left_att_settings()
{
  draw_bool_selector("SS Att Left", Preamp::getLeftSoftSteps());
}

void soft_steps_right_att_settings()
{
  draw_bool_selector("SS Att Right", Preamp::getRightSoftSteps());
}

void soft_steps_sub_att_settings()
{
  draw_bool_selector("SS Att Sub", Preamp::getSubSoftSteps());
}

void soft_mute_time_settings()
{
  draw_centered_desc_and_val("Soft Mute Time", String(String(soft_mute_times[Preamp::getSoftMuteTime()], 2) + "ms").c_str());
}

void led_strip_menu_selector(int index)
{
  draw_menu("LED Strip", led_strip_menu_names[index].c_str());
}

void led_strip_effect_menu_selector(int index)
{
  uint32_t currentEffect = LedStrip::getEffectIdx();

  display.setFont(u8g2_font_ncenB14_tr);
  display_draw_center("Effect", 0);
  display.drawLine(0, display.getMaxCharHeight(), display.getDisplayWidth(), display.getMaxCharHeight());

  if (currentEffect == index)
    display.setFont(u8g2_font_lubBI18_tf);
  else
    display.setFont(u8g2_font_ncenB18_tr);
  display_draw_center(led_strip_effect_menu_names[index].c_str(), display.getDisplayHeight() - display.getMaxCharHeight() - 5);
}

void led_strip_color_menu_selector(int index)
{
  draw_menu("Color", led_strip_color_menu_names[index].c_str());
}

void led_strip_color_red_settings()
{
  draw_amount_selector("Red", static_cast<int>(LedStrip::getColor().R), 0, 255);
}

void led_strip_color_green_settings()
{
  draw_amount_selector("Green", static_cast<int>(LedStrip::getColor().G), 0, 255);
}

void led_strip_color_blue_settings()
{
  draw_amount_selector("Blue", static_cast<int>(LedStrip::getColor().B), 0, 255);
}

void led_strip_color_brightness_settings()
{
  draw_amount_selector("Brightness", static_cast<int>(LedStrip::getBrightness()), 0, 255);
}

void settings_menu_selector(int index)
{
  draw_menu("Settings", settings_menu_names[index].c_str());
}

void settings_screensaver_menu_selector(int index)
{
  draw_menu("Screensaver", settings_screensaver_menu_names[index].c_str());
}

void settings_screensaver_enable()
{
  draw_bool_selector("Screensaver", AppSettingsStore::getScreensaverEnabled());
}

void settings_screensaver_delay()
{
  draw_centered_desc_and_val("Delay", (String(static_cast<double>(AppSettingsStore::getScreensaverDelay()) / 60.0, 1) + "min").c_str());
}

void settings_screensaver_use_spectrum()
{
  draw_bool_selector("Use Spectrum", AppSettingsStore::getScreensaverSpectrum());
}

void settings_clip_detection()
{
  draw_bool_selector("Clip Detection", AppSettingsStore::getClipDetection());
}

void settings_overheat_menu_selector(int index)
{
  draw_menu("Overheat detection", settings_overheat_menu_names[index].c_str());
}

void settings_overheat_detection_enable()
{
  draw_bool_selector("Overheat Detection", AppSettingsStore::getOverheatDetection());
}

void settings_overheat_mute()
{
  draw_bool_selector("Mute on OH", AppSettingsStore::getOverheatMute());
}

void settings_overheat_blink()
{
  draw_bool_selector("Blink on OH", AppSettingsStore::getOverheatFlashLights());
}

void settings_brightness()
{
  draw_amount_selector("Brightness", static_cast<int>(AppSettingsStore::getDisplayBrightness()), 0, 255);
}

void factory_reset_confirmation()
{
  draw_centered_desc_and_val("Factory Reset", "Are you sure?");
}

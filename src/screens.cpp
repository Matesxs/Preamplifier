#include "screens.h"

#include "global_objects.h"
#include "display_handler.h"
#include "settings.h"

void push_indicator()
{
  uint16_t screenCenter = display.getDisplayWidth() / 2;

  if (longPush)
    display.drawLine(screenCenter - LONG_PUSH_INDICATOR_LENGTH, display.getDisplayHeight() - 1, screenCenter + LONG_PUSH_INDICATOR_LENGTH, display.getDisplayHeight() - 1);
  else if (shortPush)
    display.drawLine(screenCenter - SHORT_PUSH_INDICATOR_LENGTH, display.getDisplayHeight() - 1, screenCenter + SHORT_PUSH_INDICATOR_LENGTH, display.getDisplayHeight() - 1);
}

void main_screen()
{
  display.setFont(u8g2_font_ncenB14_tr);
  int selectedInput = preamp.getInput() + 1;
  display_draw_center(String(String("INPUT ") + (selectedInput != 4 ? String(selectedInput) : "BT")).c_str(), 0);
  
  String temp1Str = String(String(temps_store[0], 1) + String("C"));
  String temp2Str = String(String(temps_store[1], 1) + String("C"));
  String volumeString = String(String("VOL: ") + String(preamp.getVolume()));
  String gainString = String(String("GAIN: ") + String(preamp.getGain()));

  uint16_t bottomRowYOffset = display.getDisplayHeight() - display.getMaxCharHeight();

  display.setFont(u8g2_font_ncenB12_tr);
  display.drawStr(15, bottomRowYOffset, volumeString.c_str());
  display.drawStr(display.getDisplayWidth() - display.getStrWidth(gainString.c_str()) - 15, bottomRowYOffset, gainString.c_str());

  display.setFont(u8g2_font_ncenB10_tr);

  display.drawStr(0, 0, temp1Str.c_str());
  display.drawStr(display.getDisplayWidth() - display.getStrWidth(temp2Str.c_str()), 0, temp2Str.c_str());
}

void mute_screen()
{
  display.setFont(u8g2_font_ncenB18_tr);
  display_draw_center("MUTED");

  display.setFont(u8g2_font_ncenB10_tr);

  String temp1Str = String(String(temps_store[0], 1) + String("C"));
  String temp2Str = String(String(temps_store[1], 1) + String("C"));

  display.drawStr(0, 0, temp1Str.c_str());
  display.drawStr(display.getDisplayWidth() - display.getStrWidth(temp2Str.c_str()), 0, temp2Str.c_str());
}

void main_menu_selector(int index)
{
  draw_menu("Menu", main_menu_names[index].c_str());
}

void input_gain_settings()
{
  draw_amount_selector("Input Gain", preamp.getGain(), 0, 15);
}

void master_volume_settings()
{
  draw_amount_selector("Volume", preamp.getVolume(), -80, 15);
}

void treble_gain_settings()
{
  draw_amount_selector("Treble Gain", preamp.getTrebleGain(), -15, 15);
}

void middle_gain_settings()
{
  draw_amount_selector("Middle Gain", preamp.getMiddleGain(), -15, 15);
}

void bass_gain_settings()
{
  draw_amount_selector("Bass Gain", preamp.getBassGain(), -15, 15);
}

void loudness_menu_selector(int index)
{
  draw_menu("Loudness", loudness_menu_names[index].c_str());
}

void loudness_att_settings()
{
  draw_amount_selector("Attenuation", preamp.getLoudnessAttenuation(), 0, 15);
}

void loudness_center_freq_settings()
{
  int loudnessCenterFreqIndex = preamp.getLoudnessCenterFreq();
  String stringToDisplay = loudnessCenterFreqIndex != 0 ? String(String(loudness_center_freqs[loudnessCenterFreqIndex]) + String("Hz")) : String("Flat");
  draw_value_selector("Center Frequency", stringToDisplay.c_str());
}

void loudness_high_boost_settings()
{
  draw_bool_selector("High Boost", preamp.getLoudnessHighBoost());
}

void attenuations_menu_selector(int index)
{
  draw_menu("Attenuations", attenuations_menu_names[index].c_str());
}

void attenuation_left_settings()
{
  draw_amount_selector("Left Attenuation", preamp.getLeftAttenuation(), -80, 15);
}

void attenuation_right_setting()
{
  draw_amount_selector("Right Attenuation", preamp.getRightAttenuation(), -80, 15);
}

void attenuation_sub_setting()
{
  draw_amount_selector("Sub Attenuation", preamp.getSubAttenuation(), -80, 15);
}

void bass_filter_menu_selector(int index)
{
  draw_menu("Bass Filter", bass_menu_names[index].c_str());
}

void bass_qfactor_settings()
{
  draw_value_selector("Q Factor", String(bass_q_factors[preamp.getBassQFactor()]).c_str());
}

void bass_center_freq_settings()
{
  draw_value_selector("Center Frequency", String(String(bass_center_freqs[preamp.getBassCenterFreq()]) + String("Hz")).c_str());
}

void bass_dc_settings()
{
  draw_bool_selector("Bass DC", preamp.getBassDC());
}

void middle_filter_menu_selector(int index)
{
  draw_menu("Middle Filter", middle_menu_names[index].c_str());
}

void middle_qfactor_settings()
{
  draw_value_selector("Q Factor", String(middle_q_factors[preamp.getMiddleQFactor()]).c_str());
}

void middle_center_freq_settings()
{
  draw_value_selector("Center Frequency", String(String(middle_center_freqs[preamp.getMiddleCenterFreq()]) + String("Hz")).c_str());
}

void treble_filter_menu_selector(int index)
{
  draw_menu("Treble Filter", treble_menu_names[index].c_str());
}

void treble_center_freq_settings()
{
  draw_value_selector("Center Frequency", String(String(treble_center_freqs[preamp.getTrebleCenterFreq()]) + String("kHz")).c_str());
}

void sub_menu_selector(int index)
{
  draw_menu("Subwoofer", sub_menu_names[index].c_str());
}

void sub_cutoff_freq_settings()
{
  int freqIndex = preamp.getSubCutoffFreq();
  String descString = freqIndex == 0 ? String("Flat") : String(String(sub_cutoff_freqs[freqIndex]) + String("Hz"));
  draw_value_selector("Cutoff Frequency", descString.c_str());
}

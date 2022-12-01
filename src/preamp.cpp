#include "preamp.h"

#include <Wire.h>

#include "debug.h"
#include "pins.h"

namespace Preamp
{
  InputSettings m_inputSettings;
  bool m_inputSettingsChanged = false;

  LoudnessSettings m_loudnessSettings;
  bool m_loudnessSettingsChanged = false;

  VolumeSettings m_volumeSettings;
  bool m_volumeSettingsChanged = false;

  TrebleFilter m_trebleFilter;
  bool m_trebleFilterChanged = false;

  MiddleFilter m_middleFilter;
  bool m_middleFilterChanged = false;

  BassFilter m_bassFilter;
  bool m_bassFilterChanged = false;

  FreqSettings m_freqSettings;
  bool m_freqSettingsChanged = false;

  Attenuation m_attenuation;
  bool m_attenuationChanged = false;

  SoftMuteStep m_softMuteStep;
  bool m_softMuteStepChanged = false;

  Preferences m_inputSettingsEEPROM;
  Preferences m_loudnessSettingsEEPROM;
  Preferences m_volumeSettingsEEPROM;
  Preferences m_trebleFilterEEPROM;
  Preferences m_middleFilterEEPROM;
  Preferences m_bassFilterEEPROM;
  Preferences m_freqSettingsEEPROM;
  Preferences m_attenuationEEPROM;
  Preferences m_softMuteStepEEPROM;

  TDA7419 m_controller;

  void init()
  {
    Wire.beginTransmission(TDA7419_address);
    uint8_t error = Wire.endTransmission();
    if (error != 0)
      DEBUG("Failed to connect to TDA7419: %d\n", error);
    else
      DEBUG("TDA7419 connected\n");

    m_controller = TDA7419();

    m_inputSettingsEEPROM.begin("pa_in", false);
    m_loudnessSettingsEEPROM.begin("pa_loud", false);
    m_volumeSettingsEEPROM.begin("pa_vol", false);
    m_trebleFilterEEPROM.begin("pa_tf", false);
    m_middleFilterEEPROM.begin("pa_mf", false);
    m_bassFilterEEPROM.begin("pa_bf", false);
    m_freqSettingsEEPROM.begin("pa_fs", false);
    m_attenuationEEPROM.begin("pa_a", false);
    m_softMuteStepEEPROM.begin("pa_sms", false);

    loadFromMemory();

    m_controller.setInput(input_mapping[m_inputSettings.selected_input], getInputGain(), PREAMP_INPUT_AUTO_Z);
    m_controller.setInput2(PREAMP_INPUT2_SOURCE, PREAMP_INPUT2_GAIN, PREAMP_REAR_SPEAKER_SOURCE);
    m_controller.setVolume(m_volumeSettings.volume, m_volumeSettings.soft_step);
    m_controller.setSoft(1, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
    m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, PREAMP_REFERENCE_OUTPUT_SELECT);
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
    m_controller.setAtt_LF(m_attenuation.att_lf, m_attenuation.soft_steps_lf);
    m_controller.setAtt_RF(m_attenuation.att_rf, m_attenuation.soft_steps_rf);
    m_controller.setAtt_SUB(m_attenuation.att_sub, m_attenuation.soft_steps_sub);
    m_controller.setMix_Gain_Eff(PREAMP_MIXING_TO_LEFT_FRONT, PREAMP_MIXING_TO_RIGHT_FRONT, PREAMP_MIXING_ENABLE,  PREAMP_SUBWOOFER_ENABLE, PREAMP_GAIN_EFFECT_HPF);
    m_controller.setSpektor(PREAMP_SA_FILTER_Q_FACTOR, PREAMP_SA_RESET_MODE, PREAMP_SA_SOURCE, PREAMP_SA_RUN, PREAMP_SA_RESET, PREAMP_SA_CLOCK_SOURCE, PREAMP_SA_COUPLING_MODE);
  }

  int getInput() { return m_inputSettings.selected_input; }

  void setInput(int val)
  {
    if (val < 0) val = 0;
    if (val > (number_of_channels - 1)) val = number_of_channels - 1;
    if (val == m_inputSettings.selected_input) return;

    m_inputSettings.selected_input = val;
    m_inputSettingsChanged = true;
    m_controller.setInput(input_mapping[m_inputSettings.selected_input], getInputGain(), PREAMP_INPUT_AUTO_Z);
  }

  void rotateInput()
  {
    if (number_of_channels == 1) return;

    m_inputSettings.selected_input++;

    if (m_inputSettings.selected_input > (number_of_channels - 1))
      m_inputSettings.selected_input = 0;

    m_inputSettingsChanged = true;
    m_controller.setInput(input_mapping[m_inputSettings.selected_input], getInputGain(), PREAMP_INPUT_AUTO_Z);
  }

  void incInput()
  {
    if (number_of_channels == 1) return;

    if (m_inputSettings.selected_input >= (number_of_channels - 1)) return;

    m_inputSettings.selected_input++;
    m_inputSettingsChanged = true;
    m_controller.setInput(input_mapping[m_inputSettings.selected_input], getInputGain(), PREAMP_INPUT_AUTO_Z);
  }

  void decInput()
  {
    if (number_of_channels == 1) return;

    if (m_inputSettings.selected_input <= 0) return;

    m_inputSettings.selected_input--;
    m_inputSettingsChanged = true;
    m_controller.setInput(input_mapping[m_inputSettings.selected_input], getInputGain(), PREAMP_INPUT_AUTO_Z);
  }

  int getInputGain() 
  { 
  #ifdef INPUT_GAIN_POTENTIOMETER
    return m_inputSettings.gain1;
  #else
    switch (m_inputSettings.selected_input)
    {
    case 0:
      return m_inputSettings.gain1;

    case 1:
      return m_inputSettings.gain2;

    case 2:
      return m_inputSettings.gain3;

    case 3:
      return m_inputSettings.gain4;
    
    default:
      DEBUG("[WARNING] Detected invalid input (%d) for getting input gain\n", m_inputSettings.selected_input);
      return m_inputSettings.gain1;
    }
  #endif
  }

  void setInputGain(int val)
  {
    if (val < 0) val = 0;
    if (val > 15) val = 15;
    if (val == getInputGain()) return;

  #ifndef INPUT_GAIN_POTENTIOMETER
    switch (m_inputSettings.selected_input)
    {
    case 0:
      m_inputSettings.gain1 = val;
      break;

    case 1:
      m_inputSettings.gain2 = val;
      break;

    case 2:
      m_inputSettings.gain3 = val;
      break;

    case 3:
      m_inputSettings.gain4 = val;
      break;
    
    default:
      DEBUG("[WARNING] Detected invalid input (%d) for setting input gain\n", m_inputSettings.selected_input);
      m_inputSettings.gain1 = val;
      break;
    }

    m_inputSettingsChanged = true;
  #else
    m_inputSettings.gain1 = val;
  #endif

    m_controller.setInput(input_mapping[m_inputSettings.selected_input], getInputGain(), PREAMP_INPUT_AUTO_Z);
  }

  void incInputGain()
  {
    setInputGain(getInputGain() + 1);
  }

  void decInputGain()
  {
    setInputGain(getInputGain() - 1);
  }

  int getLoudnessAttenuation() { return m_loudnessSettings.attenuation; }

  void setLoudnessAttenuation(int val)
  {
    if (val < 0) val = 0;
    if (val > 15) val = 15;
    if (val == m_loudnessSettings.attenuation) return;

    m_loudnessSettings.attenuation = val;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  void incLoudnessAttenuation()
  {
    if (m_loudnessSettings.attenuation >= 15) return;

    m_loudnessSettings.attenuation++;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  void decLoudnessAttenuation()
  {
    if (m_loudnessSettings.attenuation <= 0) return;

    m_loudnessSettings.attenuation--;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  int getLoudnessCenterFreq() { return m_loudnessSettings.center_freq; }

  void setLoudnessCenterFreq(int val)
  {
    if (val < 0) val = 0;
    if (val > 3) val = 3;
    if (val == m_loudnessSettings.center_freq) return;

    m_loudnessSettings.center_freq = val;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  void incLoudnessCenterFreq()
  {
    if (m_loudnessSettings.center_freq >= 3) return;

    m_loudnessSettings.center_freq++;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  void decLoudnessCenterFreq()
  {
    if (m_loudnessSettings.center_freq <= 0) return;

    m_loudnessSettings.center_freq--;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  int getLoudnessHighBoost() { return !m_loudnessSettings.high_boost; }

  void setLoudnessHighBoost(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_loudnessSettings.high_boost) return;

    m_loudnessSettings.high_boost = !val;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  void switchLoudnessHighBoost()
  {
    m_loudnessSettings.high_boost = !m_loudnessSettings.high_boost;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  int getLoudnessSoftStep() { return !m_loudnessSettings.soft_step; }

  void setLoudnessSoftStep(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_loudnessSettings.soft_step) return;

    m_loudnessSettings.soft_step = !val;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  void switchLoudnessSoftStep()
  {
    m_loudnessSettings.soft_step = !m_loudnessSettings.soft_step;
    m_loudnessSettingsChanged = true;
    m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, m_loudnessSettings.soft_step);
  }

  int getVolume() { return m_volumeSettings.volume; }

  void setVolume(int val)
  {
    if (val < -80) val = -80;
    if (val > 15) val = 15;
    if (val == m_volumeSettings.volume) return;

    m_volumeSettings.volume = val;
    m_volumeSettingsChanged = true;
    m_controller.setVolume(m_volumeSettings.volume, m_volumeSettings.soft_step);
  }

  void incVolume()
  {
    if (m_volumeSettings.volume >= 15) return;
    
    m_volumeSettings.volume++;
    m_volumeSettingsChanged = true;
    m_controller.setVolume(m_volumeSettings.volume, m_volumeSettings.soft_step);
  }

  void decVolume()
  {
    if (m_volumeSettings.volume <= -80) return;
    
    m_volumeSettings.volume--;
    m_volumeSettingsChanged = true;
    m_controller.setVolume(m_volumeSettings.volume, m_volumeSettings.soft_step);
  }

  int getVolumeSoftStep() { return !m_volumeSettings.soft_step; }

  void setVolumeSoftStep(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_volumeSettings.soft_step) return;

    m_volumeSettings.soft_step = !val;
    m_volumeSettingsChanged = true;
    m_controller.setVolume(m_volumeSettings.volume, m_volumeSettings.soft_step);
  }

  void switchVolumeSoftStep()
  {
    m_volumeSettings.soft_step = !m_volumeSettings.soft_step;
    m_volumeSettingsChanged = true;
    m_controller.setVolume(m_volumeSettings.volume, m_volumeSettings.soft_step);
  }

  int getTrebleGain() { return m_trebleFilter.gain; }

  void setTrebleGain(int val)
  {
    if (val < -15) val = -15;
    if (val > 15) val = 15;
    if (val == m_trebleFilter.gain) return;

    m_trebleFilter.gain = val;
#ifndef TREBLE_GAIN_POTENTIOMETER
    m_trebleFilterChanged = true;
#endif
    m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, PREAMP_REFERENCE_OUTPUT_SELECT);
  }

  void incTrebleGain()
  {
    if (m_trebleFilter.gain >= 15) return;

    m_trebleFilter.gain++;
#ifndef TREBLE_GAIN_POTENTIOMETER
    m_trebleFilterChanged = true;
#endif
    m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, PREAMP_REFERENCE_OUTPUT_SELECT);
  }

  void decTrebleGain()
  {
    if (m_trebleFilter.gain <= -15) return;

    m_trebleFilter.gain--;
#ifndef TREBLE_GAIN_POTENTIOMETER
    m_trebleFilterChanged = true;
#endif
    m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, PREAMP_REFERENCE_OUTPUT_SELECT);
  }

  int getTrebleCenterFreq() { return m_trebleFilter.center_freq; }

  void setTrebleCenterFreq(int val)
  {
    if (val < 0) val = 0;
    if (val > 3) val = 3;
    if (val == m_trebleFilter.center_freq) return;

    m_trebleFilter.center_freq = val;
    m_trebleFilterChanged = true;
    m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, PREAMP_REFERENCE_OUTPUT_SELECT);
  }

  void incTrebleCenterFreq()
  {
    if (m_trebleFilter.center_freq >= 3) return;

    m_trebleFilter.center_freq++;
    m_trebleFilterChanged = true;
    m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, PREAMP_REFERENCE_OUTPUT_SELECT);
  }

  void decTrebleCenterFreq()
  {
    if (m_trebleFilter.center_freq <= 0) return;

    m_trebleFilter.center_freq--;
    m_trebleFilterChanged = true;
    m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, PREAMP_REFERENCE_OUTPUT_SELECT);
  }

  int getMiddleGain() { return m_middleFilter.gain; }

  void setMiddleGain(int val)
  {
    if (val < -15) val = -15;
    if (val > 15) val = 15;
    if (val == m_middleFilter.gain) return;

    m_middleFilter.gain = val;
#ifndef MIDDLE_GAIN_POTENTIOMETER
    m_middleFilterChanged = true;
#endif
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
  }

  void incMiddleGain()
  {
    if (m_middleFilter.gain >= 15) return;

    m_middleFilter.gain++;
#ifndef MIDDLE_GAIN_POTENTIOMETER
    m_middleFilterChanged = true;
#endif
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
  }

  void decMiddleGain()
  {
    if (m_middleFilter.gain <= -15) return;

    m_middleFilter.gain--;
#ifndef MIDDLE_GAIN_POTENTIOMETER
    m_middleFilterChanged = true;
#endif
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
  }

  int getMiddleQFactor() { return m_middleFilter.qfactor; }

  void setMiddleQFactor(int val)
  {
    if (val < 0) val = 0;
    if (val > 3) val = 3;
    if (val == m_middleFilter.qfactor) return;

    m_middleFilter.qfactor = val;
    m_middleFilterChanged = true;
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
  }

  void incMiddleQFactor()
  {
    if (m_middleFilter.qfactor >= 3) return;

    m_middleFilter.qfactor++;
    m_middleFilterChanged = true;
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
  }

  void decMiddleQFactor()
  {
    if (m_middleFilter.qfactor <= 0) return;

    m_middleFilter.qfactor--;
    m_middleFilterChanged = true;
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
  }

  int getMiddleSoftStep() { return !m_middleFilter.soft_step; }

  void setMiddleSoftStep(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_middleFilter.soft_step) return;

    m_middleFilter.soft_step = !val;
    m_middleFilterChanged = true;
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
  }

  void switchMiddleSoftStep()
  {
    m_middleFilter.soft_step = !m_middleFilter.soft_step;
    m_middleFilterChanged = true;
    m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, m_middleFilter.soft_step);
  }

  int getBassGain() { return m_bassFilter.gain; }

  void setBassGain(int val)
  {
    if (val < -15) val = -15;
    if (val > 15) val = 15;
    if (val == m_bassFilter.gain) return;

    m_bassFilter.gain = val;
#ifndef BASS_GAIN_POTENTIOMETER
    m_bassFilterChanged = true;
#endif
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
  }

  void incBassGain()
  {
    if (m_bassFilter.gain >= 15) return;

    m_bassFilter.gain++;
#ifndef BASS_GAIN_POTENTIOMETER
    m_bassFilterChanged = true;
#endif
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
  }

  void decBassGain()
  {
    if (m_bassFilter.gain <= -15) return;

    m_bassFilter.gain--;
#ifndef BASS_GAIN_POTENTIOMETER
    m_bassFilterChanged = true;
#endif
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
  }

  int getBassQFactor() { return m_bassFilter.qfactor; }

  void setBassQFactor(int val)
  {
    if (val < 0) val = 0;
    if (val > 3) val = 3;
    if (val == m_bassFilter.qfactor) return;

    m_bassFilter.qfactor = val;
    m_bassFilterChanged = true;
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
  }

  void incBassQFactor()
  {
    if (m_bassFilter.qfactor >= 3) return;

    m_bassFilter.qfactor++;
    m_bassFilterChanged = true;
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
  }

  void decBassQFactor()
  {
    if (m_bassFilter.qfactor <= 0) return;

    m_bassFilter.qfactor--;
    m_bassFilterChanged = true;
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
  }

  int getBassSoftStep() { return !m_bassFilter.soft_step; }

  void setBassSoftStep(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_bassFilter.soft_step) return;

    m_bassFilter.soft_step = !val;
    m_bassFilterChanged = true;
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
  }

  void switchBassSoftStep()
  {
    m_bassFilter.soft_step = !m_bassFilter.soft_step;
    m_bassFilterChanged = true;
    m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, m_bassFilter.soft_step);
  }

  int getBassCenterFreq() { return m_freqSettings.bass_center_freq; }

  void setBassCenterFreq(int val)
  {
    if (val < 0) val = 0;
    if (val > 3) val = 3;
    if (val == m_freqSettings.bass_center_freq) return;

    m_freqSettings.bass_center_freq = val;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  void incBassCenterFreq()
  {
    if (m_freqSettings.bass_center_freq >= 3) return;

    m_freqSettings.bass_center_freq++;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  void decBassCenterFreq()
  {
    if (m_freqSettings.bass_center_freq <= 0) return;

    m_freqSettings.bass_center_freq--;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  int getBassDC() { return !m_freqSettings.bass_dc; }

  void setBassDC(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_freqSettings.bass_dc) return;

    m_freqSettings.bass_dc = !val;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  void switchBassDC()
  {
    m_freqSettings.bass_dc = !m_freqSettings.bass_dc;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  int getMiddleCenterFreq() { return m_freqSettings.mid_center_freq; }

  void setMiddleCenterFreq(int val)
  {
    if (val < 0) val = 0;
    if (val > 3) val = 3;
    if (val == m_freqSettings.mid_center_freq) return;

    m_freqSettings.mid_center_freq = val;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  void incMiddleCenterFreq()
  {
    if (m_freqSettings.mid_center_freq >= 3) return;

    m_freqSettings.mid_center_freq++;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  void decMiddleCenterFreq()
  {
    if (m_freqSettings.mid_center_freq <= 0) return;

    m_freqSettings.mid_center_freq--;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  int getSubCutoffFreq() { return m_freqSettings.sub_cutoff_freq; }

  void setSubCutoffFreq(int val)
  {
    if (val < 0) val = 0;
    if (val > 3) val = 3;
    if (val == m_freqSettings.sub_cutoff_freq) return;

    m_freqSettings.sub_cutoff_freq = val;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  void incSubCutoffFreq()
  {
    if (m_freqSettings.sub_cutoff_freq >= 3) return;

    m_freqSettings.sub_cutoff_freq++;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  void decSubCutoffFreq()
  {
    if (m_freqSettings.sub_cutoff_freq <= 0) return;

    m_freqSettings.sub_cutoff_freq--;
    m_freqSettingsChanged = true;
    m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, PREAMP_SMOOTHING_FILTER);
  }

  int getLeftAttenuation() { return m_attenuation.att_lf; }

  void setLeftAttenuation(int val)
  {
    if (val > 15) val = 15;
    if (val < -80) val = -80;
    if (val == m_attenuation.att_lf) return;

    m_attenuation.att_lf = val;
    m_attenuationChanged = true;
    m_controller.setAtt_LF(m_attenuation.att_lf, m_attenuation.soft_steps_lf);
  }

  void incLeftAttenuation()
  {
    if (m_attenuation.att_lf >= 15) return;

    m_attenuation.att_lf++;
    m_attenuationChanged = true;
    m_controller.setAtt_LF(m_attenuation.att_lf, m_attenuation.soft_steps_lf);
  }

  void decLeftAttenuation()
  {
    if (m_attenuation.att_lf <= -80) return;

    m_attenuation.att_lf--;
    m_attenuationChanged = true;
    m_controller.setAtt_LF(m_attenuation.att_lf, m_attenuation.soft_steps_lf);
  }

  int getRightAttenuation() { return m_attenuation.att_rf; }

  void setRightAttenuation(int val)
  {
    if (val > 15) val = 15;
    if (val < -80) val = -80;
    if (val == m_attenuation.att_rf) return;

    m_attenuation.att_rf = val;
    m_attenuationChanged = true;
    m_controller.setAtt_RF(m_attenuation.att_rf, m_attenuation.soft_steps_rf);
  }

  void incRightAttenuation()
  {
    if (m_attenuation.att_rf >= 15) return;

    m_attenuation.att_rf++;
    m_attenuationChanged = true;
    m_controller.setAtt_RF(m_attenuation.att_rf, m_attenuation.soft_steps_rf);
  }

  void decRightAttenuation()
  {
    if (m_attenuation.att_rf <= -80) return;

    m_attenuation.att_rf--;
    m_attenuationChanged = true;
    m_controller.setAtt_RF(m_attenuation.att_rf, m_attenuation.soft_steps_rf);
  }

  int getSubAttenuation() { return m_attenuation.att_sub; }

  void setSubAttenuation(int val)
  {
    if (val > 15) val = 15;
    if (val < -80) val = -80;
    if (val == m_attenuation.att_sub) return;

    m_attenuation.att_sub = val;
    m_attenuationChanged = true;
    m_controller.setAtt_SUB(m_attenuation.att_sub, m_attenuation.soft_steps_sub);
  }

  void incSubAttenuation()
  {
    if (m_attenuation.att_sub >= 15) return;

    m_attenuation.att_sub++;
    m_attenuationChanged = true;
    m_controller.setAtt_SUB(m_attenuation.att_sub, m_attenuation.soft_steps_sub);
  }

  void decSubAttenuation()
  {
    if (m_attenuation.att_sub <= -80) return;

    m_attenuation.att_sub--;
    m_attenuationChanged = true;
    m_controller.setAtt_SUB(m_attenuation.att_sub, m_attenuation.soft_steps_sub);
  }

  int getLeftSoftSteps() { return !m_attenuation.soft_steps_lf; }

  void setLeftSoftSteps(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_attenuation.soft_steps_lf) return;

    m_attenuation.soft_steps_lf = !val;
    m_attenuationChanged = true;
    m_controller.setAtt_LF(m_attenuation.att_lf, m_attenuation.soft_steps_lf);
  }

  void switchLeftSoftSteps()
  {
    m_attenuation.soft_steps_lf = !m_attenuation.soft_steps_lf;
    m_attenuationChanged = true;
    m_controller.setAtt_LF(m_attenuation.att_lf, m_attenuation.soft_steps_lf);
  }

  int getRightSoftSteps() { return !m_attenuation.soft_steps_rf; }

  void setRightSoftSteps(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_attenuation.soft_steps_rf) return;

    m_attenuation.soft_steps_rf = !val;
    m_attenuationChanged = true;
    m_controller.setAtt_RF(m_attenuation.att_rf, m_attenuation.soft_steps_rf);
  }

  void switchRightSoftSteps()
  {
    m_attenuation.soft_steps_rf = !m_attenuation.soft_steps_rf;
    m_attenuationChanged = true;
    m_controller.setAtt_RF(m_attenuation.att_rf, m_attenuation.soft_steps_rf);
  }

  int getSubSoftSteps() { return !m_attenuation.soft_steps_sub; }

  void setSubSoftSteps(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_attenuation.soft_steps_sub) return;

    m_attenuation.soft_steps_sub = !val;
    m_attenuationChanged = true;
    m_controller.setAtt_SUB(m_attenuation.att_sub, m_attenuation.soft_steps_sub);
  }

  void switchSubSoftSteps()
  {
    m_attenuation.soft_steps_sub = !m_attenuation.soft_steps_sub;
    m_attenuationChanged = true;
    m_controller.setAtt_SUB(m_attenuation.att_sub, m_attenuation.soft_steps_sub);
  }

  int getSoftStepsTime() { return m_softMuteStep.soft_step_time; }

  void setSoftStepsTime(int val)
  {
    if (val < 0) val = 0;
    if (val > 7) val = 7;
    if (val == m_softMuteStep.soft_step_time) return;

    m_softMuteStep.soft_step_time = val;
    m_softMuteStepChanged = true;
    m_controller.setSoft(m_softMuteStep.soft_mute, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
  }

  void incSoftStepsTime()
  {
    if (m_softMuteStep.soft_step_time >= 7) return;

    m_softMuteStep.soft_step_time++;
    m_softMuteStepChanged = true;
    m_controller.setSoft(m_softMuteStep.soft_mute, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
  }

  void decSoftStepsTime()
  {
    if (m_softMuteStep.soft_step_time <= 0) return;

    m_softMuteStep.soft_step_time--;
    m_softMuteStepChanged = true;
    m_controller.setSoft(m_softMuteStep.soft_mute, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
  }

  void setSoftMute(int val)
  {
    if (val < 0) val = 0;
    if (val > 1) val = 1;
    if (val == !m_softMuteStep.soft_mute) return;

    m_softMuteStep.soft_mute = !val;
    m_controller.setSoft(m_softMuteStep.soft_mute, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
  }

  void switchSoftMute()
  {
    m_softMuteStep.soft_mute = !m_softMuteStep.soft_mute;
    m_controller.setSoft(m_softMuteStep.soft_mute, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
  }

  int getSoftMuteTime() { return m_softMuteStep.soft_mute_time; }

  void setSoftMuteTime(int val)
  {
    if (val < 0) val = 0;
    if (val > 2) val = 2;
    if (val == m_softMuteStep.soft_mute_time) return;

    m_softMuteStep.soft_mute_time = val;
    m_softMuteStepChanged = true;
    m_controller.setSoft(m_softMuteStep.soft_mute, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
  }

  void incSoftMuteTime()
  {
    if (m_softMuteStep.soft_mute_time >= 2) return;

    m_softMuteStep.soft_mute_time++;
    m_softMuteStepChanged = true;
    m_controller.setSoft(m_softMuteStep.soft_mute, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
  }

  void decSoftMuteTime()
  {
    if (m_softMuteStep.soft_mute_time <= 0) return;

    m_softMuteStep.soft_mute_time--;
    m_softMuteStepChanged = true;
    m_controller.setSoft(m_softMuteStep.soft_mute, PREAMP_PIN_INFLUENCE_FOR_MUTE, m_softMuteStep.soft_mute_time, m_softMuteStep.soft_step_time, PREAMP_CLOCK_FAST_MODE);
  }

  void mute() 
  { 
    setSoftMute(1);
  }

  void unmute()
  {
    setSoftMute(0);
  }

  void saveInputSettings()
  {
    m_inputSettingsEEPROM.putInt("sel_inp", m_inputSettings.selected_input);
  #ifndef INPUT_GAIN_POTENTIOMETER
    m_inputSettingsEEPROM.putInt("gain1", m_inputSettings.gain1);
    m_inputSettingsEEPROM.putInt("gain2", m_inputSettings.gain2);
    m_inputSettingsEEPROM.putInt("gain3", m_inputSettings.gain3);
    m_inputSettingsEEPROM.putInt("gain4", m_inputSettings.gain4);
  #endif

    m_inputSettingsChanged = false;
  }

  void saveLoudnessSettings()
  {
    m_loudnessSettingsEEPROM.putInt("att", m_loudnessSettings.attenuation);
    m_loudnessSettingsEEPROM.putInt("cf", m_loudnessSettings.center_freq);
    m_loudnessSettingsEEPROM.putInt("hb", m_loudnessSettings.high_boost);
    m_loudnessSettingsEEPROM.putInt("ss", m_loudnessSettings.soft_step);

    m_loudnessSettingsChanged = false;
  }

  void saveVolume()
  {
    m_volumeSettingsEEPROM.putInt("vol", m_volumeSettings.volume);
    m_volumeSettingsEEPROM.putInt("ss", m_volumeSettings.soft_step);

    m_volumeSettingsChanged = false;
  }

  void saveTrebleSettings()
  {
    m_trebleFilterEEPROM.putInt("cf", m_trebleFilter.center_freq);
#ifndef TREBLE_GAIN_POTENTIOMETER
    m_trebleFilterEEPROM.putInt("gain", m_trebleFilter.gain);
#endif

    m_trebleFilterChanged = false;
  }

  void saveMiddleSettings()
  {
    m_middleFilterEEPROM.putInt("qf", m_middleFilter.qfactor);
    m_middleFilterEEPROM.putInt("ss", m_middleFilter.soft_step);
#ifndef MIDDLE_GAIN_POTENTIOMETER
    m_middleFilterEEPROM.putInt("gain", m_middleFilter.gain);
#endif

    m_middleFilterChanged = false;
  }

  void saveBassSettings()
  {
    m_bassFilterEEPROM.putInt("qf", m_bassFilter.qfactor);
    m_bassFilterEEPROM.putInt("ss", m_bassFilter.soft_step);
#ifndef BASS_GAIN_POTENTIOMETER
    m_bassFilterEEPROM.putInt("gain", m_bassFilter.gain);
#endif

    m_bassFilterChanged = false;
  }

  void saveFreqSettings()
  {
    m_freqSettingsEEPROM.putInt("scf", m_freqSettings.sub_cutoff_freq);
    m_freqSettingsEEPROM.putInt("mcf", m_freqSettings.mid_center_freq);
    m_freqSettingsEEPROM.putInt("bcf", m_freqSettings.bass_center_freq);
    m_freqSettingsEEPROM.putInt("bdc", m_freqSettings.bass_dc);

    m_freqSettingsChanged = false;
  }

  void saveAttenuationSettings()
  {
    m_attenuationEEPROM.putInt("att_lf", m_attenuation.att_lf);
    m_attenuationEEPROM.putInt("att_rf", m_attenuation.att_rf);
    m_attenuationEEPROM.putInt("att_sub", m_attenuation.att_sub);
    m_attenuationEEPROM.putInt("ss_lf", m_attenuation.soft_steps_lf);
    m_attenuationEEPROM.putInt("ss_rf", m_attenuation.soft_steps_rf);
    m_attenuationEEPROM.putInt("ss_sub", m_attenuation.soft_steps_sub);

    m_attenuationChanged = false;
  }

  void saveSoftMuteStep()
  {
    m_softMuteStepEEPROM.putInt("smt", m_softMuteStep.soft_mute_time);
    m_softMuteStepEEPROM.putInt("sst", m_softMuteStep.soft_step_time);

    m_softMuteStepChanged = false;
  }

  void saveChanged()
  {
    if (m_inputSettingsChanged)
      saveInputSettings();
    if (m_loudnessSettingsChanged)
      saveLoudnessSettings();
    if (m_volumeSettingsChanged)
      saveVolume();
    if (m_trebleFilterChanged)
      saveTrebleSettings();
    if (m_middleFilterChanged)
      saveMiddleSettings();
    if (m_bassFilterChanged)
      saveBassSettings();
    if (m_freqSettingsChanged)
      saveFreqSettings();
    if (m_attenuationChanged)
      saveAttenuationSettings();
    if (m_softMuteStepChanged)
      saveSoftMuteStep();
  }

  void loadFromMemory()
  {
    m_inputSettings.selected_input = m_inputSettingsEEPROM.getInt("sel_inp", m_inputSettings.selected_input);
  #ifndef INPUT_GAIN_POTENTIOMETER
    m_inputSettings.gain1 = m_inputSettingsEEPROM.getInt("gain1", m_inputSettings.gain1);
    m_inputSettings.gain2 = m_inputSettingsEEPROM.getInt("gain2", m_inputSettings.gain2);
    m_inputSettings.gain3 = m_inputSettingsEEPROM.getInt("gain3", m_inputSettings.gain3);
    m_inputSettings.gain4 = m_inputSettingsEEPROM.getInt("gain4", m_inputSettings.gain4);
  #endif

    m_loudnessSettings.attenuation = m_loudnessSettingsEEPROM.getInt("att", m_loudnessSettings.attenuation);
    m_loudnessSettings.center_freq = m_loudnessSettingsEEPROM.getInt("cf", m_loudnessSettings.center_freq);
    m_loudnessSettings.high_boost = m_loudnessSettingsEEPROM.getInt("hb", m_loudnessSettings.high_boost);
    m_loudnessSettings.soft_step = m_loudnessSettingsEEPROM.getInt("ss", m_loudnessSettings.soft_step);

    m_volumeSettings.volume = m_volumeSettingsEEPROM.getInt("vol", m_volumeSettings.volume);
    m_volumeSettings.soft_step = m_volumeSettingsEEPROM.getInt("ss", m_volumeSettings.soft_step);

    m_trebleFilter.center_freq = m_trebleFilterEEPROM.getInt("cf", m_trebleFilter.center_freq);
#ifndef TREBLE_GAIN_POTENTIOMETER
    m_trebleFilter.gain = m_trebleFilterEEPROM.getInt("gain", m_trebleFilter.gain);
#endif

    m_middleFilter.qfactor = m_middleFilterEEPROM.getInt("qf", m_middleFilter.qfactor);
    m_middleFilter.soft_step = m_middleFilterEEPROM.getInt("ss", m_middleFilter.soft_step);
#ifndef MIDDLE_GAIN_POTENTIOMETER
    m_middleFilter.gain = m_middleFilterEEPROM.getInt("gain", m_middleFilter.gain);
#endif

    m_bassFilter.qfactor = m_bassFilterEEPROM.getInt("qf", m_bassFilter.qfactor);
    m_bassFilter.soft_step = m_bassFilterEEPROM.getInt("ss", m_bassFilter.soft_step);
#ifndef BASS_GAIN_POTENTIOMETER
    m_bassFilter.gain = m_bassFilterEEPROM.getInt("gain", m_bassFilter.gain);
#endif

    m_freqSettings.sub_cutoff_freq = m_freqSettingsEEPROM.getInt("scf", m_freqSettings.sub_cutoff_freq);
    m_freqSettings.mid_center_freq = m_freqSettingsEEPROM.getInt("mcf", m_freqSettings.mid_center_freq);
    m_freqSettings.bass_center_freq = m_freqSettingsEEPROM.getInt("bcf", m_freqSettings.bass_center_freq);
    m_freqSettings.bass_dc = m_freqSettingsEEPROM.getInt("bdc", m_freqSettings.bass_dc);

    m_attenuation.att_lf = m_attenuationEEPROM.getInt("att_lf", m_attenuation.att_lf);
    m_attenuation.att_rf = m_attenuationEEPROM.getInt("att_rf", m_attenuation.att_rf);
    m_attenuation.att_sub = m_attenuationEEPROM.getInt("att_sub", m_attenuation.att_sub);
    m_attenuation.soft_steps_lf = m_attenuationEEPROM.getInt("ss_lf", m_attenuation.soft_steps_lf);
    m_attenuation.soft_steps_rf = m_attenuationEEPROM.getInt("ss_rf", m_attenuation.soft_steps_rf);
    m_attenuation.soft_steps_sub = m_attenuationEEPROM.getInt("ss_sub", m_attenuation.soft_steps_sub);

    m_softMuteStep.soft_mute_time = m_softMuteStepEEPROM.getInt("smt", m_softMuteStep.soft_mute_time);
    m_softMuteStep.soft_step_time = m_softMuteStepEEPROM.getInt("sst", m_softMuteStep.soft_step_time);
  }

  void resetMemory()
  {
    m_inputSettingsEEPROM.clear();
    m_loudnessSettingsEEPROM.clear();
    m_volumeSettingsEEPROM.clear();
    m_trebleFilterEEPROM.clear();
    m_middleFilterEEPROM.clear();
    m_bassFilterEEPROM.clear();
    m_freqSettingsEEPROM.clear();
    m_attenuationEEPROM.clear();
    m_softMuteStepEEPROM.clear();
  }
};

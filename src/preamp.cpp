#include "preamp.h"

#include <Wire.h>

#include "settings.h"
#include "pins.h"

const int input_mapping[] = { 1, 2, 0, 3 };

Preamp::Preamp()
{

}

bool Preamp::begin()
{
  Wire.begin();
  m_controller = TDA7419(&Wire);

  m_inputSettingsEEPROM.begin("pa_in", false);
  m_loudnessSettingsEEPROM.begin("pa_loud", false);
  m_volumeSettingsEEPROM.begin("pa_vol", false);
  m_trebleFilterEEPROM.begin("pa_tf", false);
  m_middleFilterEEPROM.begin("pa_mf", false);
  m_bassFilterEEPROM.begin("pa_bf", false);
  m_freqSettingsEEPROM.begin("pa_fs", false);
  m_attenuationEEPROM.begin("pa_a", false);

  loadFromMemory();

  m_controller.setInput(input_mapping[m_inputSettings.selected_input], m_inputSettings.gain, INPUT_AUTO_Z);
  m_controller.setInput2(INPUT2_SOURCE, INPUT2_GAIN, REAR_SPEAKER_SOURCE);
  m_controller.setVolume(m_volumeSettings.volume, VOLUME_SOFT_STEP);
  m_controller.setSoft(SOFT_MUTE, PIN_INFLUENCE_FOR_MUTE, SOFT_MUTE_TIME, SOFT_STEP_TIME, CLOCK_FAST_MODE);
  m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, REFERENCE_OUTPUT_SELECT);
  m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, MIDDLE_SOFT_STEP);
  m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, BASS_SOFT_STEP);
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
  m_controller.setAtt_LF(m_attenuation.att_lf, ATT_FRONT_LEFT_SOFT_STEP);
  m_controller.setAtt_RF(m_attenuation.att_rf, ATT_FRONT_RIGHT_SOFT_STEP);
  m_controller.setAtt_SUB(m_attenuation.att_sub, ATT_SUB_SOFT_STEP);
  m_controller.setMix_Gain_Eff(MIXING_TO_LEFT_FRONT, MIXING_TO_RIGHT_FRONT, MIXING_ENABLE,  SUBWOOFER_ENABLE, GAIN_EFFECT_HPF);
  m_controller.setSpektor(SA_FILTER_Q_FACTOR, SA_RESET_MODE, SA_SOURCE, SA_RUN, SA_RESET, SA_CLOCK_SOURCE, SA_COUPLING_MODE);

  return true;
}

void Preamp::setInput(int val)
{
  if (val < 0) val = 0;
  if (val > 3) val = 3;
  if (val == m_inputSettings.selected_input) return;

  m_inputSettings.selected_input = val;
  m_inputSettingsChanged = true;
  m_controller.setInput(input_mapping[m_inputSettings.selected_input], m_inputSettings.gain, INPUT_AUTO_Z);
}

void Preamp::rotateInput()
{
  m_inputSettings.selected_input++;

  if (m_inputSettings.selected_input > 3)
    m_inputSettings.selected_input = 0;

  m_inputSettingsChanged = true;
  m_controller.setInput(input_mapping[m_inputSettings.selected_input], m_inputSettings.gain, INPUT_AUTO_Z);
}

void Preamp::setInputGain(int val)
{
  if (val < 0) val = 0;
  if (val > 15) val = 15;
  if (val == m_inputSettings.gain) return;

  m_inputSettings.gain = val;
#ifndef POT4
  m_inputSettingsChanged = true;
#endif
  m_controller.setInput(input_mapping[m_inputSettings.selected_input], m_inputSettings.gain, INPUT_AUTO_Z);
}

void Preamp::incGain()
{
  if (m_inputSettings.gain >= 15) return;

  m_inputSettings.gain++;
#ifndef POT4
  m_inputSettingsChanged = true;
#endif
  m_controller.setInput(input_mapping[m_inputSettings.selected_input], m_inputSettings.gain, INPUT_AUTO_Z);
}

void Preamp::decGain()
{
  if (m_inputSettings.gain <= 0) return;

  m_inputSettings.gain--;
#ifndef POT4
  m_inputSettingsChanged = true;
#endif
  m_controller.setInput(input_mapping[m_inputSettings.selected_input], m_inputSettings.gain, INPUT_AUTO_Z);
}

void Preamp::setVolume(int val)
{
  if (val < -80) val = -80;
  if (val > 15) val = 15;
  if (val == m_volumeSettings.volume) return;

  m_volumeSettings.volume = val;
  m_volumeSettingsChanged = true;
  m_controller.setVolume(m_volumeSettings.volume, VOLUME_SOFT_STEP);
}

void Preamp::incVolume()
{
  if (m_volumeSettings.volume >= 15) return;
  
  m_volumeSettings.volume++;
  m_volumeSettingsChanged = true;
  m_controller.setVolume(m_volumeSettings.volume, VOLUME_SOFT_STEP);
}

void Preamp::decVolume()
{
  if (m_volumeSettings.volume <= -80) return;
  
  m_volumeSettings.volume--;
  m_volumeSettingsChanged = true;
  m_controller.setVolume(m_volumeSettings.volume, VOLUME_SOFT_STEP);
}

void Preamp::setTrebleGain(int val)
{
  if (val < -15) val = -15;
  if (val > 15) val = 15;
  if (val == m_trebleFilter.gain) return;

  m_trebleFilter.gain = val;
  // m_trebleFilterChanged = true;
  m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, REFERENCE_OUTPUT_SELECT);
}

void Preamp::setTrebleCenterFreq(int val)
{
  if (val < 0) val = 0;
  if (val > 3) val = 3;
  if (val == m_trebleFilter.center_freq) return;

  m_trebleFilter.center_freq = val;
  m_trebleFilterChanged = true;
  m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, REFERENCE_OUTPUT_SELECT);
}

void Preamp::incTrebleCenterFreq()
{
  if (m_trebleFilter.center_freq >= 3) return;

  m_trebleFilter.center_freq++;
  m_trebleFilterChanged = true;
  m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, REFERENCE_OUTPUT_SELECT);
}

void Preamp::decTrebleCenterFreq()
{
  if (m_trebleFilter.center_freq <= 0) return;

  m_trebleFilter.center_freq--;
  m_trebleFilterChanged = true;
  m_controller.setFilter_Treble(m_trebleFilter.gain, m_trebleFilter.center_freq, REFERENCE_OUTPUT_SELECT);
}

void Preamp::setMiddleGain(int val)
{
  if (val < -15) val = -15;
  if (val > 15) val = 15;
  if (val == m_middleFilter.gain) return;

  m_middleFilter.gain = val;
  // m_middleFilterChanged = true;
  m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, MIDDLE_SOFT_STEP);
}

void Preamp::setMiddleQFactor(int val)
{
  if (val < 0) val = 0;
  if (val > 3) val = 3;
  if (val == m_middleFilter.qfactor) return;

  m_middleFilter.qfactor = val;
  m_middleFilterChanged = true;
  m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, MIDDLE_SOFT_STEP);
}

void Preamp::incMiddleQFactor()
{
  if (m_middleFilter.qfactor >= 3) return;

  m_middleFilter.qfactor++;
  m_middleFilterChanged = true;
  m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, MIDDLE_SOFT_STEP);
}

void Preamp::decMiddleQFactor()
{
  if (m_middleFilter.qfactor <= 0) return;

  m_middleFilter.qfactor--;
  m_middleFilterChanged = true;
  m_controller.setFilter_Middle(m_middleFilter.gain, m_middleFilter.qfactor, MIDDLE_SOFT_STEP);
}

void Preamp::setBassGain(int val)
{
  if (val < -15) val = -15;
  if (val > 15) val = 15;
  if (val == m_bassFilter.gain) return;

  m_bassFilter.gain = val;
  // m_bassFilterChanged = true;
  m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, BASS_SOFT_STEP);
}

void Preamp::setBassQFactor(int val)
{
  if (val < 0) val = 0;
  if (val > 3) val = 3;
  if (val == m_bassFilter.qfactor) return;

  m_bassFilter.qfactor = val;
  m_bassFilterChanged = true;
  m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, BASS_SOFT_STEP);
}

void Preamp::incBassQFactor()
{
  if (m_bassFilter.qfactor >= 3) return;

  m_bassFilter.qfactor++;
  m_bassFilterChanged = true;
  m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, BASS_SOFT_STEP);
}

void Preamp::decBassQFactor()
{
  if (m_bassFilter.qfactor <= 0) return;

  m_bassFilter.qfactor--;
  m_bassFilterChanged = true;
  m_controller.setFilter_Bass(m_bassFilter.gain, m_bassFilter.qfactor, BASS_SOFT_STEP);
}

void Preamp::setLoudnessAttenuation(int val)
{
  if (val < 0) val = 0;
  if (val > 15) val = 15;
  if (val == m_loudnessSettings.attenuation) return;

  m_loudnessSettings.attenuation = val;
  m_loudnessSettingsChanged = true;
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
}

void Preamp::incLoudnessAttenuation()
{
  if (m_loudnessSettings.attenuation >= 15) return;

  m_loudnessSettings.attenuation++;
  m_loudnessSettingsChanged = true;
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
}

void Preamp::decLoudnessAttenuation()
{
  if (m_loudnessSettings.attenuation <= 0) return;

  m_loudnessSettings.attenuation--;
  m_loudnessSettingsChanged = true;
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
}

void Preamp::setLoudnessCenterFreq(int val)
{
  if (val < 0) val = 0;
  if (val > 3) val = 3;
  if (val == m_loudnessSettings.center_freq) return;

  m_loudnessSettings.center_freq = val;
  m_loudnessSettingsChanged = true;
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
}

void Preamp::incLoudnessCenterFreq()
{
  if (m_loudnessSettings.center_freq >= 3) return;

  m_loudnessSettings.center_freq++;
  m_loudnessSettingsChanged = true;
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
}

void Preamp::decLoudnessCenterFreq()
{
  if (m_loudnessSettings.center_freq <= 0) return;

  m_loudnessSettings.center_freq--;
  m_loudnessSettingsChanged = true;
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
}

void Preamp::setLoudnessHighBoost(int val)
{
  if (val < 0) val = 0;
  if (val > 1) val = 1;
  if (val == !m_loudnessSettings.high_boost) return;

  m_loudnessSettings.high_boost = !val;
  m_loudnessSettingsChanged = true;
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
}

void Preamp::switchLoudnessHighBoost()
{
  m_loudnessSettings.high_boost = !m_loudnessSettings.high_boost;
  m_loudnessSettingsChanged = true;
  m_controller.setAtt_loudness(m_loudnessSettings.attenuation, m_loudnessSettings.center_freq, m_loudnessSettings.high_boost, LAUDNESS_SOFT_STEP);
}

void Preamp::setBassCenterFreq(int val)
{
  if (val < 0) val = 0;
  if (val > 3) val = 3;
  if (val == m_freqSettings.bass_center_freq) return;

  m_freqSettings.bass_center_freq = val;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::incBassCenterFreq()
{
  if (m_freqSettings.bass_center_freq >= 3) return;

  m_freqSettings.bass_center_freq++;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::decBassCenterFreq()
{
  if (m_freqSettings.bass_center_freq <= 0) return;

  m_freqSettings.bass_center_freq--;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::setMiddleCenterFreq(int val)
{
  if (val < 0) val = 0;
  if (val > 3) val = 3;
  if (val == m_freqSettings.mid_center_freq) return;

  m_freqSettings.mid_center_freq = val;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::incMiddleCenterFreq()
{
  if (m_freqSettings.mid_center_freq >= 3) return;

  m_freqSettings.mid_center_freq++;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::decMiddleCenterFreq()
{
  if (m_freqSettings.mid_center_freq <= 0) return;

  m_freqSettings.mid_center_freq--;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::setSubCutoffFreq(int val)
{
  if (val < 0) val = 0;
  if (val > 3) val = 3;
  if (val == m_freqSettings.sub_cutoff_freq) return;

  m_freqSettings.sub_cutoff_freq = val;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::incSubCutoffFreq()
{
  if (m_freqSettings.sub_cutoff_freq >= 3) return;

  m_freqSettings.sub_cutoff_freq++;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::decSubCutoffFreq()
{
  if (m_freqSettings.sub_cutoff_freq <= 0) return;

  m_freqSettings.sub_cutoff_freq--;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::setBassDC(int val)
{
  if (val < 0) val = 0;
  if (val > 1) val = 1;
  if (val == !m_freqSettings.bass_dc) return;

  m_freqSettings.bass_dc = !val;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::switchBassDC()
{
  m_freqSettings.bass_dc = !m_freqSettings.bass_dc;
  m_freqSettingsChanged = true;
  m_controller.setSub_M_B(m_freqSettings.sub_cutoff_freq, m_freqSettings.mid_center_freq, m_freqSettings.bass_center_freq, m_freqSettings.bass_dc, SMOOTHING_FILTER);
}

void Preamp::setLeftAttenuation(int val)
{
  if (val > 15) val = 15;
  if (val < -80) val = -80;
  if (val == m_attenuation.att_lf) return;

  m_attenuation.att_lf = val;
  m_attenuationChanged = true;
  m_controller.setAtt_LF(m_attenuation.att_lf, ATT_FRONT_LEFT_SOFT_STEP);
}

void Preamp::incLeftAttenuation()
{
  if (m_attenuation.att_lf >= 15) return;

  m_attenuation.att_lf++;
  m_attenuationChanged = true;
  m_controller.setAtt_LF(m_attenuation.att_lf, ATT_FRONT_LEFT_SOFT_STEP);
}

void Preamp::decLeftAttenuation()
{
  if (m_attenuation.att_lf <= -80) return;

  m_attenuation.att_lf--;
  m_attenuationChanged = true;
  m_controller.setAtt_LF(m_attenuation.att_lf, ATT_FRONT_LEFT_SOFT_STEP);
}

void Preamp::setRightAttenuation(int val)
{
  if (val > 15) val = 15;
  if (val < -80) val = -80;
  if (val == m_attenuation.att_rf) return;

  m_attenuation.att_rf = val;
  m_attenuationChanged = true;
  m_controller.setAtt_RF(m_attenuation.att_rf, ATT_FRONT_RIGHT_SOFT_STEP);
}

void Preamp::incRightAttenuation()
{
  if (m_attenuation.att_rf >= 15) return;

  m_attenuation.att_rf++;
  m_attenuationChanged = true;
  m_controller.setAtt_RF(m_attenuation.att_rf, ATT_FRONT_RIGHT_SOFT_STEP);
}

void Preamp::decRightAttenuation()
{
  if (m_attenuation.att_rf <= -80) return;

  m_attenuation.att_rf--;
  m_attenuationChanged = true;
  m_controller.setAtt_RF(m_attenuation.att_rf, ATT_FRONT_RIGHT_SOFT_STEP);
}

void Preamp::setSubAttenuation(int val)
{
  if (val > 15) val = 15;
  if (val < -80) val = -80;
  if (val == m_attenuation.att_sub) return;

  m_attenuation.att_sub = val;
  m_attenuationChanged = true;
  m_controller.setAtt_SUB(m_attenuation.att_sub, ATT_SUB_SOFT_STEP);
}

void Preamp::incSubAttenuation()
{
  if (m_attenuation.att_sub >= 15) return;

  m_attenuation.att_sub++;
  m_attenuationChanged = true;
  m_controller.setAtt_SUB(m_attenuation.att_sub, ATT_SUB_SOFT_STEP);
}

void Preamp::decSubAttenuation()
{
  if (m_attenuation.att_sub <= -80) return;

  m_attenuation.att_sub--;
  m_attenuationChanged = true;
  m_controller.setAtt_SUB(m_attenuation.att_sub, ATT_SUB_SOFT_STEP);
}

void Preamp::mute() 
{ 
  m_controller.setInput(5, 0, INPUT_AUTO_Z); 
}

void Preamp::unmute()
{
  m_controller.setInput(input_mapping[m_inputSettings.selected_input], m_inputSettings.gain, INPUT_AUTO_Z);
}

void Preamp::saveInputSettings()
{
  m_inputSettingsEEPROM.putInt("sel_inp", m_inputSettings.selected_input);
#ifndef POT4
  m_inputSettingsEEPROM.putInt("gain", m_inputSettings.gain);
#endif
}

void Preamp::saveLoudnessSettings()
{
  m_loudnessSettingsEEPROM.putInt("att", m_loudnessSettings.attenuation);
  m_loudnessSettingsEEPROM.putInt("cf", m_loudnessSettings.center_freq);
  m_loudnessSettingsEEPROM.putInt("hb", m_loudnessSettings.high_boost);
}

void Preamp::saveVolume()
{
  m_volumeSettingsEEPROM.putInt("vol", m_volumeSettings.volume);
}

void Preamp::saveTrebleSettings()
{
  // m_trebleFilterEEPROM.putInt("gain", m_trebleFilter.gain);
  m_trebleFilterEEPROM.putInt("cf", m_trebleFilter.center_freq);
}

void Preamp::saveMiddleSettings()
{
  // m_middleFilterEEPROM.putInt("gain", m_middleFilter.gain);
  m_middleFilterEEPROM.putInt("qf", m_middleFilter.qfactor);
}

void Preamp::saveBassSettings()
{
  // m_bassFilterEEPROM.putInt("gain", m_bassFilter.gain);
  m_bassFilterEEPROM.putInt("qf", m_bassFilter.qfactor);
}

void Preamp::saveFreqSettings()
{
  m_freqSettingsEEPROM.putInt("scf", m_freqSettings.sub_cutoff_freq);
  m_freqSettingsEEPROM.putInt("mcf", m_freqSettings.mid_center_freq);
  m_freqSettingsEEPROM.putInt("bcf", m_freqSettings.bass_center_freq);
  m_freqSettingsEEPROM.putInt("bdc", m_freqSettings.bass_dc);
}

void Preamp::saveAttenuationSettings()
{
  m_attenuationEEPROM.putInt("att_lf", m_attenuation.att_lf);
  m_attenuationEEPROM.putInt("att_rf", m_attenuation.att_rf);
  m_attenuationEEPROM.putInt("att_sub", m_attenuation.att_sub);
}

void Preamp::saveChanged()
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
}

void Preamp::loadFromMemory()
{
  m_inputSettings.selected_input = m_inputSettingsEEPROM.getInt("sel_inp", m_inputSettings.selected_input);
#ifndef POT4
  m_inputSettings.gain = m_inputSettingsEEPROM.getInt("gain", m_inputSettings.gain);
#endif

  m_loudnessSettings.attenuation = m_loudnessSettingsEEPROM.getInt("att", m_loudnessSettings.attenuation);
  m_loudnessSettings.center_freq = m_loudnessSettingsEEPROM.getInt("cf", m_loudnessSettings.center_freq);
  m_loudnessSettings.high_boost = m_loudnessSettingsEEPROM.getInt("hb", m_loudnessSettings.high_boost);

  m_volumeSettings.volume = m_volumeSettingsEEPROM.getInt("vol", m_volumeSettings.volume);

  // m_trebleFilter.gain = m_trebleFilterEEPROM.getInt("gain", m_trebleFilter.gain);
  m_trebleFilter.center_freq = m_trebleFilterEEPROM.getInt("cf", m_trebleFilter.center_freq);

  // m_middleFilter.gain = m_middleFilterEEPROM.getInt("gain", m_middleFilter.gain);
  m_middleFilter.qfactor = m_middleFilterEEPROM.getInt("qf", m_middleFilter.qfactor);

  // m_bassFilter.gain = m_bassFilterEEPROM.getInt("gain", m_bassFilter.gain);
  m_bassFilter.qfactor = m_bassFilterEEPROM.getInt("qf", m_bassFilter.qfactor);

  m_freqSettings.sub_cutoff_freq = m_freqSettingsEEPROM.getInt("scf", m_freqSettings.sub_cutoff_freq);
  m_freqSettings.mid_center_freq = m_freqSettingsEEPROM.getInt("mcf", m_freqSettings.mid_center_freq);
  m_freqSettings.bass_center_freq = m_freqSettingsEEPROM.getInt("bcf", m_freqSettings.bass_center_freq);
  m_freqSettings.bass_dc = m_freqSettingsEEPROM.getInt("bdc", m_freqSettings.bass_dc);

  m_attenuation.att_lf = m_attenuationEEPROM.getInt("att_lf", m_attenuation.att_lf);
  m_attenuation.att_rf = m_attenuationEEPROM.getInt("att_rf", m_attenuation.att_rf);
  m_attenuation.att_sub = m_attenuationEEPROM.getInt("att_sub", m_attenuation.att_sub);
}

void Preamp::resetMemory()
{
  m_inputSettingsEEPROM.clear();
  m_loudnessSettingsEEPROM.clear();
  m_volumeSettingsEEPROM.clear();
  m_trebleFilterEEPROM.clear();
  m_middleFilterEEPROM.clear();
  m_bassFilterEEPROM.clear();
  m_freqSettingsEEPROM.clear();
  m_attenuationEEPROM.clear();
}

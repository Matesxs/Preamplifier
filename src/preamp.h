#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "TDA7419.h"

// Input
#define INPUT_AUTO_Z 1              // 0 - 1 (on, off)

// Input2
#define INPUT2_SOURCE 5             // 0 - 5 (5 - mute)
#define INPUT2_GAIN 0               // 0 - 15
#define REAR_SPEAKER_SOURCE 0       // 0 - 1 (main, secondary)

// Soft-mute/clock generator
#define PIN_INFLUENCE_FOR_MUTE 1    // 0 - 1 (PIN and IIC, IIC)
#define CLOCK_FAST_MODE 0           // 0 - 1 (on, off)

// Treble Filter
#define REFERENCE_OUTPUT_SELECT 1   // 0 - 1 (external, internal)

// Freq settings
#define SMOOTHING_FILTER 0          // 0 - 1 (on, off)

// Mixing / gain effect
#define MIXING_TO_LEFT_FRONT 1      // 0 - 1 (on, off)
#define MIXING_TO_RIGHT_FRONT 1     // 0 - 1 (on, off)
#define MIXING_ENABLE 1             // 0 - 1 (on, off)
#define SUBWOOFER_ENABLE 0          // 0 - 1 (on, off)
#define GAIN_EFFECT_HPF 0           // 0 - 10 (4, 6 ... 22, 0)

// Spectrum analyzer / clock source / AC mode
#define SA_FILTER_Q_FACTOR 0        // 0 - 1 (3.5, 1.75)
#define SA_RESET_MODE 0             // 0 - 1 (IIC, Auto)
#define SA_SOURCE 1                 // 0 - 1 (Bass, In Gain)
#define SA_RUN 0                    // 0 - 1 (on, off)
#define SA_RESET 0                  // 0 - 1 (on, off)
#define SA_CLOCK_SOURCE 0           // 0 - 1 (internal, external)
#define SA_COUPLING_MODE 0          // 0 - 3 (DC Coupling (without HPF), AC coupling after In gain, DC Coupling (with HPF), AC coupling after Bass)

class Preamp
{
  typedef struct
  {
    int selected_input = 0;         // 0 - 3
    int gain = 0;                   // 0 - 15
  } InputSettings;

  typedef struct
  {
    int attenuation = 0;            // 0 - 15
    int center_freq = 0;            // 0 - 3 (flat, 400Hz, 800Hz, 2400Hz)
    int high_boost = 1;             // 0 - 1 (on, off)
    int soft_step = 1;              // 0 - 1 (on, off)
  } LoudnessSettings;

  typedef struct
  {
    int volume = 0;                 // -80 - 15
    int soft_step = 1;              // 0 - 1 (on, off)
  } VolumeSettings;

  typedef struct
  {
    int gain = 0;                   // -15 - 15
    int center_freq = 0;            // 0 - 3 (10kHz, 12.5kHz, 15kHz, 17.5kHz)
  } TrebleFilter;

  typedef struct
  {
    int gain = 0;                   // -15 - 15
    int qfactor = 0;                // 0 - 3 (0.5, 0.75, 1, 1.25)
    int soft_step = 1;              // 0 - 1 (on, off)
  } MiddleFilter;

  typedef struct
  {
    int gain = 0;                   // -15 - 15
    int qfactor = 0;                // 0 - 3 (1, 1.25, 1.5, 2)
    int soft_step = 1;              // 0 - 1 (on, off)
  } BassFilter;

  typedef struct
  {
    int sub_cutoff_freq = 0;        // 0 - 3 (flat, 80Hz, 120Hz, 160Hz)
    int mid_center_freq = 0;        // 0 - 3 (500Hz, 1000Hz, 1500Hz, 2500Hz)
    int bass_center_freq = 0;       // 0 - 3 (60Hz, 80Hz, 100Hz, 200Hz)
    int bass_dc = 1;                // 0 - 1 (on, off)
  } FreqSettings;

  typedef struct
  {
    int att_lf = 0;                 // -80 - 15
    int soft_steps_lf = 1;          // 0 - 1 (on, off)
    int att_rf = 0;                 // -80 - 15
    int soft_steps_rf = 1;          // 0 - 1 (on, off)
    int att_sub = 0;                // -80 - 15
    int soft_steps_sub = 1;         // 0 - 1 (on, off)
  } Attenuation;

  typedef struct
  {
    int soft_mute = 1;              // 0 - 1 (on, off)
    int soft_mute_time = 0;         // 0 - 2 (0.48ms, 0.96ms, 123ms)
    int soft_step_time = 0;         // 0 - 7 (0.160ms, 0.321ms, 0.642ms, 1.28ms, 2.56ms, 5.12ms, 10.24ms, 20.48ms)
  } SoftMuteStep;

public:
  Preamp();
  bool begin();

  // Input settings
  int getInput() const { return m_inputSettings.selected_input; }
  void setInput(int val);
  void rotateInput();
  void incInput();
  void decInput();

  int getGain() const { return m_inputSettings.gain; }
  void setInputGain(int val);
  void incGain();
  void decGain();

  // Laudness settings
  int getLoudnessAttenuation() const { return m_loudnessSettings.attenuation; }
  void setLoudnessAttenuation(int val);
  void incLoudnessAttenuation();
  void decLoudnessAttenuation();

  int getLoudnessCenterFreq() const { return m_loudnessSettings.center_freq; }
  void setLoudnessCenterFreq(int val);
  void incLoudnessCenterFreq();
  void decLoudnessCenterFreq();

  int getLoudnessHighBoost() const { return !m_loudnessSettings.high_boost; }
  void setLoudnessHighBoost(int val);
  void switchLoudnessHighBoost();

  int getLoudnessSoftStep() const { return !m_loudnessSettings.soft_step; }
  void setLoudnessSoftStep(int val);
  void switchLoudnessSoftStep();

  // Volume settings
  int getVolume() const { return m_volumeSettings.volume; }
  void setVolume(int val);
  void incVolume();
  void decVolume();

  int getVolumeSoftStep() const { return !m_volumeSettings.soft_step; }
  void setVolumeSoftStep(int val);
  void switchVolumeSoftStep();

  // Treble settings
  int getTrebleGain() const { return m_trebleFilter.gain; }
  void setTrebleGain(int val);

  int getTrebleCenterFreq() const { return m_trebleFilter.center_freq; }
  void setTrebleCenterFreq(int val);
  void incTrebleCenterFreq();
  void decTrebleCenterFreq();

  // Middle settings
  int getMiddleGain() const { return m_middleFilter.gain; }
  void setMiddleGain(int val);

  int getMiddleQFactor() const { return m_middleFilter.qfactor; }
  void setMiddleQFactor(int val);
  void incMiddleQFactor();
  void decMiddleQFactor();

  int getMiddleSoftStep() const { return !m_middleFilter.soft_step; }
  void setMiddleSoftStep(int val);
  void switchMiddleSoftStep();

  // Bass settings
  int getBassGain() const { return m_bassFilter.gain; }
  void setBassGain(int val);

  int getBassQFactor() const { return m_bassFilter.qfactor; }
  void setBassQFactor(int val);
  void incBassQFactor();
  void decBassQFactor();

  int getBassSoftStep() const { return !m_bassFilter.soft_step; }
  void setBassSoftStep(int val);
  void switchBassSoftStep();

  // Freq settings
  int getBassCenterFreq() const { return m_freqSettings.bass_center_freq; }
  void setBassCenterFreq(int val);
  void incBassCenterFreq();
  void decBassCenterFreq();

  int getBassDC() const { return !m_freqSettings.bass_dc; }
  void setBassDC(int val);
  void switchBassDC();

  int getMiddleCenterFreq() const { return m_freqSettings.mid_center_freq; }
  void setMiddleCenterFreq(int val);
  void incMiddleCenterFreq();
  void decMiddleCenterFreq();

  int getSubCutoffFreq() const { return m_freqSettings.sub_cutoff_freq; }
  void setSubCutoffFreq(int val);
  void incSubCutoffFreq();
  void decSubCutoffFreq();

  // Attenuations
  int getLeftAttenuation() const { return m_attenuation.att_lf; }
  void setLeftAttenuation(int val);
  void incLeftAttenuation();
  void decLeftAttenuation();

  int getRightAttenuation() const { return m_attenuation.att_rf; }
  void setRightAttenuation(int val);
  void incRightAttenuation();
  void decRightAttenuation();

  int getSubAttenuation() const { return m_attenuation.att_sub; }
  void setSubAttenuation(int val);
  void incSubAttenuation();
  void decSubAttenuation();

  int getLeftSoftSteps() const { return !m_attenuation.soft_steps_lf; }
  void setLeftSoftSteps(int val);
  void switchLeftSoftSteps();

  int getRightSoftSteps() const { return !m_attenuation.soft_steps_rf; }
  void setRightSoftSteps(int val);
  void switchRightSoftSteps();

  int getSubSoftSteps() const { return !m_attenuation.soft_steps_sub; }
  void setSubSoftSteps(int val);
  void switchSubSoftSteps();

  // SoftMuteStep settings
  int getSoftStepsTime() const { return m_softMuteStep.soft_step_time; }
  void setSoftStepsTime(int val);
  void incSoftStepsTime();
  void decSoftStepsTime();

  void setSoftMute(int val);
  void switchSoftMute();

  int getSoftMuteTime() const { return m_softMuteStep.soft_mute_time; }
  void setSoftMuteTime(int val);
  void incSoftMuteTime();
  void decSoftMuteTime();

  // Mic
  void mute();
  void unmute();

  // Saving
  void saveInputSettings();
  void saveLoudnessSettings();
  void saveVolume();
  void saveTrebleSettings();
  void saveMiddleSettings();
  void saveBassSettings();
  void saveFreqSettings();
  void saveAttenuationSettings();
  void saveSoftMuteStep();
  void saveChanged();
  void loadFromMemory();
  void resetMemory();

private:
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
};

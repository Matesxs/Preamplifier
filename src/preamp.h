#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "TDA7419.h"

// Input
#define INPUT_AUTO_Z 0              // 0 - 1 (on, off)

// Input2
#define INPUT2_SOURCE 5             // 0 - 5 (5 - mute)
#define INPUT2_GAIN 0               // 0 - 15
#define REAR_SPEAKER_SOURCE 0       // 0 - 1 (main, secondary)

// Laudness
#define LAUDNESS_SOFT_STEP 0        // 0 - 1 (on, off)

// Soft-mute/clock generator
#define SOFT_MUTE 0                 // 0 - 1 (on, off)
#define PIN_INFLUENCE_FOR_MUTE 0    // 0 - 1 (PIN and IIC, IIC)
#define SOFT_MUTE_TIME 0            // 0 - 2 (0.48ms - 123ms)
#define SOFT_STEP_TIME 0            // 0 - 7 (0.160ms - 20.48ms)
#define CLOCK_FAST_MODE 0           // 0 - 1 (on, off)

// Volume
#define VOLUME_SOFT_STEP 0          // 0 - 1 (on, off)

// Treble Filter
#define REFERENCE_OUTPUT_SELECT 0   // 0 - 1 (external, internal)

// Middle Filter
#define MIDDLE_SOFT_STEP 0          // 0 - 1 (on, off)

// Bass Filter
#define BASS_SOFT_STEP 0            // 0 - 1 (on, off)

// Freq settings
#define SMOOTHING_FILTER 0          // 0 - 1 (on, off)

// Attenuation settings
#define ATT_FRONT_LEFT_SOFT_STEP 0  // 0 - 1 (on, off)
#define ATT_FRONT_RIGHT_SOFT_STEP 0 // 0 - 1 (on, off)
#define ATT_SUB_SOFT_STEP 0         // 0 - 1 (on, off)

// Mixing / gain effect
#define MIXING_TO_LEFT_FRONT 0      // 0 - 1 (on, off)
#define MIXING_TO_RIGHT_FRONT 0     // 0 - 1 (on, off)
#define MIXING_ENABLE 0             // 0 - 1 (on, off)
#define SUBWOOFER_ENABLE 0          // 0 - 1 (on, off)
#define GAIN_EFFECT_HPF 0           // 0 - 10 (4, 6 ... 22, 0)

// Spectrum analyzer / clock source / AC mode
#define SA_FILTER_Q_FACTOR 0        // 0 - 1 (3.5, 1.75)
#define RESET_MODE 0                // 0 - 1 (IIC, Auto)
#define SA_SOURCE 0                 // 0 - 1 (Bass, In Gain)
#define SA_RUN 0                    // 0 - 1 (on, off)
#define RESET 0                     // 0 - 1 (on, off)
#define CLOCK_SOURCE 0              // 0 - 1 (internal, external)
#define COUPLING_MODE 0             // 0 - 3 (DC Coupling (without HPF), AC coupling after In gain, DC Coupling (with HPF), AC coupling after Bass)

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
    int high_boost = 0;             // 0 - 1 (on, off)
  } LoudnessSettings;

  typedef struct
  {
    int volume = 0;                 // -80 - 15
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
  } MiddleFilter;

  typedef struct
  {
    int gain = 0;                   // -15 - 15
    int qfactor = 0;                // 0 - 3 (1, 1.25, 1.5, 2)
  } BassFilter;

  typedef struct
  {
    int sub_cutoff_freq = 0;        // 0 - 3 (flat, 80Hz, 120Hz, 160Hz)
    int mid_center_freq = 0;        // 0 - 3 (500Hz, 1000Hz, 1500Hz, 2500Hz)
    int bass_center_freq = 0;       // 0 - 3 (60Hz, 80Hz, 100Hz, 200Hz)
    int bass_dc = 0;                // 0 - 1 (on, off)
  } FreqSettings;

  typedef struct
  {
    int att_lf = 0;                 // -80 - 15
    int att_rf = 0;                 // -80 - 15
    int att_sub = 0;                // -80 - 15
  } Attenuation;  

public:
  Preamp();
  bool begin();

  // Input settings
  int getInput() const { return m_inputSettings.selected_input; }
  void setInput(int val);
  void rotateInput();

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

  // Volume settings
  int getVolume() const { return m_volumeSettings.volume; }
  void setVolume(int val);
  void incVolume();
  void decVolume();

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

  // Bass settings
  int getBassGain() const { return m_bassFilter.gain; }
  void setBassGain(int val);

  int getBassQFactor() const { return m_bassFilter.qfactor; }
  void setBassQFactor(int val);
  void incBassQFactor();
  void decBassQFactor();

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
  void saveChanged();
  void loadFromMemory();

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

  Preferences m_inputSettingsEEPROM;
  Preferences m_loudnessSettingsEEPROM;
  Preferences m_volumeSettingsEEPROM;
  Preferences m_trebleFilterEEPROM;
  Preferences m_middleFilterEEPROM;
  Preferences m_bassFilterEEPROM;
  Preferences m_freqSettingsEEPROM;
  Preferences m_attenuationEEPROM;

  TDA7419 m_controller;
};

#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "TDA7419.h"

// Input
#define PREAMP_INPUT_AUTO_Z 1              // 0 - 1 (on, off)

// Input2
#define PREAMP_INPUT2_SOURCE 5             // 0 - 5 (5 - mute)
#define PREAMP_INPUT2_GAIN 0               // 0 - 15
#define PREAMP_REAR_SPEAKER_SOURCE 0       // 0 - 1 (main, secondary)

// Soft-mute/clock generator
#define PREAMP_PIN_INFLUENCE_FOR_MUTE 1    // 0 - 1 (PIN and IIC, IIC)
#define PREAMP_CLOCK_FAST_MODE 0           // 0 - 1 (on, off)

// Treble Filter
#define PREAMP_REFERENCE_OUTPUT_SELECT 1   // 0 - 1 (external, internal)

// Freq settings
#define PREAMP_SMOOTHING_FILTER 0          // 0 - 1 (on, off)

// Mixing / gain effect
#define PREAMP_MIXING_TO_LEFT_FRONT 1      // 0 - 1 (on, off)
#define PREAMP_MIXING_TO_RIGHT_FRONT 1     // 0 - 1 (on, off)
#define PREAMP_MIXING_ENABLE 1             // 0 - 1 (on, off)
#define PREAMP_SUBWOOFER_ENABLE 0          // 0 - 1 (on, off)
#define PREAMP_GAIN_EFFECT_HPF 0           // 0 - 10 (4, 6 ... 22, 0)

// Spectrum analyzer / clock source / AC mode
#define PREAMP_SA_FILTER_Q_FACTOR 1        // 0 - 1 (3.5, 1.75)
#define PREAMP_SA_RESET_MODE 1             // 0 - 1 (IIC, Auto)
#define PREAMP_SA_SOURCE 1                 // 0 - 1 (Bass, In Gain)
#define PREAMP_SA_RUN 0                    // 0 - 1 (on, off)
#define PREAMP_SA_RESET 0                  // 0 - 1 (on, off)
#define PREAMP_SA_CLOCK_SOURCE 0           // 0 - 1 (internal, external)
#define PREAMP_SA_COUPLING_MODE 0          // 0 - 3 (DC Coupling (without HPF), AC coupling after In gain, DC Coupling (with HPF), AC coupling after Bass)

namespace Preamp
{
  typedef struct
  {
    int selected_input = 0;         // 0 - 3
    int gain1 = 0;                  // 0 - 15
    int gain2 = 0;                  // 0 - 15
    int gain3 = 0;                  // 0 - 15
    int gain4 = 0;                  // 0 - 15
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

  void init();

  // Input settings
  int getInput();
  void setInput(int val);
  void rotateInput();
  void incInput();
  void decInput();

  int getInputGain();
  void setInputGain(int val);
  void incInputGain();
  void decInputGain();

  // Laudness settings
  int getLoudnessAttenuation();
  void setLoudnessAttenuation(int val);
  void incLoudnessAttenuation();
  void decLoudnessAttenuation();

  int getLoudnessCenterFreq();
  void setLoudnessCenterFreq(int val);
  void incLoudnessCenterFreq();
  void decLoudnessCenterFreq();

  int getLoudnessHighBoost();
  void setLoudnessHighBoost(int val);
  void switchLoudnessHighBoost();

  int getLoudnessSoftStep();
  void setLoudnessSoftStep(int val);
  void switchLoudnessSoftStep();

  // Volume settings
  int getVolume();
  void setVolume(int val);
  void incVolume();
  void decVolume();

  int getVolumeSoftStep();
  void setVolumeSoftStep(int val);
  void switchVolumeSoftStep();

  // Treble settings
  int getTrebleGain();
  void setTrebleGain(int val);
  void incTrebleGain();
  void decTrebleGain();

  int getTrebleCenterFreq();
  void setTrebleCenterFreq(int val);
  void incTrebleCenterFreq();
  void decTrebleCenterFreq();

  // Middle settings
  int getMiddleGain();
  void setMiddleGain(int val);
  void incMiddleGain();
  void decMiddleGain();

  int getMiddleQFactor();
  void setMiddleQFactor(int val);
  void incMiddleQFactor();
  void decMiddleQFactor();

  int getMiddleSoftStep();
  void setMiddleSoftStep(int val);
  void switchMiddleSoftStep();

  // Bass settings
  int getBassGain();
  void setBassGain(int val);
  void incBassGain();
  void decBassGain();

  int getBassQFactor();
  void setBassQFactor(int val);
  void incBassQFactor();
  void decBassQFactor();

  int getBassSoftStep();
  void setBassSoftStep(int val);
  void switchBassSoftStep();

  // Freq settings
  int getBassCenterFreq();
  void setBassCenterFreq(int val);
  void incBassCenterFreq();
  void decBassCenterFreq();

  int getBassDC();
  void setBassDC(int val);
  void switchBassDC();

  int getMiddleCenterFreq();
  void setMiddleCenterFreq(int val);
  void incMiddleCenterFreq();
  void decMiddleCenterFreq();

  int getSubCutoffFreq();
  void setSubCutoffFreq(int val);
  void incSubCutoffFreq();
  void decSubCutoffFreq();

  // Attenuations
  int getLeftAttenuation();
  void setLeftAttenuation(int val);
  void incLeftAttenuation();
  void decLeftAttenuation();

  int getRightAttenuation();
  void setRightAttenuation(int val);
  void incRightAttenuation();
  void decRightAttenuation();

  int getSubAttenuation();
  void setSubAttenuation(int val);
  void incSubAttenuation();
  void decSubAttenuation();

  int getLeftSoftSteps();
  void setLeftSoftSteps(int val);
  void switchLeftSoftSteps();

  int getRightSoftSteps();
  void setRightSoftSteps(int val);
  void switchRightSoftSteps();

  int getSubSoftSteps();
  void setSubSoftSteps(int val);
  void switchSubSoftSteps();

  // SoftMuteStep settings
  int getSoftStepsTime();
  void setSoftStepsTime(int val);
  void incSoftStepsTime();
  void decSoftStepsTime();

  void setSoftMute(int val);
  void switchSoftMute();

  int getSoftMuteTime();
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
};

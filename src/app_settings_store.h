#pragma once

#include <Arduino.h>

namespace AppSettingsStore
{
  typedef struct
  {
    bool enabledScreensaver;
    uint64_t screensaverDelay;
    bool spectrumAsScreensaver;

    bool clip_detection;
  } AppSettings;

  void init();
  void save();
  void reset();

  bool getScreensaverEnabled();
  void toggleScreenSaverEnabled();
  uint64_t getScreensaverDelay();
  void setScreensaverDelay(uint64_t ssdelay);
  bool getScreensaverSpectrum();
  void toggleScreensaverSpectrum();

  bool getClipDetection();
  void toggleClipDetection();
}

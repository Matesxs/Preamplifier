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

    bool overheat_detection_enabled;
    bool overheat_mute;
    bool overheat_flash_lights;

    uint8_t display_brightness;
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

  bool getOverheatDetection();
  void toggleOverheatDetection();
  bool getOverheatMute();
  void toggleOverheatMute();
  bool getOverheatFlashLights();
  void toggleOverheatFlashLights();

  uint8_t getDisplayBrightness();
  void incDisplayBrightness();
  void decDisplayBrightness();
}

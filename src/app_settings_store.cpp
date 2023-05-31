#include "app_settings_store.h"

#include <Preferences.h>

namespace AppSettingsStore
{
  Preferences settingsStore;
  volatile AppSettings settings;

  volatile bool screensaver_dirty = false;
  volatile bool clip_detection_dirty = false;

  void init()
  {
    settingsStore.begin("app_settings", false);

    settings.enabledScreensaver = settingsStore.getBool("ss_en", false);
    settings.screensaverDelay = settingsStore.getULong64("ss_del", 300);
    settings.spectrumAsScreensaver = settingsStore.getBool("ss_spec", false);

    settings.clip_detection = settingsStore.getBool("cd", false);
  }

  void save()
  {
    if (screensaver_dirty)
    {
      settingsStore.putBool("ss_en", settings.enabledScreensaver);
      settingsStore.putULong64("ss_del", settings.screensaverDelay);
      settingsStore.putBool("ss_spec", settings.spectrumAsScreensaver);

      screensaver_dirty = false;
    }

    if (clip_detection_dirty)
    {
      settingsStore.putBool("cd", settings.clip_detection);

      clip_detection_dirty = false;
    }
  }

  void reset()
  {
    settingsStore.clear();
  }


  bool getScreensaverEnabled()
  {
    return settings.enabledScreensaver;
  }

  void toggleScreenSaverEnabled()
  {
    settings.enabledScreensaver = !settings.enabledScreensaver;
    screensaver_dirty = true;
  }

  uint64_t getScreensaverDelay()
  {
    return settings.screensaverDelay;
  }

  void setScreensaverDelay(uint64_t ssdelay)
  {
    settings.screensaverDelay = ssdelay;
    screensaver_dirty = true;
  }

  bool getScreensaverSpectrum()
  {
    return settings.spectrumAsScreensaver;
  }

  void toggleScreensaverSpectrum()
  {
    settings.spectrumAsScreensaver = !settings.spectrumAsScreensaver;
    screensaver_dirty = true;
  }

  bool getClipDetection()
  {
    return settings.clip_detection;
  }

  void toggleClipDetection()
  {
    settings.clip_detection = !settings.clip_detection;
    clip_detection_dirty = true;
  }
}

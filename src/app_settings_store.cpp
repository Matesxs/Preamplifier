#include "app_settings_store.h"

#include <Preferences.h>

namespace AppSettingsStore
{
  Preferences settingsStore;
  volatile AppSettings settings;

  volatile bool screensaver_dirty = false;
  volatile bool clip_detection_dirty = false;
  volatile bool overheat_dirty = false;
  volatile bool display_dirty = false;

  void init()
  {
    settingsStore.begin("app_settings", false);

    settings.enabledScreensaver = settingsStore.getBool("ss_en", false);
    settings.screensaverDelay = settingsStore.getULong64("ss_del", 300);
    settings.spectrumAsScreensaver = settingsStore.getBool("ss_spec", false);

    settings.clip_detection = settingsStore.getBool("cd", false);

    settings.overheat_detection_enabled = settingsStore.getBool("ode", true);
    settings.overheat_mute = settingsStore.getBool("odm", true);
    settings.overheat_flash_lights = settingsStore.getBool("odfl", true);

    settings.display_brightness = settingsStore.getUChar("disp_br", 255);
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

    if (overheat_dirty)
    {
      settingsStore.putBool("ode", settings.overheat_detection_enabled);
      settingsStore.putBool("odm", settings.overheat_mute);
      settingsStore.putBool("odfl", settings.overheat_flash_lights);

      overheat_dirty = false;
    }

    if (display_dirty)
    {
      settingsStore.putUChar("disp_br", settings.display_brightness);

      display_dirty = false;
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

  void setScreensaverDelay(const uint64_t ssdelay)
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

  bool getOverheatDetection()
  {
    return settings.overheat_detection_enabled;
  }

  void toggleOverheatDetection()
  {
    settings.overheat_detection_enabled = !settings.overheat_detection_enabled;
    overheat_dirty = true;
  }

  bool getOverheatMute()
  {
    return settings.overheat_mute;
  }

  void toggleOverheatMute()
  {
    settings.overheat_mute = !settings.overheat_mute;
    overheat_dirty = true;
  }

  bool getOverheatFlashLights()
  {
    return settings.overheat_flash_lights;
  }

  void toggleOverheatFlashLights()
  {
    settings.overheat_flash_lights = !settings.overheat_flash_lights;
    overheat_dirty = true;
  }

  uint8_t getDisplayBrightness()
  {
    return settings.display_brightness;
  }

  void incDisplayBrightness()
  {
    if (settings.display_brightness < 255)
    {
      settings.display_brightness++;
      display_dirty = true;
    }
  }

  void decDisplayBrightness()
  {
    if (settings.display_brightness > 0)
    {
      settings.display_brightness--;
      display_dirty = true;
    }
  }
}

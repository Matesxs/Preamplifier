#include <Arduino.h>
#include <Wire.h>

#include <debug.h>

#include "preamp.h"
#include "io_handling/input_handlers.h"
#include "display/display_handler.h"
#include "display/ui_controller.h"
#include "settings.h"
#include "io_handling/temperature_reader.h"
#include "io_handling/potentiometer_handling.h"
#include "io_handling/spectrum_analyzer.h"
#include "ledstrip/led_strip_controller.h"
#include "app_settings_store.h"

[[noreturn]] void auto_saver_task(void*)
{
  while (true)
  {
    Preamp::saveChanged();
    AppSettingsStore::save();

    vTaskDelay(pdMS_TO_TICKS(SETTING_SAVE_INTERVAL_MS));
  }
}

void setup()
{
  Serial.begin(115200);

  AppSettingsStore::init();

#ifdef ENABLE_LED_STRIP
  LedStrip::begin();
#endif

  Wire.begin();

  Preamp::init(&Wire);

  prepare_display(AppSettingsStore::getDisplayBrightness());

  InputHandling::init();
  PotentiometerHandling::init();
  SpectrumAnalyzer::init(&Wire);
  vTaskDelay(pdMS_TO_TICKS(100));

  xTaskCreateUniversal(PotentiometerHandling::handle_potentiometer_task, "pot", 2048, nullptr, 1, nullptr, 1);
  xTaskCreateUniversal(display_draw_task, "disp", 2048, nullptr, 2, nullptr, 0);
  xTaskCreateUniversal(auto_saver_task, "save", 2048, nullptr, 1, nullptr, 0);
#ifdef ENABLE_TEMPERATURE_MONITORING
  xTaskCreateUniversal(TemperatureReader::temperature_task, "temp", 2048, nullptr, 1, nullptr, 0);
#endif
  xTaskCreateUniversal(SpectrumAnalyzer::updateSpectrumTask, "sa", 2048, nullptr, 1, nullptr, 1);

  DEBUG("Initialisation completed\n");
}

void loop()
{
  vTaskDelete(nullptr);
}

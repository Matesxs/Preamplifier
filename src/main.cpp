#include <Arduino.h>
#include <U8g2lib.h>
#include <Preferences.h>

#include "preamp.h"
#include "pins.h"
#include "global_objects.h"
#include "io_handling/input_handlers.h"
#include "display/display_handler.h"
#include "display/ui_controller.h"
#include "settings.h"
#include "debug.h"
#include "io_handling/temperature_reader.h"
#include "io_handling/potentiometer_handling.h"
#include "io_handling/spectrum_analyzer.h"
#include "connection/wifiHandling.h"
#include "connection/OTA.h"
#include "connection/dns.h"

// U8G2_SH1122_256X64_F_4W_SW_SPI display(U8G2_R0, DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN, DISPLAY_CS_PIN, DISPLAY_DATA_COMMAND_PIN, DISPLAY_RESET_PIN);
U8G2_SH1122_256X64_F_4W_HW_SPI display(U8G2_R0, DISPLAY_CS_PIN, DISPLAY_DATA_COMMAND_PIN, DISPLAY_RESET_PIN);
Preferences mainSettings;

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  Preamp::init();

  mainSettings.begin("main", false);
  delay(100);

  display.begin();
  display.setBusClock(20000000);
  // DEBUG("Display bus clock: %d\n", display.getBusClock());
  prepare_display();

  InputHandling::init();
  PotentiometerHandling::init();

  Wifi::begin(mainSettings.isKey("wifi_ssid") ? mainSettings.getString("wifi_ssid", "").c_str() : "", mainSettings.isKey("wifi_pass") ? mainSettings.getString("wifi_pass", "").c_str() : "");
  DNS::begin(mainSettings.isKey("dns_name") ? mainSettings.getString("dns_name", "").c_str() : "");
  OTA::begin();

#ifdef ENABLE_SPECTRUM_ANALYZER
  SpectrumAnalyzer::init();
#endif

  delay(100);

  xTaskCreateUniversal(PotentiometerHandling::handle_potentiometer_task, "pot", 2048, NULL, 1, NULL, 1);
  xTaskCreateUniversal(display_draw_task, "disp", 2048, NULL, 2, NULL, 0);
  xTaskCreateUniversal(auto_saver_task, "save", 2048, NULL, 1, NULL, 0);
#ifdef ENABLE_TEMPERATURE_MONITORING
  xTaskCreateUniversal(TemperatureReader::temperature_task, "temp", 2048, NULL, 1, NULL, 0);
#endif
#ifdef ENABLE_SPECTRUM_ANALYZER
  xTaskCreateUniversal(SpectrumAnalyzer::updateSpectrumTask, "sa", 2048, NULL, 1, NULL, 1);
#endif

  DEBUG("Initialisation completed\n");
}

void loop()
{
  vTaskDelete(NULL);
}

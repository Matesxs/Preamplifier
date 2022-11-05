#include <Arduino.h>
#include <U8g2lib.h>

#include "pins.h"
#include "global_objects.h"
#include "input_handlers.h"
#include "display_handler.h"
#include "ui_controller.h"
#include "settings.h"
#include "debug.h"
#include "temperature_reader.h"
#include "potentiometer_handling.h"
#include "spectrum_analyzer.h"

// U8G2_SH1122_256X64_F_4W_SW_SPI display(U8G2_R0, DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN, DISPLAY_CS_PIN, DISPLAY_DATA_COMMAND_PIN, DISPLAY_RESET_PIN);
U8G2_SH1122_256X64_F_4W_HW_SPI display(U8G2_R0, DISPLAY_CS_PIN, DISPLAY_DATA_COMMAND_PIN, DISPLAY_RESET_PIN);

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  display.begin();
  display.setBusClock(20000000);
  // DEBUG("Display bus clock: %d\n", display.getBusClock());
  prepare_display();

  InputHandling::init();
  PotentiometerHandling::init();

#ifdef ENABLE_SPECTRUM_ANALYZER
  SpectrumAnalyzer::init();
#endif

  Preamp::init();
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

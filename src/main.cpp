#include <Arduino.h>
#include <U8g2lib.h>

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

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  Preamp::init();

  prepare_display();

  InputHandling::init();
  PotentiometerHandling::init();
  SpectrumAnalyzer::init();
  delay(100);

  xTaskCreateUniversal(PotentiometerHandling::handle_potentiometer_task, "pot", 2048, NULL, 1, NULL, 1);
  xTaskCreateUniversal(display_draw_task, "disp", 2048, NULL, 2, NULL, 0);
  xTaskCreateUniversal(auto_saver_task, "save", 2048, NULL, 1, NULL, 0);
#ifdef ENABLE_TEMPERATURE_MONITORING
  xTaskCreateUniversal(TemperatureReader::temperature_task, "temp", 2048, NULL, 1, NULL, 0);
#endif
  xTaskCreateUniversal(SpectrumAnalyzer::updateSpectrumTask, "sa", 2048, NULL, 1, NULL, 1);

  DEBUG("Initialisation completed\n");
}

void loop()
{
  vTaskDelete(NULL);
}

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

Preamp preamp;

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  display.begin();
  display.setBusClock(20000000);
  DEBUG("Display bus clock: %d\n", display.getBusClock());
  prepare_display();

  pinMode(POT1, INPUT);
  pinMode(POT2, INPUT);
  pinMode(POT3, INPUT);
#ifdef POT4
  pinMode(POT4, INPUT);
#endif

#ifdef ENABLE_SPECTRUM_ANALYZER
  SpectrumAnalyzer::init();
#endif

  encoder_button.begin();
#ifdef CHANNEL_SW_BTN
  channel_switch_button.begin();
#endif
  attachInterrupt(ROT_A, update_encoder, FALLING);
  attachInterrupt(ROT_B, update_encoder, FALLING);

  // Init preamp
  preamp.begin();
  delay(100);

  xTaskCreateUniversal(encoder_task, "enc", 2048, NULL, 1, NULL, 1);
  xTaskCreateUniversal(handle_potentiometer_task, "pot", 2048, NULL, 1, NULL, 1);
  xTaskCreateUniversal(display_draw_task, "disp", 2048, NULL, 2, NULL, 0);
  xTaskCreateUniversal(auto_saver_task, "save", 2048, NULL, 1, NULL, 0);
  xTaskCreateUniversal(temperature_task, "temp", 2048, NULL, 1, NULL, 0);
#ifdef ENABLE_SPECTRUM_ANALYZER
  xTaskCreateUniversal(SpectrumAnalyzer::updateSpectrumTask, "sa", 2048, NULL, 1, NULL, 1);
#endif

  DEBUG("Initialisation completed\n");
}

void loop()
{
  vTaskDelete(NULL);
}

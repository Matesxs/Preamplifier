#pragma once

#include <Arduino.h>
#include <Wire.h>

namespace SpectrumAnalyzer
{
  #define SA_BANDS_NUMBER 7
  #define SA_RESET_TIME_MS 50
  #define SA_OUTPUT_DELAY_US 2
  #define SA_CLK_SIGNAL_HALF_PERIOD_US 10

  void init(TwoWire *wire);
  void updateSpectrum();
  void updateSpectrumTask(void*);

  extern uint16_t spectrum[SA_BANDS_NUMBER];
  extern uint16_t spectrumRaw[SA_BANDS_NUMBER];
};

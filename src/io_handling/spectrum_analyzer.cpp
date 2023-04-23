#include "spectrum_analyzer.h"

#include <Arduino.h>
#include <MovingAverage.h>

#include "pins.h"
#include "settings.h"
#include "debug.h"

namespace SpectrumAnalyzer
{
  uint16_t spectrum[SA_BANDS_NUMBER];
  uint16_t spectrumRaw[SA_BANDS_NUMBER];
  MovingAverage<uint16_t, SPECTRUM_AVG_BUFFER> filters[SA_BANDS_NUMBER];

  void init()
  {
    pinMode(SA_CLK, OUTPUT);
    digitalWrite(SA_CLK, HIGH);
    pinMode(SA_DATA, INPUT);
  }

  void updateSpectrum()
  {
    vTaskDelay(pdMS_TO_TICKS(SA_RESET_TIME_MS));

    portDISABLE_INTERRUPTS();
    for (int i = 0; i < SA_BANDS_NUMBER; i++)
    {
      digitalWrite(SA_CLK, LOW);
      delayMicroseconds(SA_CLK_SIGNAL_HALF_PERIOD_US);

      digitalWrite(SA_CLK, HIGH);
      delayMicroseconds(SA_CLK_SIGNAL_HALF_PERIOD_US);
    
      uint16_t val = analogRead(SA_DATA);
      spectrumRaw[i] = val;
      spectrum[i] = filters[i].add(val);
    }
    portENABLE_INTERRUPTS();

    vTaskDelay(pdMS_TO_TICKS(1));
  }

  void updateSpectrumTask(void*)
  {
    while (true)
    {
      updateSpectrum();

      // DEBUG("Spectrum: ");
      // for (int i = 0; i < SA_BANDS_NUMBER; i++)
      //   DEBUG("%d ", spectrum[i]);
      // DEBUG("\n");

      vTaskDelay(pdMS_TO_TICKS(SPECTRUM_UPDATE_INTERVAL_MS));
    }

    vTaskDelete(NULL);
  }
}

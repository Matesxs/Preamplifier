#include "spectrum_analyzer.h"

#include <Arduino.h>
#include <MovingAverage.h>
#include <ADS1100.h>

#include "pins.h"
#include "settings.h"

namespace SpectrumAnalyzer
{
  uint16_t spectrum[SA_BANDS_NUMBER];
  uint16_t spectrumRaw[SA_BANDS_NUMBER];
  MovingAverage<uint16_t, SPECTRUM_AVG_BUFFER> filters[SA_BANDS_NUMBER];
  ADS1100 ads(SPECTRUM_ADC_ADDRESS);

  void init(TwoWire *wire)
  {
    pinMode(SA_CLK, OUTPUT);
    digitalWrite(SA_CLK, HIGH);

    ads.begin(wire);
    ads.configure(ADS1100_CONVERSION_SINGLE,ADS1100_DATA_RATE_128SPS,ADS1100_GAIN_1X);
  }

  void updateSpectrum()
  {
    vTaskDelay(pdMS_TO_TICKS(SA_RESET_TIME_MS));

    // portDISABLE_INTERRUPTS();
    for (int i = 0; i < SA_BANDS_NUMBER; i++)
    {
      digitalWrite(SA_CLK, LOW);
      delayMicroseconds(SA_CLK_SIGNAL_HALF_PERIOD_US);

      digitalWrite(SA_CLK, HIGH);
      delayMicroseconds(SA_CLK_SIGNAL_HALF_PERIOD_US);

      ads.startSingleConversion();
      while (!ads.conversionDone())
        vTaskDelay(pdMS_TO_TICKS(1));

      const uint16_t val = ads.value;
      spectrumRaw[i] = val;
      spectrum[i] = filters[i].add(val);
    }
    // portENABLE_INTERRUPTS();

    vTaskDelay(pdMS_TO_TICKS(1));
  }

  [[noreturn]] void updateSpectrumTask(void*)
  {
    while (true)
    {
      updateSpectrum();

      // String spectrumString;
      // for (int i =0; i < SA_BANDS_NUMBER; i++)
      //   spectrumString += (String(spectrum[i]) + " ");
      // DEBUG("Spectrum: %s\r\n", spectrumString.c_str());

      vTaskDelay(pdMS_TO_TICKS(SPECTRUM_UPDATE_INTERVAL_MS));
    }
  }
}

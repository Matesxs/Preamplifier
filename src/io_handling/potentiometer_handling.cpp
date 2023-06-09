#include "potentiometer_handling.h"

#include <Arduino.h>
#include <MovingAverage.h>

#include "global_objects.h"
#include "preamp.h"
#include "settings.h"
#include "pins.h"
#include "display/ui_controller.h"

namespace PotentiometerHandling
{
  volatile uint16_t values[4];

#ifdef TREBLE_GAIN_POTENTIOMETER
  MovingAverage<uint16_t, POT_FILTER_SAMPLES> trebleFilter;
#endif
#ifdef MIDDLE_GAIN_POTENTIOMETER
  MovingAverage<uint16_t, POT_FILTER_SAMPLES> middleFilter;
#endif
#ifdef BASS_GAIN_POTENTIOMETER
  MovingAverage<uint16_t, POT_FILTER_SAMPLES> bassFilter;
#endif
#ifdef INPUT_GAIN_POTENTIOMETER
  MovingAverage<uint16_t, POT_FILTER_SAMPLES> inputGainFilter;
#endif

  void init()
  {
#ifdef TREBLE_GAIN_POTENTIOMETER
    pinMode(TREBLE_GAIN_POTENTIOMETER, INPUT);
#endif
#ifdef MIDDLE_GAIN_POTENTIOMETER
    pinMode(MIDDLE_GAIN_POTENTIOMETER, INPUT);
#endif
#ifdef BASS_GAIN_POTENTIOMETER
    pinMode(BASS_GAIN_POTENTIOMETER, INPUT);
#endif
#ifdef INPUT_GAIN_POTENTIOMETER
    pinMode(INPUT_GAIN_POTENTIOMETER, INPUT);
#endif
  }

  void handle_potentiometer_task(void *)
  {
#ifdef TREBLE_GAIN_POTENTIOMETER
    values[0] = trebleFilter.add(analogRead(TREBLE_GAIN_POTENTIOMETER));
    Preamp::setTrebleGain((int)map(values[0], POT_MIN_VAL, POT_MAX_VAL, -15, 15));
#endif

#ifdef MIDDLE_GAIN_POTENTIOMETER
    values[1] = middleFilter.add(analogRead(MIDDLE_GAIN_POTENTIOMETER));
    Preamp::setMiddleGain((int)map(values[1], POT_MIN_VAL, POT_MAX_VAL, -15, 15));
#endif

#ifdef BASS_GAIN_POTENTIOMETER
    values[2] = bassFilter.add(analogRead(BASS_GAIN_POTENTIOMETER));
    Preamp::setBassGain((int)map(values[2], POT_MIN_VAL, POT_MAX_VAL, -15, 15));
#endif

#ifdef INPUT_GAIN_POTENTIOMETER
    values[3] = inputGainFilter.add(analogRead(INPUT_GAIN_POTENTIOMETER));
    Preamp::setInputGain((int)map(values[3], POT_MIN_VAL, POT_MAX_VAL, 0, 15));
#endif

    uint16_t tmp;
    uint16_t diff;

    while (true)
    {
#ifdef TREBLE_GAIN_POTENTIOMETER
      tmp = trebleFilter.add(analogRead(TREBLE_GAIN_POTENTIOMETER));
      diff = (uint16_t)abs((int32_t)tmp - (int32_t)values[0]);
      if (diff > POTENTIOMETER_MIN_CHANGE)
      {
        values[0] = tmp;
        handle_controll(InputType::POT1_CH);
      }
#endif

#ifdef MIDDLE_GAIN_POTENTIOMETER
      tmp = middleFilter.add(analogRead(MIDDLE_GAIN_POTENTIOMETER));
      diff = (uint16_t)abs((int32_t)tmp - (int32_t)values[1]);
      if (diff > POTENTIOMETER_MIN_CHANGE)
      {
        values[1] = tmp;
        handle_controll(InputType::POT2_CH);
      }
#endif

#ifdef BASS_GAIN_POTENTIOMETER
      tmp = bassFilter.add(analogRead(BASS_GAIN_POTENTIOMETER));
      diff = (uint16_t)abs((int32_t)tmp - (int32_t)values[2]);
      if (diff > POTENTIOMETER_MIN_CHANGE)
      {
        values[2] = tmp;
        handle_controll(InputType::POT3_CH);
      }
#endif

#ifdef INPUT_GAIN_POTENTIOMETER
      tmp = inputGainFilter.add(analogRead(INPUT_GAIN_POTENTIOMETER));
      diff = (uint16_t)abs((int32_t)tmp - (int32_t)values[3]);
      if (diff > POTENTIOMETER_MIN_CHANGE)
      {
        values[3] = tmp;
        handle_controll(InputType::POT4_CH);
      }
#endif

      vTaskDelay(pdMS_TO_TICKS(POTENTIOMETER_PULLING_RATE_MS));
    }

    vTaskDelete(NULL);
  }
};

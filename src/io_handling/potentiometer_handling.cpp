#include "potentiometer_handling.h"

#include <Arduino.h>
#include <MovingAverage.h>

#include "global_objects.h"
#include "preamp.h"
#include "settings.h"
#include "pins.h"
#include "display/ui_controller.h"
#include "downmapper.h"

namespace PotentiometerHandling
{
  volatile int values[4];

#ifdef TREBLE_GAIN_POTENTIOMETER
  MovingAverage<uint16_t, POT_FILTER_SAMPLES> trebleFilter;
  Downmapper::LatchingDownmapper<int> trebleMapper(POT_MIN_VAL, POT_MAX_VAL, -15, 15, 0.6, -15);
#endif
#ifdef MIDDLE_GAIN_POTENTIOMETER
  MovingAverage<uint16_t, POT_FILTER_SAMPLES> middleFilter;
  Downmapper::LatchingDownmapper<int> middleMapper(POT_MIN_VAL, POT_MAX_VAL, -15, 15, 0.6, -15);
#endif
#ifdef BASS_GAIN_POTENTIOMETER
  MovingAverage<uint16_t, POT_FILTER_SAMPLES> bassFilter;
  Downmapper::LatchingDownmapper<int> bassMapper(POT_MIN_VAL, POT_MAX_VAL, -15, 15, 0.6, -15);
#endif
#ifdef INPUT_GAIN_POTENTIOMETER
  MovingAverage<uint16_t, POT_FILTER_SAMPLES> inputGainFilter;
  Downmapper::LatchingDownmapper<int> inputGainMapper(POT_MIN_VAL, POT_MAX_VAL, 0, 15, 0.6, 0);
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
    int tmp;

#ifdef TREBLE_GAIN_POTENTIOMETER
    values[0] = trebleMapper.Map(trebleFilter.add(analogRead(TREBLE_GAIN_POTENTIOMETER)));
    Preamp::setTrebleGain(values[0]);
#endif

#ifdef MIDDLE_GAIN_POTENTIOMETER
    values[1] = middleMapper.Map(middleFilter.add(analogRead(MIDDLE_GAIN_POTENTIOMETER)));
    Preamp::setMiddleGain(values[1]);
#endif

#ifdef BASS_GAIN_POTENTIOMETER
    values[2] = bassMapper.Map(bassFilter.add(analogRead(BASS_GAIN_POTENTIOMETER)));
    Preamp::setBassGain(values[2]);
#endif

#ifdef INPUT_GAIN_POTENTIOMETER
    values[3] = inputGainMapper.Map(inputGainFilter.add(analogRead(INPUT_GAIN_POTENTIOMETER)));
    Preamp::setInputGain(values[3]);
#endif

    while (true)
    {
#ifdef TREBLE_GAIN_POTENTIOMETER
      tmp = trebleMapper.Map(trebleFilter.add(analogRead(TREBLE_GAIN_POTENTIOMETER)));
      if (tmp != values[0])
      {
        values[0] = tmp;
        handle_controll(InputType::POT1_CH);
      }
#endif

#ifdef MIDDLE_GAIN_POTENTIOMETER
      tmp = middleMapper.Map(middleFilter.add(analogRead(MIDDLE_GAIN_POTENTIOMETER)));
      if (tmp > values[1])
      {
        values[1] = tmp;
        handle_controll(InputType::POT2_CH);
      }
#endif

#ifdef BASS_GAIN_POTENTIOMETER
      tmp = bassMapper.Map(bassFilter.add(analogRead(BASS_GAIN_POTENTIOMETER)));
      if (tmp > values[2])
      {
        values[2] = tmp;
        handle_controll(InputType::POT3_CH);
      }
#endif

#ifdef INPUT_GAIN_POTENTIOMETER
      tmp = inputGainMapper.Map(inputGainFilter.add(analogRead(INPUT_GAIN_POTENTIOMETER)));
      if (tmp != values[3])
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

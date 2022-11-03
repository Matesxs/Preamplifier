#include "potentiometer_handling.h"

#include <Arduino.h>

#include "global_objects.h"
#include "settings.h"
#include "pins.h"
#include "ui_controller.h"

volatile uint16_t potentiomater_values[POT_COUNT];

void handle_potentiometer_task(void *)
{
  for (int i = 0; i < POT_COUNT; i++)
  {
    switch (i)
    {
    case 0:
      potentiomater_values[i] = analogRead(POT1);
      break;

    case 1:
      potentiomater_values[i] = analogRead(POT2);
      break;

    case 2:
      potentiomater_values[i] = analogRead(POT3);
      break;

#ifdef POT4
    case 3:
      potentiomater_values[i] = analogRead(POT4);
      break;
#endif

    default:
      break;
    }
  }

#ifdef POT4
  preamp.setInputGain((int)map(potentiomater_values[3], 0, 4095, 0, 15));
#endif
  preamp.setTrebleGain((int)map(potentiomater_values[0], 0, 4095, -15, 15));
  preamp.setMiddleGain((int)map(potentiomater_values[1], 0, 4095, -15, 15));
  preamp.setBassGain((int)map(potentiomater_values[2], 0, 4095, -15, 15));

  while (true)
  {
    uint16_t tmp;
    uint16_t diff;

    for (int i = 0; i < POT_COUNT; i++)
    {
      switch (i)
      {
      case 0:
        tmp = analogRead(POT1);

        diff = (uint16_t)abs((int32_t)tmp - (int32_t)potentiomater_values[i]);
        // DEBUG("POT1 diff: %d\n", diff);
        if (diff > POTENTIOMETER_MIN_CHANGE)
        {
          potentiomater_values[i] = tmp;
          handle_controll(InputType::POT1_CH);
        }

        break;

      case 1:
        tmp = analogRead(POT2);
        
        diff = (uint16_t)abs((int32_t)tmp - (int32_t)potentiomater_values[i]);
        // DEBUG("POT2 diff: %d\n", diff);
        if (diff > POTENTIOMETER_MIN_CHANGE)
        {
          potentiomater_values[i] = tmp;
          handle_controll(InputType::POT2_CH);
        }
        break;

      case 2:
        tmp = analogRead(POT3);
        
        diff = (uint16_t)abs((int32_t)tmp - (int32_t)potentiomater_values[i]);
        // DEBUG("POT3 diff: %d\n", diff);
        if (diff > POTENTIOMETER_MIN_CHANGE)
        {
          potentiomater_values[i] = tmp;
          handle_controll(InputType::POT3_CH);
        }
        break;

#ifdef POT4
      case 3:
        tmp = analogRead(POT4);
        
        diff = (uint16_t)abs((int32_t)tmp - (int32_t)potentiomater_values[i]);
        // DEBUG("POT4 diff: %d\n", diff);
        if (diff > POTENTIOMETER_MIN_CHANGE)
        {
          potentiomater_values[i] = tmp;
          handle_controll(InputType::POT4_CH);
        }
        break;
#endif
      
      default:
        break;
      }

      vTaskDelay(pdMS_TO_TICKS(1));
    }

    vTaskDelay(pdMS_TO_TICKS(POTENTIOMETER_PULLING_RATE_MS));
  }

  vTaskDelete(NULL);
}

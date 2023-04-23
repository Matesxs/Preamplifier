#pragma once

#include <Arduino.h>

namespace PotentiometerHandling
{
  void init();

  extern volatile uint16_t values[];

  void handle_potentiometer_task(void *);
};

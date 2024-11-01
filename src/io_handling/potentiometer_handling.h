#pragma once

#include <Arduino.h>

namespace PotentiometerHandling
{
  void init();

  extern volatile int values[];

  void handle_potentiometer_task(void *);
};

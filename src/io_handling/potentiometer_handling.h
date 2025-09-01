#pragma once

namespace PotentiometerHandling
{
  void init();

  extern volatile int values[];

  [[noreturn]] void handle_potentiometer_task(void *);
};

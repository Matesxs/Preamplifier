#pragma once

namespace TemperatureReader
{
  extern float* temperatures;
  void temperature_task(void*);
  uint8_t getSensorCount();
  float maxTemp();
}

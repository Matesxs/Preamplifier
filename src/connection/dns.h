#pragma once

namespace DNS
{
  bool begin(const char* name="");
  bool setName(const char* name);
  const char* getName();
  bool reset();
  void end();
}


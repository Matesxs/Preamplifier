#pragma once

namespace Wifi
{
  void SetCredentials(const char* ssid, const char* password);
  void begin(const char* ssid = "", const char* password = "");
  void end();
  const char* getSSID();
  const char* getPassword();
}

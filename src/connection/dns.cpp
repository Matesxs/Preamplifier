#include "dns.h"

#include <Arduino.h>
#include <ESPmDNS.h>

namespace DNS
{
  String dns_name;

  bool begin(const char* name)
  {
    dns_name = String(name);

    if (dns_name != "")
    {
      return MDNS.begin(dns_name.c_str());
    }

    return false;
  }

  bool setName(const char* name)
  {
    end();
    return begin(name);
  }

  bool reset()
  {
    if (dns_name != "")
    {
      end();
      return MDNS.begin(dns_name.c_str());
    }

    return true;
  }

  const char* getName()
  {
    return dns_name.c_str();
  }

  void end()
  {
    MDNS.end();
  }
}

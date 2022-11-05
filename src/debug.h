#pragma once

#include "settings.h"

#ifdef ENABLE_DEBUG
#define DEBUG(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

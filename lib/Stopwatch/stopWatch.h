#pragma once

#include "Arduino.h"

void startStopwatch(uint64_t microsecond_divider=1000);
void restartStopwatch();
unsigned long long getStopwatchVal();

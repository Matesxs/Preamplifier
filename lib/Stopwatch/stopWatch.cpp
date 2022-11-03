#include "stopWatch.h"

volatile unsigned long long count = 0;

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define CRITICAL(X, Y)       \
  portENTER_CRITICAL_ISR(Y); \
  X;                         \
  portEXIT_CRITICAL_ISR(Y);

void IRAM_ATTR onTime()
{
  CRITICAL(count++, &timerMux);
}

void restartStopwatch()
{
  CRITICAL(count = 0, &timerMux);
}

void startStopwatch(uint64_t microsecond_divider)
{
  if (timer == NULL)
  {
    timer = timerBegin(3, 80, true);
    timerAttachInterrupt(timer, &onTime, true);
    timerAlarmWrite(timer, microsecond_divider, true); 
    timerAlarmEnable(timer);
  }

  restartStopwatch();
}

unsigned long long getStopwatchVal()
{
  unsigned long long tmp = count;
  vTaskDelay(5);
  return tmp;
}
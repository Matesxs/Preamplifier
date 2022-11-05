#pragma once

#include <Arduino.h>

void IRAM_ATTR update_encoder();
void encoder_task(void*);
void buttons_task(void *);

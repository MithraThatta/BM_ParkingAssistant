#include "stm32f103xb.h"
#include <stdio.h>
#include <stdint.h>
#ifndef TIMER_H_
#define TIMER_H_

void Tim4InputCaptureSetup(void);
void testpulseINIT(void);
void systickDelayuS(int Delay);
void testpulse(void);

#endif /* TIMER_H_ */

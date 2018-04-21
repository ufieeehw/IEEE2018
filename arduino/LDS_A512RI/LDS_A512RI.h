#ifndef __LDS_A512RI_H__
#define __LDS_A512RI_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

void init_led();

void led_clear();

void led_display(int number);

#endif
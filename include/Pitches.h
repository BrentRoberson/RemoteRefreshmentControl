
#ifndef PITCHES_H
#define PITCHES_H

#include<Arduino.h>
#define buzzerPin  4 // ESP32 pin GIOP18 connected to piezo buzzer

void startup();
void rfidGoodTap();
void rfidBadTap();

#endif
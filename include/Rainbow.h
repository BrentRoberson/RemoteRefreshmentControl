
#include <Arduino.h>
#include <Globals.h>


void LEDSetup();
void changeColor(char r, char g, char b);
void rainbowCycle(int DelayDuration);
byte *Wheel(byte WheelPosition);
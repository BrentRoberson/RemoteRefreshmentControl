
#include <Arduino.h>
#include "FastLED.h"
#define NUM_LEDS  90    // Enter the total number of LEDs on the strip
#define PIN      26       // The pin connected to Din to control the LEDs


void LEDSetup();

void rainbowCycle(int DelayDuration);
byte *Wheel(byte WheelPosition);
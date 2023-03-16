
#include <Rainbow.h>
byte *c;
uint16_t j = 0;
CRGB leds[NUM_LEDS];
unsigned int LedDelay = 0;
void rainbowCycle(int DelayDuration) {

  if((LedDelay+DelayDuration)<millis()){
    LedDelay = millis(); 
    for(uint16_t i=0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      leds[NUM_LEDS - 1 - i].setRGB(*c, *(c+1), *(c+2));
    }
    FastLED.show();
    j = (j+1)%256;
  }
}


void LEDSetup(){
  FastLED.addLeds<WS2812B, LED_STRIP, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(12, 1500);    // Set power limit of LED strip to 5V, 1500mA
  FastLED.clear();        
}

byte *Wheel(byte WheelPosition) {
  static byte c[3];
 
  if(WheelPosition < 85) {
   c[0] = WheelPosition * 3;
   c[1] = 255 - WheelPosition * 3;
   c[2] = 0;
  }
  else if(WheelPosition < 170) {
   WheelPosition -= 85;
   c[0] = 255 - WheelPosition * 3;
   c[1] = 0;
   c[2] = WheelPosition * 3;
  }
  else {
   WheelPosition -= 170;
   c[0] = 0;
   c[1] = WheelPosition * 3;
   c[2] = 255 - WheelPosition * 3;
  }

  return c;
}
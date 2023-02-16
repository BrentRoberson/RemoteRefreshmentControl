#include <Pitches.h>

#define buzzerPin  4 // ESP32 pin GIOP18 connected to piezo buzzer

int goodTapFrequencies[] = {2255, 3000, 5000, 6000, 8000};
int badTapFrequencies[] = {2255, 1200, 600, 300, 150};
int tapFrequencyCount = 4;


void startup() {
  ledcSetup(0, 2000, 8);
  ledcAttachPin(buzzerPin, 0);
  for( int j = 0; j< 2; j++){
    for (int i = 0; i <= tapFrequencyCount; i++) {
      ledcWriteTone(0, goodTapFrequencies[i]);
      delay(50);
      ledcWriteTone(0, 0);
      delay(50);
    }
    for (int i = tapFrequencyCount; i >=0 ; i--) {
      ledcWriteTone(0, goodTapFrequencies[i]);
      delay(20);
      ledcWriteTone(0, 0);
      delay(20);
    }
  }
  
  ledcWriteTone(0, 0);
}

void rfidGoodTap(){
  for (int i = 0; i < tapFrequencyCount; i++) {
    ledcWriteTone(0, goodTapFrequencies[i]);
    delay(20);
    ledcWriteTone(0, 0);
    delay(20);
  }
}
void rfidBadTap(){
  for (int i = 0; i <= tapFrequencyCount; i++) {
    ledcWriteTone(0, badTapFrequencies[i]);
    delay(20);
    ledcWriteTone(0, 0);
    delay(20);
  }
  for (int i = 0; i <= tapFrequencyCount; i++) {
    ledcWriteTone(0, badTapFrequencies[i]);
    delay(20);
    ledcWriteTone(0, 0);
    delay(20);
  }
  
}


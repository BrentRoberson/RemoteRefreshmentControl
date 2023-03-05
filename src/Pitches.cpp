#include <Pitches.h>


int goodTapFrequencies[] = {2255, 3000, 5000, 6000, 8000};
int badTapFrequencies[] = {2255, 1200, 600, 300, 150};
int tapFrequencyCount = 4;


void startup() {
  // ledcSetup(0, 2000, 8);
  // ledcAttachPin(BUZZER_PIN, 0);
  // for( int j = 0; j< 2; j++){
  //   for (int i = 0; i <= tapFrequencyCount; i++) {
  //     ledcWriteTone(0, goodTapFrequencies[i]);
  //     delay(50);
  //     ledcWriteTone(0, 0);
  //     delay(50);
  //   }
  //   for (int i = tapFrequencyCount; i >=0 ; i--) {
  //     ledcWriteTone(0, goodTapFrequencies[i]);
  //     delay(20);
  //     ledcWriteTone(0, 0);
  //     delay(20);
  //   }
  // }
  
  // ledcWriteTone(0, 0);
}


void rfidGoodTap(){
  for(int i = 0; i<3; i++){
    digitalWrite(BUZZER_PIN,HIGH);
    delay(20);
    digitalWrite(BUZZER_PIN,LOW);
    delay(10);
  }
}


void rfidBadTap(){
  for(int i = 0; i<3; i++){
    digitalWrite(BUZZER_PIN,HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN,LOW);
    delay(25);
  }
  
}

void newAddedTap(){
  for(int i = 0; i<6; i++){
    digitalWrite(BUZZER_PIN,HIGH);
    delay(10);
    digitalWrite(BUZZER_PIN,LOW);
    delay(20);
  }
}

void addedTap(){
  for(int i = 0; i<6; i++){
    digitalWrite(BUZZER_PIN,HIGH);
    delay(15);
    digitalWrite(BUZZER_PIN,LOW);
    delay(10);
  }
}


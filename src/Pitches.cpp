#include <Pitches.h>


int goodTapFrequencies[] = {2255, 3000, 5000, 6000, 8000};
int badTapFrequencies[] = {2255, 1200, 600, 300, 150};
int tapFrequencyCount = 4;


void startup() {
  for(int i = 0; i<6; i++){
    digitalWrite(BUZZER_PIN,HIGH);
    delay(10);
    digitalWrite(BUZZER_PIN,LOW);
    delay(5);
  }
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


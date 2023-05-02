#include <Pitches.h>



void startup() {
  int melody[] = {
  A7,A7,GS7,GS7,FS7,GS7,FS7,0, E7
  };

  int noteDurations[] = {
    4, 4, 4, 4, 8, 8, 8, 8, 2
  };
  for (int note = 0; note < sizeof(melody) / sizeof(melody[0]); note++) {
    Serial.print("Playing note");
    int noteDuration = 1000 / noteDurations[note];
    tone(BUZZER_PIN, melody[note], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.35;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }

}


void rfidGoodTap(){
  for(int i = 0; i<3; i++){
    tone(BUZZER_PIN, 2700, 50);
    delay(40);
    noTone(BUZZER_PIN);
    delay(40);
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


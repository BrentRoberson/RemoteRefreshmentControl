#include <Pitches.h>


int goodTapFrequencies[] = {2255, 3000, 5000, 6000, 8000};
int badTapFrequencies[] = {2255, 1200, 600, 300, 150};
int tapFrequencyCount = 4;


void startup() {
  int melody[] = {
  A7,A7,GS7,GS7,FS7,GS7,FS7,0, E7
  };

  // The noteDurations array contains the duration of each note (1 for whole note, 2 for half note, etc.)
  int noteDurations[] = {
    4, 4, 4, 4, 8, 8, 8, 8, 2
  };

  // Play the song once
  for (int note = 0; note < sizeof(melody) / sizeof(melody[0]); note++) {
    Serial.print("Playing note");
    int noteDuration = 1000 / noteDurations[note];
    tone(BUZZER_PIN, melody[note], noteDuration);

    // To distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.35;
    delay(pauseBetweenNotes);

    // Stop the tone playing
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
    tone(BUZZER_PIN, 2500-250*i, 50);
    delay(40);
    noTone(BUZZER_PIN);
    delay(40);
  }
  
}


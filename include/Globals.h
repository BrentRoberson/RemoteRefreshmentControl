#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

#define BUTTON_PIN 13
#define ENC_CLOCK 26
#define ENC_DATA 12
#define NUM_SCREENS 5

extern double pricePerOunce; // = .40;
extern double ouncesLeft; // = 640;
extern double drinksLeft; // = ouncesLeft/12;
extern LiquidCrystal_I2C lcd;
extern Encoder myEnc;
extern String screens[NUM_SCREENS][2];
extern int parameters[NUM_SCREENS];


#endif 
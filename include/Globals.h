#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>

#define BUTTON_PIN 13
#define ENC_CLOCK 2
#define ENC_DATA 4
#define NUM_SCREENS 5
#define CLK 2 // CLK ENCODER
#define DT 4 // DT ENCODER


extern double pricePerOunce; // = .40;
extern double ouncesLeft; // = 640;
extern double drinksLeft; // = ouncesLeft/12;
extern LiquidCrystal_I2C lcd;
extern ESP32Encoder encoder;
extern String screens[NUM_SCREENS][2];
extern int parameters[NUM_SCREENS];


#endif 
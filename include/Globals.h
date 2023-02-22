#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <DynamicArray.h>
#include <Customer.h>

#define BUTTON_PIN 13
#define ENC_CLOCK 2
#define ENC_DATA 4
#define NUM_SCREENS 5
#define CLK 2 // CLK ENCODER
#define DT 4 // DT ENCODER
#define SS_PIN  32  // ESP32 pin GIOP5 
#define RST_PIN 33 // ESP32 pin GIOP27 
#define SOLENOID 5

class Customer;
extern String drinkOTD;
extern double pricePerOunce; // = .40;
extern double totalQuarts; // = 640;
extern int maxDrinks; // = ouncesLeft/12;
extern LiquidCrystal_I2C lcd;
extern ESP32Encoder encoder;
extern String screens[NUM_SCREENS];
extern DynamicArray<Customer> customers;


#endif 
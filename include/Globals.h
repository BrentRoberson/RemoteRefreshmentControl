#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <DynamicArray.h>
#include <Customer.h>

#define BUTTON_PIN 13
#define NUM_SCREENS 5  //settings 
#define CLK 2 // CLK ENCODER
#define DT 4 // DT ENCODER
#define SS_PIN  32  // For RFID
#define RST_PIN 33 // For RFID
#define PUMP 5
#define DOOR_LOCK 14
#define SENSOR 27
#define GREEN_BUTTON 12
#define RED_BUTTON  10
#define LED_STRIP 9
class Customer;
extern String drinkOTD;
extern double pricePerOunce; // = .40;
extern double totalQuarts; // = 640;
extern int maxDrinks; // 
extern LiquidCrystal_I2C lcd;
extern ESP32Encoder encoder;
extern DynamicArray<Customer> customers;
extern Customer lastCustomerScanned;


#endif 
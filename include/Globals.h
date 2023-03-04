#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <DynamicArray.h>
#include <Customer.h>
#include <ESPNow.h>

#define ENCODER_BUTTON 13
// #define NUM_SCREENS 5  //settings 
#define CLK 2 // CLK ENCODER
#define DT 4 // DT ENCODER
#define SS_PIN  32  // For RFID
#define RST_PIN 33 // For RFID
#define PUMP 16
#define DOOR_LOCK 14
#define SENSOR 27
#define registerButton 5
#define DISPENSE_BUTTON 25
#define DONE_BUTTON  26
#define LED_STRIP 17
#define BUZZER_PIN  12 

class Customer;
extern String drinkOTD;
extern double pricePerOunce; // = .40;
extern double totalQuarts; // = 640;
extern int maxDrinks; // 
extern double refund_amount;
extern bool register_sent;
extern bool refund_received;
extern bool bal_received;
extern double returned_balance;
extern ESP32NOW espNow; 
extern LiquidCrystal_I2C lcd;
extern ESP32Encoder encoder;
extern DynamicArray<Customer> customers;
extern Customer lastCustomerScanned;


#endif 
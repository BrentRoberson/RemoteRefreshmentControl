#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <DynamicArray.h>
#include <Customer.h>
#include <ESPNow.h>
#include "SPI.h"

#define ENCODER_BUTTON 13
// #define NUM_SCREENS 5  //settings 
#define CLK 2 // CLK ENCODER
#define DT 4 // DT ENCODER
#define CS_PIN_RFID  32  // For RFID
#define RST_PIN 33 // For RFID
#define PUMP 16
#define DOOR_LOCK 14
#define SENSOR 27
#define registerButton 5
#define DISPENSE_BUTTON 34
#define DONE_BUTTON  35
#define LED_STRIP 17
#define BUZZER_PIN  12 
#define SCK_RFID  18
#define MISO_RFID  19
#define MOSI_RFID  23
#define SCK_SD 18
#define MISO_SD 19
#define MOSI_SD 23




/*
RFID is SPI communication, LCD is I2C
SD card also uses SPI communication

RFID connections:
CS (SS) - GND (only one slave on SPI bus VSPI)
RST - 33 
MISO - 23
MOSI - 19
SCK - 18

LCD connections:
SDA - 21
SCL - 22

SD connections:
CS (SS) - GND (only one slave on SPI bus HSPI)
MISO - 23
MOSI - 19
SCK - 18



*/

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
extern SPIClass spi;
extern ESP32Encoder encoder;
extern DynamicArray<Customer> customers;
extern Customer lastCustomerScanned;


#endif 
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <DynamicArray.h>
#include <PinButton.h>
#include <Customer.h>
#include <ESPNow.h>
#include <SPI.h>
#include <SDCard.h>

#define ENCODER_BUTTON 13
// #define NUM_SCREENS 5  //settings 
#define CLK 2 // CLK ENCODER
#define DT 4 // DT ENCODER
#define CS_PIN_RFID  32  // For RFID
#define CS_SD 5
#define RST_PIN 33 // For RFID
#define PUMP 16
#define DOOR_LOCK 14
#define SENSOR 27
#define registerButton 27
#define DISPENSE_BUTTON 34
#define DONE_BUTTON  35
#define LED_STRIP 26
#define BUZZER_PIN  12 
#define NUM_SETTINGS 6
#define JSON_SIZE 20000



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


*/
class SDCard;
class Customer;
extern float pricePerOunce; // = .40;
extern float totalQuarts; // = 640;
extern int maxDrinks; // 
extern float refund_amount;
extern bool register_sent;
extern bool refund_received;
extern bool bal_received;
extern float returned_balance;
extern bool new_sd_data;
extern Customer curr_cust;
extern ESP32NOW espNow; 
extern LiquidCrystal_I2C lcd;
extern SPIClass spi;
extern ESP32Encoder encoder;
extern DynamicArray<Customer> customers;
extern int currentScannedIndex;
extern PinButton encoderButton;
extern PinButton sendButton;
extern SDCard SdData;

#endif 
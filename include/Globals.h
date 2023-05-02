#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <DynamicArray.h>
#include <PinButton.h>
#include <Customer.h>
#include <SPI.h>
#include <SDCard.h>
#include <Webserver.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


#define ENCODER_BUTTON 16
// #define NUM_SCREENS 5  //settings 
#define CLK 2 // CLK ENCODER
#define DT 4 // DT ENCODER
#define CS_SD 5
#define PUMP 13
#define DOOR_LOCK 25
#define SENSOR 27
#define CD_SD 14
#define DISPENSE_BUTTON 32
#define LED_STRIP 26
#define BUZZER_PIN  12 
#define NUM_SETTINGS 7
#define JSON_SIZE 30000
#define NUM_LEDS  30    // Enter the total number of LEDs on the strip
#define DISPENSE_LEDS 33

/*
SD card uses SPI communication
SD connections:
DI - 23
DO - 19
CS - 5 
CLK - 18

NFC is I2C communication, LCD is also I2C
NFC connections:
SDA - 21
SCL - 22

LCD connections:
SDA - 21
SCL - 22


*/
class SDCard;
class Customer;
extern WebServer server;
extern float pricePerOunce; // = .40;
extern float totalQuarts; // = 640;
extern int maxOunces; // 
extern float refund_amount;
extern bool register_sent;
extern bool refund_received;
extern bool bal_received;
extern float returned_balance;
extern bool new_sd_data;
extern Customer curr_cust;
extern LiquidCrystal_I2C lcd;
extern SPIClass spi;
extern ESP32Encoder encoder;
extern DynamicArray<Customer> customers;
extern int currentScannedIndex;
extern PinButton encoderButton;
extern PinButton sendButton;
extern SDCard SdData;
extern bool isServer;
extern const char* routerSsid;
extern const char* routerPassword;
extern const char* accessPointSSID;
extern const char* accessPointPassword;
#endif 
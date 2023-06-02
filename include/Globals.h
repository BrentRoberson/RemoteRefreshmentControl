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
#define FASTLED_INTERNAL
#include <FastLED.h>


#define ENCODER_BUTTON 16
// #define NUM_SCREENS 5  //settings 
#define CLK 2 // CLK ENCODER
#define DT 4 // DT ENCODER
#define CS_SD 5
#define PUMP_OR_SOLENOID 13
#define DOOR_LOCK 25
#define SENSOR 15
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
extern float pricePerOunce;
extern float totalQuarts; 
extern uint8_t maxOunces; 
extern bool new_sd_data;
extern LiquidCrystal_I2C lcd;
extern SPIClass spi;
extern ESP32Encoder encoder;
extern DynamicArray<Customer> customers;
extern uint16_t currentScannedIndex;
extern PinButton encoderButton;
extern PinButton sendButton;
extern SDCard SdData;
extern bool isServer;
extern bool usingPump;
extern bool usingAP;
extern const char* routerSSID;
extern const char* routerPassword;
extern const char* accessPointSSID;
extern const char* accessPointPassword;
extern const char* serverIP;

#endif 
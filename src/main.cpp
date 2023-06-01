#include <WiFi.h>
#include <Arduino.h>
#include <NFC.h>
#include <Drink.h>
#include <Customer.h>
#include <Pitches.h>
#include <LiquidCrystal_I2C.h>
#include <Menu.H>
#include <SDCard.h>
#include <Rainbow.h>
#include <EspServer.h>
#include <SdFat.h>
#include <EspClient.h>

#define CLK_PIN 14
#define DT_PIN 26
#define SW_PIN 13

const uint8_t SCK_PIN = 18;
const uint8_t MOSI_PIN = 19;
const uint8_t MISO_PIN = 23;

SPIClass spi(HSPI);
SdFat sd;

PinButton encoderButton(ENCODER_BUTTON);
float pricePerOunce;
float totalQuarts;
uint8_t maxOunces;
bool isServer = true;
ESP32Encoder encoder;
bool readError;
Customer clientCustomer = Customer();
DynamicArray<Customer> customers;
uint16_t currentScannedIndex;
bool new_sd_data = false;
LiquidCrystal_I2C lcd(0x27, 20, 4);
SDCard SdData("/DATA.txt");
Menu menu;

//eventually read in this data to settings on the sd card
bool usingAP;
const char* serverIP;
const char* routerSSID;
const char* routerPassword;
const char* accessPointSSID;
const char* accessPointPassword;

void setup() {
  // Set up Serial Monitor
  pinMode(23, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(MISO_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  pinMode(DISPENSE_BUTTON, INPUT_PULLUP);
  pinMode(DOOR_LOCK, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);
  digitalWrite(PUMP,LOW);
  digitalWrite(DOOR_LOCK,LOW);
  pinMode(CS_SD, OUTPUT);

  spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN); //Delete once uploading to new boards
  SD.end(); // Release the default SPI pins

  lcd.init();
  lcd.backlight();
  Serial.println("above interrupt");
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  Serial.println("after interrupt");
  LEDSetup();

  startup();
  lcd.print("NFC Connecting...");
  NFCsetup();
  lcd.clear();
  lcd.print("Reading in SD...");
  if(!SD.begin(CS_SD,spi)) {
    Serial.println("initialization failed or no SD card!");
    lcd.clear();
    lcd.print("SD FAILURE");
    return;
  }
  SdData.readInSD();
  isServer= false;
  serverIP = "192.168.1.40";
  usingAP = false;
  routerSSID = "The Dawg House";
  routerPassword = "kodabear";
  accessPointSSID = "BarBox Wifi";
  accessPointPassword = "brentiepoo";

  if(isServer){
    setupServer();
  }
  else{
    setupClient();
  }
  Serial.println("initialization done.");
  menu.printLcdWelcome();
}


void loop() {
  menu.run();
}
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
#include <ESP32API.h>
#define CLK_PIN 14
#define DT_PIN 26
#define SW_PIN 13

PinButton encoderButton(ENCODER_BUTTON);
PinButton doneButton(DONE_BUTTON);
float pricePerOunce= .30;
float totalQuarts = 10;
int maxOunces = 80;

ESP32Encoder encoder;
bool readError;
Customer curr_cust = Customer();
DynamicArray<Customer> customers;
int currentScannedIndex;
bool new_sd_data = false;
LiquidCrystal_I2C lcd(0x27, 20, 4);
//Menu menu;
SDCard SdData("/DATA.txt");

Menu menu;


void setup() {
  // Set up Serial Monitor
  
  Serial.begin(115200);
  pinMode(DISPENSE_BUTTON, INPUT);
  pinMode(DONE_BUTTON, INPUT);
  pinMode(DOOR_LOCK, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);

  setupAPI();
  lcd.init();
  lcd.backlight();
  // esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Dispenser setup");
  lcd.print("Startup Completed!");
  LEDSetup();
  startup();
  NFCsetup();
  if(!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  SdData.readInSD();
  menu.printLcdWelcome();

}


void loop() {
  menu.run();
}
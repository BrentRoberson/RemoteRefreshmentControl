#ifndef NEWMENU_H
#define NEWMENU_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <Globals.h>
#include <Customer.h>
#include <RFID.h>
#include <Pitches.h>
#include <PinButton.h>

void editLastCustomerScreen(String title, String action, std::function<void()> onValidation);
template <typename T>
void editSetting(T & value, double increment, double decrement);
template <typename T>
void displaySetting(const char* title, T value);

void openDoorScreen();
void enterSettings();
void printLcdWelcome();
void encoderPushed();
void menuSetup();
void waitScreen();
void dispenseScreen();
void settingsScreen();
void run();

#endif
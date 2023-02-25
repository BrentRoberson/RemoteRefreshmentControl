#ifndef REGISTER_MENU_H
#define REGISTER_MENU_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <Globals.h>
#include <Customer.h>
#include <RFID.h>
#include <Pitches.h>


class registerMenu: public Menu {
 
  public:
    registerMenu();
    void setup();
    void rfidScanner();
    void displayMenu();
    static void triggerMenu();
};



#endif

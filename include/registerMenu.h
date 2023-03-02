#ifndef REGISTER_MENU_H
#define REGISTER_MENU_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <Globals.h>
#include <Customer.h>
#include <message.h>
#include <Menu.h>
#include <ESPNow.h>
#include <RFID.h>
#include <WiFi.h>
#include <Pitches.h>


class registerMenu: public Menu {
 
  public:
    registerMenu();
    void displayMenu() override;
    void waitScreen() override;
    static void triggerMenu();
    void run() override;
    void printLcdWelcome() override;
    void setup() override;
    void set_num_screens(int screens);
    void add_a_tag();
    void refund_tag();
    void check_balance();
};



#endif

#ifndef REGISTER_MENU_H
#define REGISTER_MENU_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <Globals.h>
#include <Customer.h>
#include <message.h>
#include <NewMenu.h>
#include <ESPNow.h>
#include <RFID.h>
#include <WiFi.h>
#include <Pitches.h>


class registerMenu: public NewMenu {
 
  public:
    registerMenu();
    void displayMenu(); 
    void waitScreen() override;
    void run() override;
    void setup();
    void printLcdWelcome() override;
    void set_num_screens(int screens);
    void add_a_tag();
    void refund_tag();
    void check_balance();
    static void triggerMenu();


    static int currentScreen;
    bool waiting;
    static unsigned long menuTriggeredTime;
    static int NUM_SCREENS;
    static long oldPosition;
    long newPosition;
    static long initPosition;
    static int validationTurns;
    static bool updateScreen;
};

int registerMenu::NUM_SCREENS;
int registerMenu::currentScreen;
long registerMenu::oldPosition;
unsigned long registerMenu::menuTriggeredTime;
long registerMenu::initPosition;



#endif

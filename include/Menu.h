#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <Globals.h>
#include <Customer.h>
#include <RFID.h>
#include <Pitches.h>
#include <RFID.h>
//https://www.youtube.com/watch?v=P36GIB0dYks&list=PLfwpK3RIxFR39m27Q8C6zjnj9kOjJz3VV&index=5
//heavily relied on this menu system^


class Menu {
 
  public:
    Menu();
    virtual void setup();
    virtual void run();
    virtual void waitScreen();
    void clearLCDLine(int line);
    static void triggerMenu();
    // void triggerMenu();

    virtual void displayMenu();

    // public member variables of the function 
    static int NUM_SCREENS;
    static long oldPosition;
    long newPosition;
    static long initPosition;
    static unsigned long menuTriggeredTime;
    static int currentScreen;
    static bool updateScreen;
    bool updateJustVal = true;
    unsigned long rfidTriggerTime = 0;
    bool waiting = false;
    bool scanTimeout = false;
    String readTag = "";
    bool newTap = false;
    static int validationTurns;
    
  protected:
    virtual void printLcdWelcome();

    template <typename T>
    void editSetting(T & value, double increment, double decrement);

    void printSettingTitle();

    void openDoorScreen();

    template <typename T>
    void displaySetting(const char* title, T value);

    bool validated();

    void editLastCustomerScreen(String title, String action, std::function<void()> onValidation);

};





#endif


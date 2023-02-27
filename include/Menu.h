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
    void setup();
    void run();
    void waitScreen();
    void clearLCDLine(int line);
    static void triggerMenu();
    void displayMenu();

  private:
    void printLcdWelcome();

    template <typename T>
    void editSetting(T & value, double increment);

    void printSettingTitle();

    void openDoor();

    template <typename T>
    void displaySetting(const char* title, T value);

    bool validated();

    void editLastCustomerScreen(String title, String action, std::function<void()> onValidation);

};





#endif


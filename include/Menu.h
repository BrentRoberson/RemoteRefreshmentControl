#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <Globals.h>

//https://www.youtube.com/watch?v=P36GIB0dYks&list=PLfwpK3RIxFR39m27Q8C6zjnj9kOjJz3VV&index=5
//heavily relied on this menu system^


class Menu {
  private:
    static long oldPosition;
    static long initPosition;
    static unsigned long menuTriggeredTime;
    static const int numOfScreens;
    static int currentScreen;
    String ** screens;
    int * parameters;
    static bool updateScreen;

  public:
    Menu(int numOfScr);
    void setup();
    void run();
    void initScreen();
    void printPosition(long pos);
    void clearLCDLine(int line);
    static void triggerMenu();
    void displayMenu();
};

#endif

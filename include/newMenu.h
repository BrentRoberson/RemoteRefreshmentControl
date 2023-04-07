#ifndef NEWMENU_H
#define NEWMENU_H

#include <Jug.h>
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <Globals.h>
#include <Customer.h>
#include <NFC.h>
#include <Pitches.h>
#include <PinButton.h>
#include <Jug.h>
#include <Rainbow.h>
#include <ESP32API.h>
class NewMenu {
  public:

    NewMenu();
    virtual void run();
    virtual void waitScreen();
    virtual void printLcdWelcome();

  protected:
    virtual void printSettingTitle();

    void editCustOnSwipe(String title, String action, std::function<void()> onSwipe) ;
    
    void addOrSetMoneyOnSwipe(bool setAmount);

    virtual void dispenseScreen();

    virtual void settingsScreen();

    template <typename T>
    void editSetting(T & value, float increment, float decrement);

    template <typename T>
    void displaySetting(const char* title, T value);

    long oldPosition;
    long newPosition;
    long initPosition;
    unsigned long buttonJustPressed;
    unsigned long settingsTriggeredTime;
    unsigned long dispenseLastTouched;
    int currentScreen;
    bool updateScreen;
    unsigned long rfidTriggerTime;
    bool waiting;
    bool scanTimeout;
    String readTag;
    bool newTap;
    int addAmount;
    int validationTurns;
    int currentSetting;
    unsigned long lastButtonPressTime;
    unsigned long currentTime;
    bool encoderPressed;
};

#endif

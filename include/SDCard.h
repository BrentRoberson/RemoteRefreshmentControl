#ifndef SDCARD_H
#define SDCARD_H

#include <Globals.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <DynamicArray.h>
#include <Customer.h>

class Customer;

class SDCard {
  public:
    SDCard(const String filename);
    void addOrUpdateCustomer(Customer & customer);
    void updateSettings();
    void readInSD();
    void removeCustomer(Customer & customer);
  private:
    String filename;
};

#endif

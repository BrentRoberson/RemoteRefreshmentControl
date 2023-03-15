#ifndef SDCard_h
#define SDCard_h

#include <ArduinoJson.h>
#include <SD.h>
#include "DynamicArray.h"
#include "Customer.h"
#include "Globals.h"

class SDCard {
  public:
    SDCard(const char* filename);
    void addOrUpdateCustomer(Customer* customer);
    void readCustomers();
    
  private:
    const char* filename;
};

#endif

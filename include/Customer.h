
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <DynamicArray.cpp>
#include <Drink.h>
#include <Globals.h>
#include <LiquidCrystal_I2C.h>
//#include <Menu.h>

class Customer {
  public:
    Customer(String id = "", float balance = 0, bool manager = false);

    //eventually make private with accessor functions
    String ID;
    float balance;
    float ouncesDrank;
    bool manager;
    

    void lcdPrint();
    void print();
    

};
void removeCustomer();
void makeManager();



#endif
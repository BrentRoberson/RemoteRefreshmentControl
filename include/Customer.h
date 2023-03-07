
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <DynamicArray.cpp>
#include <Drink.h>
#include <Globals.h>
#include <LiquidCrystal_I2C.h>
//#include <Menu.h>

class Customer {
  public:
    Customer(String id = "", double balance = 0);

    //eventually make private with accessor functions
    String ID;
    double balance;
    DynamicArray<Drink>* drinks;
    bool manager;
    

    void lcdPrint();
    void print();
    

};
void removeCustomer();
void makeManager();



#endif
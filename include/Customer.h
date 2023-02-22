
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <DynamicArray.cpp>
#include <Drink.h>
#include <LiquidCrystal_I2C.h>

class Customer {
  public:
    Customer(String id = "", double balance = 0);
    String ID;
    double balance;
    DynamicArray<Drink>* drinks;
    void lcdPrint(LiquidCrystal_I2C & lcd);
    void print();
};




#endif
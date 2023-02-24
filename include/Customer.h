
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <DynamicArray.cpp>
#include <Drink.h>
#include <LiquidCrystal_I2C.h>
#include <Globals.h>
#include <Menu.h>

class Customer {
  public:
    Customer(String id = "", double balance = 0);
    friend class Menu;

    //eventually make private with accessor functions
    String ID;
    double balance;
    DynamicArray<Drink>* drinks;
    bool manager;

    void lcdPrint();
    void print();
    void editCustomer(Customer customer);
};

void addCustomer();



#endif
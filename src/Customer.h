
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <Drink.h>
#include <DynamicArray.h>
#include <Arduino.h>

class Customer {
  public:
    String ID;
    float ozLeft;
    DynamicArray<Drink> * drinks;
    Customer(String id = "", float oz = 0);
    void print();
};




#endif
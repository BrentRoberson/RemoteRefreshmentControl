
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <Arduino.h>
#include <DynamicArray.h>
#include <Drink.h>

class Customer {
  public:
    String ID;
    float ozLeft;
    DynamicArray<Drink> * drinks;
    Customer(String id = "", float oz = 0);
    void print();
};




#endif
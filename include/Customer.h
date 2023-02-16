
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <DynamicArray.cpp>
#include <Drink.h>


class Customer {
  public:
    Customer(String id = "", float oz = 0);
    String ID;
    float ozLeft;
    DynamicArray<Drink>* drinks;
    void print();
};




#endif
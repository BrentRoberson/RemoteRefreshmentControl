#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <Arduino.h>
#include <Customer.h>

using namespace std;

template <typename T> 
class DynamicArray {
  private:
    T* array = NULL;

    int size;

    int capacity;

  public:
    DynamicArray();

    DynamicArray(int capacity);

    int getSize();

    int getCapacity();

    void push_back(T value);

    void pop_back();

    void growArray();

    void shrinkArray();

    int search(String id);

    void insertAt(int index, T value);

    void deleteAt(int index);

    void printArrayDetails();

    bool isEmpty();
};

#endif
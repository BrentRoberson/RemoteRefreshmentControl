#ifndef DRINK_H
#define DRINK_H

#include <Arduino.h>

class Drink {
public:
    Drink(unsigned long time = 0, float ounces = 0);
    unsigned long getTime();
    float getOunces();
    void setTime(unsigned long time);
    void setOunces(float ounces);
    
private:
    unsigned long time;
    float ounces;
};

#endif
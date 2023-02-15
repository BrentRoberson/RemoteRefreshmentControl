#include "Drink.h"

Drink::Drink(unsigned long time, float ounces) {
    this->time = time;
    this->ounces = ounces;
}

unsigned long Drink::getTime() {
    return time;
}

float Drink::getOunces() {
    return ounces;
}

void Drink::setTime(unsigned long time) {
    this->time = time;
}

void Drink::setOunces(float ounces) {
    this->ounces = ounces;
}
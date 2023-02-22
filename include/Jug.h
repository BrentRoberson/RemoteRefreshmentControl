#ifndef JUG_H
#define JUG_H

#include <Arduino.h>
#include <Menu.h>

class Jug {
  private: 
    double startingGallons;
    double totalGallons;
    double ozLeft;
    int drinksLeft;
    int maxDrinks;

  public:
    friend class Menu;

    // add any public member functions here
};

#endif

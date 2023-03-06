#ifndef JUG_H
#define JUG_H

#include <Arduino.h>
#include <Menu.h>
#include <Globals.h>

class Jug {
  private: 

  public:
    friend class Menu;
    Jug();

    // add any public member functions here
};

void IRAM_ATTR pulseCounter();
void dispense();

#endif

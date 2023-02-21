#include <Jug.h>
#include <LiquidCrystal_I2C.h>
#include <ClickEncoder.h>
#define CLK_PIN 14
#define DT_PIN 12
#define SW_PIN 13

ClickEncoder encoder(CLK_PIN, DT_PIN, SW_PIN);
int menuOption = 0;
bool editing = false;
int buttonState = LOW;

void runMenu(LiquidCrystal_I2C lcd) {
  while (true) {
    if (!editing) {
      encoder.service();
      int encoderValue = encoder.getValue();
      if (encoderValue != 0) {
        menuOption += encoderValue;
        if (menuOption < 0) {
          menuOption = 0;
        }
        if (menuOption > 4) {
          menuOption = 4;
        }
      }
    }

    buttonState = digitalRead(SW_PIN);
    if (buttonState == LOW) {
      if (editing) {
        editing = false;
      } else {
        editing = true;
      }
    }

    if (editing) {
      encoder.service();
      int encoderValue = encoder.getValue();
      switch (menuOption) {
        case 0:
          jug.startingGallons += encoderValue * 0.1;
          break;
        case 1:
          jug.ozLeft += encoderValue;
          break;
        case 2:
          jug.drinksLeft += encoderValue;
          break;
      }
    }

    lcd.clear();
    switch (menuOption) {
      case 0:
        lcd.print("Starting Gallons:");
        lcd.setCursor(0, 1);
        lcd.print(jug.startingGallons);
        break;
      case 1:
        lcd.print("Oz Left:");
        lcd.setCursor(0, 1);
        lcd.print(jug.ozLeft);
        break;
      case 2:
        lcd.print("Drinks Left:");
        lcd.setCursor(0, 1);
        lcd.print(jug.drinksLeft);
        break;
    }
    if (editing) {
      lcd.setCursor(15, 1);
      lcd.print("*");
    }
  }
}


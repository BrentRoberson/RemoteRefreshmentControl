// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <ClickEncoder.h>

// // Initialize the library with the LCD's I2C address
// LiquidCrystal_I2C lcd(0x27, 16, 2);
// ClickEncoder encoder(2, 3, 4);

// const int buttonPin = 5;
// int buttonState = 0;
// int menuOption = 0;
// bool editing = false;

// class Jug {
//   public:
//     float startingGallons = 0;
//     float ozLeft = 0;
//     int drinksLeft = 0;
//     float drinkSize = 0;
// };

// Jug jug;

// void setup() {
//   lcd.begin();
//   lcd.backlight();
//   Serial.begin(9600);
//   pinMode(buttonPin, INPUT);
// }

// void loop() {
//   if (!editing) {
//     encoder.service();
//     int encoderValue = encoder.getValue();
//     if (encoderValue != 0) {
//       menuOption += encoderValue;
//       if (menuOption < 0) {
//         menuOption = 0;
//       }
//       if (menuOption > 4) {
//         menuOption = 4;
//       }
//     }
//   }

//   buttonState = digitalRead(buttonPin);
//   if (buttonState == LOW) {
//     if (editing) {
//       editing = false;
//     } else {
//       editing = true;
//     }
//   }

//   if (editing) {
//     encoder.service();
//     int encoderValue = encoder.getValue();
//     switch (menuOption) {
//       case 0:
//         jug.startingGallons += encoderValue * 0.1;
//         break;
//       case 1:
//         jug.ozLeft += encoderValue;
//         break;
//       case 2:
//         jug.drinksLeft += encoderValue;
//         break;
//       case 3:
//         jug.drinkSize += encoderValue * 0.1;
//         break;
//     }
//   }

//   lcd.clear();
//   switch (menuOption) {
//     case 0:
//       lcd.print("Starting Gallons:");
//       lcd.setCursor(0, 1);
//       lcd.print(jug.startingGallons);
//       break;
//     case 1:
//       lcd.print("Oz Left:");
//       lcd.setCursor(0, 1);
//       lcd.print(jug.ozLeft);
//       break;
//     case 2:
//       lcd.print("Drinks Left:");
//       lcd.setCursor(0, 1);
//       lcd.print(jug.drinksLeft);
//       break;
//     case 3:
//       lcd.print("Drink Size (oz):");
//       lcd.setCursor(0, 1);
//       lcd.print(jug.drinkSize);
//       break;
//   }
//   if (editing) {
//     lcd.setCursor(15, 1);
//     lcd.print("*");
//   }
// }

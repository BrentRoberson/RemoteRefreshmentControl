// #include <registerMenu.h>
// #include <ESPNow.h>

// long oldPosition = 0;
// long newPosition = 0;
// long initPosition = 0;
// unsigned long menuTriggeredTime = 0;
// int currentScreen = -1;
// bool updateEntireScreen = true;
// bool updateJustVal = true;
// String readTag = "";
// long rfidTriggerTime = 0;
// bool waiting = false;
// bool scanTimeout = false;
// bool tagRead;
// int validationTurns = 0;
// Customer lastScannedCustomer;

// double MAXIMUM_CURRENY_INPUT =50;

// registerMenu::registerMenu(){}

// void lcdWelcome(){
//   lcd.clear();
//   lcd.setCursor(0,0);
//   lcd.print("Welcome to RRC!");
//   lcd.setCursor(0, 1);
//   lcd.print(" Scan to add a new tag!");
// }

// void rfidScanner(){
//   readTag = rfidScan();
//   if(readTag!=""){
//     Serial.println(readTag);
//     rfidGoodTap();
//   }
// }

// void Menu::waitScreen() {
//   waiting = true;
//   lcdWelcome();
//   while(menuTriggeredTime==0 ){
//     rfidScanner();
//     if(rfidTriggerTime + 4000 < millis() && tagRead){
//       lcdWelcome();
//       tagRead = false;
//     }
//   }
// }

// void registerMenu::displayMenu() {
//   double input_amount = 0;
//   displaySetting("Input Deposited Amount:",MAXIMUM_CURRENY_INPUT);
//   processEncoderInput(input_amount);
//   broadcast_address
// // read dial input for amount
// // send this amount to the dispenser 
// // print a complete message 

// }

// template <typename T>
// void processEncoderInput(T & value) {
//   newPosition = encoder.getCount();
//   if (newPosition != oldPosition && newPosition % 2 == 0) {
//     Serial.println(newPosition);
//     if(menuTriggeredTime != 0 && currentScreen != -1) {
//       if(newPosition > oldPosition) {
//         value++;
//       } else {
//         value--;
//       }
//       //reset menu trigger time on parameter change
//       menuTriggeredTime = millis();
//       updateEntireScreen = true;
//     }
//     oldPosition = newPosition;
//   }
// }

// template <typename T>
// void displaySetting(const char* title, T value) {
//   if (updateEntireScreen) {
//     printSettingTitle();
//     lcd.setCursor(0, 1);
//     lcd.print(title);
//     lcd.setCursor(0, 2);
//     lcd.print(value);
//     updateEntireScreen = false;
//   }
// }
// #include "Menu.h"

// // long oldPosition = 0;
// // long newPosition = 0;
// // long initPosition = 0;
// // unsigned long menuTriggeredTime = 0;
// // int currentScreen = -1;
// // bool updateScreen = true;
// // bool updateJustVal = true;
// // unsigned long rfidTriggerTime = 0;
// // bool waiting = false;
// // bool scanTimeout = false;
// // String readTag = "";
// // bool newTap = false;
// // int validationTurns = 0;
// // Customer lastScannedCustomer;
// Customer lastScannedCustomer;

// // define and initialize the static member variables
// unsigned long Menu::menuTriggeredTime = 0;
// int Menu::currentScreen = -1;
// int Menu::validationTurns = 0;
// long Menu::initPosition = 0;
// long Menu::oldPosition = 0;
// bool Menu::updateScreen = true;
// int Menu::NUM_SCREENS = 6;

// Menu::Menu(){}


// void Menu::setup()  {
//   pinMode(BUTTON_PIN, INPUT_PULLUP);
//   attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Menu :: triggerMenu, FALLING);
//   //  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), [](){myMenu.triggerMenu();}, FALLING);
//   encoder.attachHalfQuad(DT, CLK);
//   encoder.setCount(0);
//   printLcdWelcome();
// }

// void Menu::displayMenu() {
//   // Serial.print(currentScreen);
//   switch (currentScreen) {
//     case 0:
//       displaySetting("Total Quarts:", totalQuarts);
//       editSetting(totalQuarts, .5, .5);
//       break;
//     case 1:
//       displaySetting("Price per ounce:", pricePerOunce);
//       editSetting(pricePerOunce, .01, .01);
//       break;
//     case 2:
//       displaySetting("Max drinks:", maxDrinks);
//       editSetting(maxDrinks, 1, 1);
//       break;
//     case 3:
//       editLastCustomerScreen("Rm", "  Removed", []() {
//         customers.pop_back();
//       });
//       break;
//     case 4:
//       editLastCustomerScreen("Mgr", "Made Manager", []() {
//         lastScannedCustomer.manager = true;
//       });
//       break;
//       //open door
//     case 5:
//       openDoorScreen();
//       break;

//     //make a case to resetAll
//   }
// }


// void Menu::run() {
//   if(menuTriggeredTime != 0 && currentScreen != -1) {
//     displayMenu();
//     if(menuTriggeredTime + 4000 < millis()) {
//       validationTurns = 0; //set this back to 0 if a successful validation has occurred (validationTurns = -1)
//       menuTriggeredTime = 0;
//       currentScreen = -1;
//       encoder.setCount(initPosition);
//       oldPosition = initPosition;
//       newPosition = initPosition;
//       initPosition = 0;
//       digitalWrite(DOOR_LOCK,LOW);
//       waitScreen();
//     }
//   }
//   else if(!waiting){
//     waitScreen();
//   }
// }


// void Menu::waitScreen() {
//   waiting = true;
//   printLcdWelcome();

//   while(menuTriggeredTime==0 ){
//     readTag = rfidScan();
    
//     if(readTag!=""){
//       rfidTriggerTime = millis();
//       newTap = true;
//       int customerIndex = customers.search(readTag);
//       lcd.clear();
//       lcd.setCursor(0,0);
//         if(customerIndex>-1)
//         {
//           rfidGoodTap();
//           lastCustomerScanned = customers[customerIndex];
//           lastCustomerScanned.lcdPrint();
//         }
//         else{
//           rfidBadTap();
//           lcd.print("Unknown ID");
//           lcd.setCursor(0,1);
//           lcd.print("Please check in");
//           Serial.print("Customer not added");
//           Serial.println("(For testing purposes, customer is added) ");
//           customers.push_back(Customer(readTag, rand()%50));
//         } 
        
//       }
//     if(rfidTriggerTime + 4000 < millis() && newTap){
//       printLcdWelcome();
//       newTap = false;
//     }
//   }
// }

// void Menu::clearLCDLine(int line)
// {               
//   lcd.setCursor(0,line);
//   for(int n = 0; n < 20; n++) //
//   {
//     lcd.print(" ");
//   }
// }

// void Menu:: triggerMenu()
// {
//   if(menuTriggeredTime + 350 < millis()){
//     if(menuTriggeredTime == 0) {
//       initPosition = oldPosition;
//     }
//     menuTriggeredTime = millis();
//     currentScreen++;
//     if(currentScreen >= NUM_SCREENS) {
//       currentScreen = -1;
//     }
//     validationTurns = 0;
//     updateScreen = true;
//   }
// }

// template <typename T>
// void Menu:: editSetting(T & value, double increment, double decrement) {
//   newPosition = encoder.getCount();
//   if (newPosition != oldPosition && newPosition % 2 == 0) {
//     // Serial.println(newPosition);
//     if(menuTriggeredTime != 0 && currentScreen != -1) {
//       if(newPosition > oldPosition) {
//         value += increment;
//       } else {
//         value -= decrement;
//       }
//       //reset menu trigger time on parameter change
//       menuTriggeredTime = millis();
//       updateScreen = true;
//     }
//     oldPosition = newPosition;
//   }
// }

// void Menu:: printSettingTitle(){
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print(" ***  SETTINGS  *** ");
// }

// template <typename T>
// void Menu:: displaySetting(const char* title, T value) {
//   if (updateScreen) {
//     printSettingTitle();
//     lcd.setCursor(0, 1);
//     lcd.print(title);
//     lcd.setCursor(0, 2);
//     lcd.print(value);
//     updateScreen = false;
//   }
// }


// bool Menu:: validated(){
//   //if we are not waiting for a successful validation to complete
//   if (validationTurns!=-1)
//   {
//     editSetting(validationTurns, 1, -1);
//   }
//   //if turned 5 times, return true for validated
//   return(validationTurns>=5);
// }

// void Menu:: editLastCustomerScreen(String title, String action, std::function<void()> onValidation) {
//   if (updateScreen) {
//     printSettingTitle();
//     lcd.setCursor(0, 1);
//     lcd.print("Last Customer: ");
//     lcd.print(title);
//     lcd.setCursor(0, 2);
//     if (customers.getSize() > 0) {
//       lcd.print("Balance: $");
//       lcd.print(lastScannedCustomer.balance);
//       lcd.setCursor(0, 3);
//       lcd.print(5 - validationTurns);
//       lcd.print(" turns to validate");
//     } else {
//       lcd.print("No Customers Scanned");
//     }
//     updateScreen = false;
//   }
//   //if there are customers
//   if (customers.getSize() > 0 && validated()) {
//     lcd.clear();
//     lcd.setCursor(3, 1);
//     lcd.print("Last Customer ");
//     lcd.setCursor(3, 2);
//     lcd.print(action);
//     onValidation();
//     validationTurns = -1; //successful validation
//     updateScreen = false;
//     menuTriggeredTime -= 2000;
//   }
// }

// void Menu::openDoorScreen(){
//   if(updateScreen){
//     lcd.clear();
//     lcd.setCursor(0,0);
//     lcd.print("Turn to Open Door");
//     lcd.setCursor(0, 1);
//     lcd.print(5 - validationTurns);
//     lcd.print(" Turns Left");
//     updateScreen = false;
//   }
//   if (validated())
//   {
//     lcd.clear();
//     lcd.setCursor(0,0);
//     lcd.print("Door Opened!");
//     digitalWrite(DOOR_LOCK,HIGH);
//     validationTurns = -1; //successful validation
//     menuTriggeredTime -= 2000;
//     updateScreen = false;

//   }
// } 

// void Menu::printLcdWelcome(){
//   lcd.clear();
//   lcd.setCursor(0,0);
//   lcd.print("Welcome to RRC!");
//   lcd.setCursor(0, 1);
//   lcd.print(" Scan to Begin!");
//   lcd.setCursor(0, 2);
//   lcd.print("Price/Oz: $");
//   lcd.print(pricePerOunce);
//   lcd.setCursor(0, 3);
//   lcd.print("Oz Left: ");
//   lcd.print(totalQuarts*32.0);
//   lcd.print("oz");
// }

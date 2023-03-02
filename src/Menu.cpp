#include "Menu.h"

// long oldPosition = 0;
// long newPosition = 0;
// long initPosition = 0;
// unsigned long menuTriggeredTime = 0;
// int currentScreen = -1;
// bool updateEntireScreen = true;
// bool updateJustVal = true;
// unsigned long rfidTriggerTime = 0;
// bool waiting = false;
// bool scanTimeout = false;
// String readTag = "";
// bool newTap = false;
// int validationTurns = 0;
// Customer lastScannedCustomer;
Customer lastScannedCustomer;

// define and initialize the static member variables
unsigned long Menu::menuTriggeredTime = 0;
int Menu::currentScreen = -1;
int Menu::validationTurns = 0;
long Menu::initPosition = 0;
long Menu::oldPosition = 0;
bool Menu::updateEntireScreen = true;
int Menu::NUM_SCREENS = 5;

Menu::Menu(){}


void Menu::setup()  {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Menu :: triggerMenu, FALLING);
  //  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), [](){myMenu.triggerMenu();}, FALLING);
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(0);
  printLcdWelcome();
}

void Menu::displayMenu() {
  // Serial.print(currentScreen);
  switch (currentScreen) {
    case 0:
      displaySetting("Total Quarts:", totalQuarts);
      editSetting(totalQuarts, .5);
      break;
    case 1:
      displaySetting("Price per ounce:", pricePerOunce);
      editSetting(pricePerOunce, .01);
      break;
    case 2:
      displaySetting("Max drinks:", maxDrinks);
      editSetting(maxDrinks, 1);
      break;
    case 3:
      editLastCustomerScreen("Rm", "  Removed", []() {
        customers.pop_back();
      });
      break;
    case 4:
      editLastCustomerScreen("Mgr", "Made Manager", []() {
        lastScannedCustomer.manager = true;
      });
      break;
      //open door
    case 5:
      openDoor();
      break;

    //make a case to resetAll
  }
}


void Menu::run() {
  if(menuTriggeredTime != 0 && currentScreen != -1) {
    displayMenu();
    if(menuTriggeredTime + 20000 < millis()) {

      menuTriggeredTime = 0;
      currentScreen = -1;
      encoder.setCount(initPosition);
      oldPosition = initPosition;
      newPosition = initPosition;
      initPosition = 0;
      waitScreen();
    }
  }
  else if(!waiting){
    waitScreen();
  }
}


void Menu::waitScreen() {
  waiting = true;
  printLcdWelcome();

  while(menuTriggeredTime==0 ){
    readTag = rfidScan();
    
    if(readTag!=""){
      rfidTriggerTime = millis();
      newTap = true;
      int customerIndex = customers.search(readTag);
      lcd.clear();
      lcd.setCursor(0,0);
        if(customerIndex>-1)
        {
          rfidGoodTap();
          lastCustomerScanned = customers[customerIndex];
          lastCustomerScanned.lcdPrint();
        }
        else{
          rfidBadTap();
          lcd.print("Unknown ID");
          lcd.setCursor(0,1);
          lcd.print("Please check in");
          Serial.print("Customer not added");
          Serial.println("(For testing purposes, customer is added) ");
          customers.push_back(Customer(readTag, rand()%50));
        } 
        
      }
    // Serial.println(readTag);
    // Serial.println(rfidTriggerTime);

    // Serial.println();
    // Serial.println();
    if(rfidTriggerTime + 4000 < millis() && newTap){
      printLcdWelcome();
      newTap = false;
    }
  }
}

void Menu::clearLCDLine(int line)
{               
  lcd.setCursor(0,line);
  for(int n = 0; n < 20; n++) // 20 indicates symbols in line. For 2x16 LCD write - 16
  {
    lcd.print(" ");
  }
}

void Menu:: triggerMenu()
{
  if(menuTriggeredTime + 500 < millis()){
    if(menuTriggeredTime == 0) {
      initPosition = oldPosition;
    }
    menuTriggeredTime = millis();
    currentScreen++;
    if(currentScreen >= NUM_SCREENS) {
      currentScreen = -1;
    }
    validationTurns = 0;
    updateEntireScreen = true;
  }
}

template <typename T>
void Menu:: editSetting(T & value, double increment) {
  newPosition = encoder.getCount();
  if (newPosition != oldPosition && newPosition % 2 == 0) {
    // Serial.println(newPosition);
    if(menuTriggeredTime != 0 && currentScreen != -1) {
      if(newPosition > oldPosition) {
        value += increment;
      } else {
        value -= increment;
      }
      //reset menu trigger time on parameter change
      menuTriggeredTime = millis();
      updateEntireScreen = true;
    }
    oldPosition = newPosition;
  }
}

void Menu:: printSettingTitle(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ***  SETTINGS  *** ");
}

template <typename T>
void Menu:: displaySetting(const char* title, T value) {
  if (updateEntireScreen) {
    printSettingTitle();
    lcd.setCursor(0, 1);
    lcd.print(title);
    lcd.setCursor(0, 2);
    lcd.print(value);
    updateEntireScreen = false;
  }
}


bool Menu:: validated(){
  // Serial.println(validationTurns);
  editSetting(validationTurns,1);
  //if turned 5 times, return true for validated
  return(validationTurns>=5 || validationTurns<=-5);
}

void Menu:: editLastCustomerScreen(String title, String action, std::function<void()> onValidation) {
  if (updateEntireScreen) {
    printSettingTitle();
    lcd.setCursor(0, 1);
    lcd.print("Last Customer: ");
    lcd.print(title);
    lcd.setCursor(0, 2);
    if (customers.getSize() > 0) {
      lcd.print("Balance: $");
      lcd.print(lastScannedCustomer.balance);
      lcd.setCursor(0, 3);
      lcd.print(5 - abs(validationTurns));
      lcd.print(" turns to validate");
    } else {
      lcd.print("No Customers Scanned");
    }
    updateEntireScreen = false;
  }
  //if there are customers
  if (customers.getSize() > 0 && validated()) {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("Last Customer ");
    lcd.setCursor(3, 2);
    lcd.print(action);
    onValidation();
    validationTurns = 0;
    updateEntireScreen = false;
    menuTriggeredTime -= 2000;
  }
}

void Menu::openDoor(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Turn to Open Door");
  lcd.setCursor(0, 1);
  if (validated())
  {
    digitalWrite(DOOR_LOCK,HIGH);
    delay(2000);
    digitalWrite(DOOR_LOCK,LOW);
    delay(2000);

  }
  lcd.print(validationTurns);
  lcd.print(" Turns Left");
} 

void Menu::printLcdWelcome(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to RRC!");
  lcd.setCursor(0, 1);
  lcd.print(" Scan to Begin!");
  lcd.setCursor(0, 2);
  lcd.print("Price/Oz: $");
  lcd.print(pricePerOunce);
  lcd.setCursor(0, 3);
  lcd.print("Oz Left: ");
  lcd.print(totalQuarts*32.0);
  lcd.print("oz");
}

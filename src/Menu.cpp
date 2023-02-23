#include "Menu.h"

long oldPosition = 0;
long newPosition = 0;
long initPosition = 0;
unsigned long menuTriggeredTime = 0;
int currentScreen = -1;
bool updateEntireScreen = true;
bool updateJustVal = true;
String readTag = "";
long rfidTriggerTime = 0;
bool waiting = false;
bool scanTimeout = false;
bool tagRead;
Customer lastScannedCustomer;

Menu::Menu(){}

void lcdWelcome(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to RRC!");
  lcd.setCursor(0, 1);
  lcd.print("Today's Drink: ");
  lcd.print(drinkOTD);
  lcd.setCursor(0, 2);
  lcd.print("Price/Oz: $");
  lcd.print(pricePerOunce);
  lcd.setCursor(0, 3);
  lcd.print("Oz Left: ");
  lcd.print(totalQuarts/32.0);
  lcd.print("oz");
}

void Menu::setup()  {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Menu :: triggerMenu, FALLING);
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(0);
  lcdWelcome();
}



void Menu::run() {
  if(menuTriggeredTime != 0 && currentScreen != -1) {
    displayMenu();
    if(menuTriggeredTime + 4000 < millis()) {
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

void rfidScanner(){
  // Serial.print(menuTriggeredTime);
  // Serial.print(rfidTriggerTime);
  readTag = waitForTag();
  if(readTag!=""){
    int customerIndex = customers.search(readTag);
    rfidTriggerTime = millis();
    tagRead = true;
    if(customerIndex>-1)
    {
      lastScannedCustomer = customers[customerIndex];
      rfidGoodTap();
      customers[customerIndex].lcdPrint();
      //print "press green button to dispense
      //enter drink menued itCustomer/buy drink
      //make this a public function in the customer class, make the array private var
      customers[customerIndex].drinks->push_back(Drink(rand()%24, rand()%2000));
      customers[customerIndex].balance -= rand()%6;
      customers[customerIndex].print();
    }
    else{
      rfidBadTap();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Card Unrecognized");
      lcd.setCursor(2,2);
      lcd.print("Please check in");
      lcd.setCursor(3,3);
      lcd.print("at front desk");
      Serial.print("Customer not added, please go to check in to add your card. ");
      Serial.println("For testing purposes, customer is added: ");
      customers.push_back(Customer(readTag, rand()%50));
    }
    Serial.println(readTag);
    
  }
}

void Menu::waitScreen() {
  waiting = true;
  lcdWelcome();

  while(menuTriggeredTime==0 ){
    rfidScanner();

    if(rfidTriggerTime + 4000 < millis() && tagRead){
      lcdWelcome();
      tagRead = false;
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
      currentScreen = 0;
    }
    updateEntireScreen = true;
  }
}

template <typename T>
void processEncoderInput(T & value) {
  
  newPosition = encoder.getCount();


  if (newPosition != oldPosition && newPosition % 2 == 0) {
    Serial.println(newPosition);

    if(menuTriggeredTime != 0 && currentScreen != -1) {
      if(newPosition > oldPosition) {
        value++;
      } else {
        value--;
      }
      //reset menu trigger time on parameter change
      menuTriggeredTime = millis();
      updateJustVal = true;
    }
    oldPosition = newPosition;
  }
}



void Menu::displayMenu() {
  if (updateEntireScreen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ***  SETTINGS  *** ");
  }

  switch (currentScreen) {
    case 0:
      if (updateEntireScreen) {
        lcd.setCursor(0, 1);
        lcd.print("Total Quarts:");
        lcd.setCursor(0, 2);
        lcd.print(totalQuarts);
        updateEntireScreen = false;
      }
      processEncoderInput(totalQuarts);
      break;
    case 1:
      if (updateEntireScreen) {
        lcd.setCursor(0, 1);
        lcd.print("Price per ounce:");
        lcd.setCursor(0, 2);
        lcd.print(pricePerOunce);
        updateEntireScreen = false;
      }
      processEncoderInput(pricePerOunce);
      break;
    case 2:
      if (updateEntireScreen) {
        lcd.setCursor(0, 1);
        lcd.print("Max drinks:");
        lcd.setCursor(0, 2);
        lcd.print(maxDrinks);
        updateEntireScreen = false;
      }
      processEncoderInput(maxDrinks);
      break;
    case 3:
      if (updateEntireScreen) {
        lcd.setCursor(0, 1);
        lcd.print("Last scanned:");
        lcd.setCursor(0, 2);
        if (customers.getSize() > 0) {
          lastScannedCustomer.lcdPrint();
        } else {
          lcd.print("No Customers Scanned");
        }
        updateEntireScreen = false;
      }
      break;
    case 4:
      if (updateEntireScreen) {
        lcd.setCursor(0, 1);
        lcd.print("All Customers");
        lcd.setCursor(0, 2);
        if (customers.getSize() > 0) {
          lastScannedCustomer.lcdPrint();
        } else {
          lcd.print("No Customers Scanned");
        }
      }
      updateEntireScreen = false;
      break;
  }

  
}
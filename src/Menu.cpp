#include "Menu.h"

long oldPosition = 0;
long newPosition = 0;
long initPosition = 0;
unsigned long menuTriggeredTime = 0;
int currentScreen = -1;
bool updateEntireScreen = true;
String readTag = "";
long rfidTriggerTime = 0;
bool waiting = false;

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
  lcd.print(ouncesLeft);
  lcd.print("oz");
}

void Menu::setup()  {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Menu :: triggerMenu, FALLING);
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(-999);
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
      initPosition = -999;
      waitScreen();
      
    }
  }
  else if(!waiting){
    waitScreen();
  }
  delay(10);
}


void Menu::waitScreen() {
  waiting = true;
  lcdWelcome();
  //rfid code so RFID still works
  while(menuTriggeredTime=0){
    Serial.print(menuTriggeredTime);
    Serial.print(rfidTriggerTime);
    readTag = waitForTag();
    if(readTag!=""){
      int customerIndex = customers.search(readTag);
      rfidTriggerTime = millis();
      if(customerIndex>-1)
      {
        rfidGoodTap();
        customers[customerIndex].lcdPrint();
        //print "press green button to dispense
        //enter drink menueditCustomer/buy drink
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
      readTag = "";
    }
    //get off of purchase screen
    // if(rfidTriggerTime+4000<millis()){
    //   break;
    // }

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
  if(menuTriggeredTime + 100 < millis()){
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

void Menu:: displayMenu() {
  //parameter editing
  // long newPosition = encoder.getCount();
  // if (newPosition != oldPosition && newPosition % 2 == 0) {
  //   Serial.println(newPosition);
  //   if(menuTriggeredTime != 0 && currentScreen != -1) {
  //     if(newPosition > oldPosition) {
  //       parameters[currentScreen]++;
  //     } else {
  //       parameters[currentScreen]--;
  //     }
  //     //reset menu trigger time on parameter change
  //     menuTriggeredTime = millis();
  //     updateEntireScreen = true;
  //   }
  //   oldPosition = newPosition;
  // }

  if(updateEntireScreen) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" ***  SETTINGS  *** ");
    lcd.setCursor(0,1);
    lcd.print(currentScreen);
    
    updateEntireScreen = false;
  }
}
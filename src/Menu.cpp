#include "Menu.h"

// Static member variable definitions
long Menu::oldPosition = -999;
long Menu::initPosition = -999;
unsigned long Menu::menuTriggeredTime = 0;
const int Menu::numOfScreens = 6;
int Menu::currentScreen = -1;
bool Menu::updateScreen = true;

Menu::Menu(){}


void Menu::setup()  {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Menu :: triggerMenu, FALLING);
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(-999);
  //initScreen();
}

void Menu::run() {
  long newPosition = encoder.getCount();
  if (newPosition != oldPosition && newPosition % 2 == 0) {
    Serial.println(newPosition);
    printPosition(newPosition / 2);
    if(menuTriggeredTime != 0 && currentScreen != -1) {
      if(newPosition > oldPosition) {
        parameters[currentScreen]++;
      } else {
        parameters[currentScreen]--;
      }
      //reset menu trigger time on parameter change
      menuTriggeredTime = millis();
      updateScreen = true;
    }
    oldPosition = newPosition;
  }

  if(menuTriggeredTime != 0 && currentScreen != -1) {
    displayMenu();
    if(menuTriggeredTime + 4000 < millis()) {
      menuTriggeredTime = 0;
      currentScreen = -1;
      Serial.println("Init pos:");
      Serial.println(initPosition);
      encoder.setCount(initPosition);
      oldPosition = initPosition;
      newPosition = initPosition;
      initPosition = -999;
      printPosition(oldPosition / 2);
      clearLCDLine(2);
      lcd.setCursor(5,2);
      lcd.print("Subscribe!");
      lcd.setCursor(3,3);
      lcd.print("Taste The Code");
    }
  } else {
    //time
  }

  delay(10);
}

void Menu::initScreen() {
  
  lcd.clear();
// double init clears any previous text
  // Print a message to the LCD.
  lcd.setCursor(3,0);
  lcd.print("Jungle Juice Sluice");
  lcd.setCursor(4,1);
  lcd.print("LCD Test");
  lcd.setCursor(5,2);
  lcd.print("Does it work????");
  lcd.setCursor(3,3);
  lcd.print("Pls make it work");
}

void Menu::printPosition(long pos) {
  clearLCDLine(1);
  lcd.setCursor(0,1);
  lcd.print("Position: ");
  lcd.print(pos);
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
  if(menuTriggeredTime + 50 < millis()){
    if(menuTriggeredTime == 0) {
      initPosition = oldPosition;
    }
    menuTriggeredTime = millis();
    currentScreen++;
    if(currentScreen >= numOfScreens) {
      currentScreen = 0;
    }
    updateScreen = true;
  }
}

void Menu:: displayMenu() {
  if(updateScreen) {
    lcd.clear();
    lcd.print(" ***  SETTINGS  *** ");
    lcd.setCursor(0,1);
    lcd.print(screens[currentScreen][0]);
    lcd.setCursor(0,2);
    lcd.print(parameters[currentScreen]);
    lcd.print(" ");
    lcd.print(screens[currentScreen][1]);
    updateScreen = false;
  }
}
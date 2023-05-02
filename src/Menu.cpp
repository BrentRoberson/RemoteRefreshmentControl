
#include <Menu.h>



Menu::Menu() {
  oldPosition = 0;
  newPosition = 0;
  initPosition = 0;
  buttonJustPressed = 0;
  settingsTriggeredTime=0;
  dispenseLastTouched=0;
  //0: Waiting 1: Dispensing 2: Settings 
  currentScreen = 0;
  updateScreen = 0;
  rfidTriggerTime = 0;
  waiting = false;
  scanTimeout = false;
  String readTag = "";
  newTap = false;
  addAmount = 15;
  currentSetting = 0;

  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(0);

}


void Menu:: printLcdWelcome(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to BarBox!");
  lcd.setCursor(0, 1);
  lcd.print("Scan to Begin!");
  lcd.setCursor(0, 2);
  lcd.print("Price/Oz: $");
  lcd.print(pricePerOunce);
  lcd.setCursor(0, 3);
  lcd.print("Oz Left: ");
  lcd.print(totalQuarts*32.0);
  lcd.print("oz");

}


template <typename T>
void Menu::editSetting(T & value, float increment, float decrement) {
  newPosition = encoder.getCount();
  if (newPosition != oldPosition && newPosition % 2 == 0) {
    if(newPosition > oldPosition) {
      value += increment;
    } else {
      value -= decrement;
    }
    updateScreen = true;
    oldPosition = newPosition;
    settingsTriggeredTime = millis();
  }
}

void Menu:: printSettingTitle(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ***  SETTINGS  *** ");
    lcd.setCursor(0,1);
}


template <typename T>
void Menu:: displaySetting(const char* title, T value) {
  if (updateScreen) {
    printSettingTitle();
    lcd.print(title);
    lcd.setCursor(0, 2);
    lcd.print(value);
    updateScreen = false;
  }
}


void Menu:: editCustOnSwipe(String title, String action, std::function<void()> onSwipe) {
  if (updateScreen) {
    printSettingTitle();
    lcd.setCursor(0, 1);
    lcd.print("Swipe Card To");
    lcd.setCursor(0, 2);
    lcd.print(title); // make manager, //remove customer //open door
    newTap = false;
    updateScreen = false;
  }
  readTag = rfidScan();
  if(readTag!=""){
    rfidTriggerTime = millis();
    settingsTriggeredTime = millis();
    newTap = true;
    int customerIndex = customers.search(readTag);
    printSettingTitle();
    if(customerIndex>-1)
    {
      addedTap();
      currentScannedIndex = customerIndex;
      onSwipe();
      lcd.print("Customer ");
      lcd.setCursor(0,2);
      lcd.print(action); //Made Manager! or Removed!
    }
    else{
      newAddedTap();
      lcd.print("New Card");
      customers.push_back(Customer(readTag, 0));
      currentScannedIndex = -1;
      lcd.setCursor(0,2);
      lcd.print(action); //Made Manager! or Removed!
    } 
    SdData.addOrUpdateCustomer(customers[currentScannedIndex]);

  }
  if(rfidTriggerTime + 1500 < millis() && newTap){
    newTap = false;
    updateScreen = true;
  }

}

void Menu:: addOrSetMoneyOnSwipe(bool setAmount){
  editSetting(addAmount,1,1);
  if (updateScreen) {
    printSettingTitle();
    lcd.setCursor(0, 1);
    lcd.print("Swipe Card To");
    lcd.setCursor(0, 2);
    if(setAmount){
      lcd.print("Set: $");
    } else{
      lcd.print("Add: $");
    }
    lcd.print(addAmount);
    newTap = false;
    updateScreen = false;
  }
  readTag = rfidScan();
  if(readTag!=""){
    Customer temp = Customer();
    rfidTriggerTime = millis();
    settingsTriggeredTime = millis();
    newTap = true;
    int customerIndex = customers.search(readTag);
    printSettingTitle();
    if(customerIndex>-1)
    {
      addedTap();
      currentScannedIndex = customerIndex;
      lcd.print("Customer ");
      lcd.setCursor(0,1);
      if(setAmount){
        lcd.print("Set: $");
      } else{
        lcd.print("Added: $");
      }
      lcd.print(addAmount);
      temp = customers[currentScannedIndex];
      if(setAmount){
        customers[currentScannedIndex].balance = addAmount;
      }else{
        customers[currentScannedIndex].balance += addAmount;
      }
      
    }
    else{
      newAddedTap();
      lcd.print("New Card");
      lcd.print("Given $");
      lcd.print(addAmount);
      temp = Customer(readTag,addAmount);
      customers.push_back(temp);
    } 
    SdData.addOrUpdateCustomer(temp);

  }
  if(rfidTriggerTime + 2000 < millis() && newTap){
    newTap = false;
    updateScreen = true;
  }
}



void Menu:: settingsScreen(){
  delay(100);
  settingsTriggeredTime = millis();
  //exits the while loop when current setting exceeds the NUM_SETTINGS
  while(settingsTriggeredTime + 10000 > millis() && currentSetting < NUM_SETTINGS && currentSetting >= 0){
    //check for website updates
    handleClient();

    encoderButton.update();
    if(encoderButton.isSingleClick()){
      currentSetting += 1;
      updateScreen = true;
      settingsTriggeredTime = millis();
    }
    if(encoderButton.isDoubleClick()){
      currentSetting -= 1 ; //effectively returns to waitScreen
      settingsTriggeredTime = millis();
      updateScreen = true;
    }
    if(encoderButton.isLongClick()){
      currentSetting = 99; //effectively returns to waitScreen

    }
    switch (currentSetting) {
      case 0:
        displaySetting("Total Quarts:", totalQuarts);
        editSetting(totalQuarts, .5, .5);
        break;
      case 1:
        displaySetting("Price per ounce:", pricePerOunce);
        editSetting(pricePerOunce, .01, .01);
        break;
      // case 2: make max ounces
      //   displaySetting("Max drinks:", maxOunces);
      //   editSetting(maxOunces, 1, 1);
      //   break;
      case 2:
        editCustOnSwipe("Remove Customer", "Removed!", removeCustomer);
        break;
      case 3:
        editCustOnSwipe("Make Manager", "Made Manager!", makeManager);
        break;
        //open door
      case 4:
        editCustOnSwipe("Open Door", "Door Opened!", openDoor);
        break;
      case 5:
        addOrSetMoneyOnSwipe(false);
        break;
      case 6:
        addOrSetMoneyOnSwipe(true);
        break;
      case 7:
        if(updateScreen){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("reset all");
          updateScreen = false;
        }
        break;
      //make a case to resetAll
    }
  }
  SdData.updateSettings();
  currentSetting = 0;
  settingsTriggeredTime = 0;
  currentScreen = 0;
  updateScreen = true;
}

void Menu:: waitScreen(){
  rainbowCycle(50);
  if (updateScreen){
    updateScreen = false;
    printLcdWelcome();
  }
  //check for website updates
  handleClient();
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
      currentScannedIndex = customerIndex;
      newTap = false;
      currentScreen = 1; //dispense
      updateScreen = true;
    }
    else{
      rfidBadTap();
      lcd.print("Unknown ID");
      lcd.setCursor(0,1);
      lcd.print("Please check in");
      Serial.print("Customer not added");
      changeColor(0,255,0);
      while(rfidScan(500)!=""){

      }
      updateScreen = true;
    } 
  }
}

void Menu:: dispenseScreen(){
  updateScreen = true;
  while(rfidScan(1000)!=""){
    //check for website updates
    handleClient();
    if(updateScreen){
      delay(100);
      customers[currentScannedIndex].lcdPrint();
      updateScreen = false;
      changeColor(random(255),random(100,255),random(255));
    }
    encoderButton.update();
    if(encoderButton.isSingleClick()){
      if(customers[currentScannedIndex].manager) {
        currentSetting = 0;
        currentScreen = 2;
        updateScreen= true;
        return;
      }
      else {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Manager Not Found");
        delay(2000);
        updateScreen = true;
      }
    }
    //may need to change to low later, check limit switch functionality
    if(digitalRead(DISPENSE_BUTTON)==LOW){
      dispense();
    }
  }
  currentScreen = 0;
  updateScreen = true;
  delay(100);

}


void Menu:: run(){
  switch(currentScreen){
    case 0: 
      waitScreen();
      break;

    case 1:
      dispenseScreen();
      break;

    case 2:
      settingsScreen();
      break;
  }
}
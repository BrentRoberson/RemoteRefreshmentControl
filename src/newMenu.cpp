
#include <newMenu.h>



NewMenu::NewMenu() {
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


void NewMenu:: printLcdWelcome(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to RRC!");
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
void NewMenu::editSetting(T & value, float increment, float decrement) {
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

void NewMenu:: printSettingTitle(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ***  SETTINGS  *** ");
    lcd.setCursor(0,1);
}


template <typename T>
void NewMenu:: displaySetting(const char* title, T value) {
  if (updateScreen) {
    printSettingTitle();
    lcd.print(title);
    lcd.setCursor(0, 2);
    lcd.print(value);
    updateScreen = false;
  }
}


void NewMenu:: editCustOnSwipe(String title, String action, std::function<void()> onSwipe) {
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

void NewMenu:: addMoneyOnSwipe(bool setAmount){
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
      customers.push_back(Customer(readTag, addAmount));
      temp = Customer(readTag,addAmount);
    } 

    SdData.addOrUpdateCustomer(temp);

  }
  if(rfidTriggerTime + 1500 < millis() && newTap){
    newTap = false;
    updateScreen = true;
  }
}



void NewMenu:: settingsScreen(){
  delay(100);
  settingsTriggeredTime = millis();
  //exits the while loop when current setting exceeds the NUM_SETTINGS
  while(settingsTriggeredTime + 10000 > millis() && currentSetting < NUM_SETTINGS){

    //ESP32_ISR_Disable(ENCODER_BUTTON);
    encoderButton.update();
    if(encoderButton.isSingleClick()){
      currentSetting +=1;
      updateScreen = true;
      settingsTriggeredTime = millis();
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
      //   displaySetting("Max drinks:", maxDrinks);
      //   editSetting(maxDrinks, 1, 1);
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
        addMoneyOnSwipe(false);
        break;
      case 6:
        addMoneyOnSwipe(true);
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
  currentSetting = 0;
  settingsTriggeredTime = 0;
  currentScreen = 0;
  updateScreen = true;

}

void NewMenu:: waitScreen(){
  rainbowCycle(10);
  if (updateScreen){
    updateScreen = false;
    printLcdWelcome();
  }
  if (new_sd_data){
    SdData.addOrUpdateCustomer(curr_cust);
    new_sd_data = false;
    curr_cust = Customer();
  }
  encoderButton.update();
  readTag = rfidScan();
  encoderButton.update();
  if(encoderButton.isSingleClick()){

    if(customers[currentScannedIndex].manager && rfidTriggerTime +4000 >millis()) {
      updateScreen = true;
      currentSetting = 0;
      currentScreen = 2;
    }
    else {
      lcd.clear();
      lcd.setCursor(0,2);
      lcd.print("Manager Not Found");
      delay(2000);
      updateScreen = true;
    }
    
  }

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
    } 
      
    }
  if(rfidTriggerTime + 4000 < millis() && newTap){
    newTap = false;
    updateScreen = true;
  }
}

void NewMenu:: dispenseScreen(){
  bool done = false;
  dispenseLastTouched = millis();
  updateScreen = true;
  while(dispenseLastTouched + 10000 > millis() && !done){
    if(updateScreen){
      customers[currentScannedIndex].lcdPrint();
      updateScreen = false;
    }
    if(digitalRead(DISPENSE_BUTTON)==LOW){
      dispense();
      dispenseLastTouched = millis();
    }
    if(digitalRead(DONE_BUTTON)==LOW){
      done = true;
    }
  }
  dispenseLastTouched = 0;
  currentScreen = 0;
  updateScreen = true;
  delay(100);

}


void NewMenu:: run(){
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
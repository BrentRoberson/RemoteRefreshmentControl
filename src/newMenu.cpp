
#include <newMenu.h>
//#include <ESP32_ISR_Servo.h>

static int NUM_SETTINGS = 7;
static long oldPosition = 0;
long newPosition = 0;
static long initPosition = 0;
static unsigned long buttonJustPressed = 0;
static unsigned long settingsTriggeredTime=0;
static unsigned long dispenseLastTouched=0;
//0: Waiting 1: Dispensing 2: Settings 
static int currentScreen = 0;
static bool updateScreen = 0;
unsigned long rfidTriggerTime = 0;
bool waiting = false;
bool scanTimeout = false;
String readTag = "";
bool newTap = false;
int addAmount = 15;
static int validationTurns=0;
static int currentSetting = 0;
unsigned long lastButtonPressTime = 0; // Initialize variable to store the last button press time
const unsigned long debounceDelay = 50; // Set debounce delay to 50 milliseconds
unsigned long currentTime = 0; // Get the current time in milliseconds
bool encoderPressed = 0; // Read the button state
PinButton encoderButton(ENCODER_BUTTON);
PinButton doneButton(DONE_BUTTON);


void printLcdWelcome(){
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

void menuSetup()  {
  pinMode(DISPENSE_BUTTON, INPUT);
  pinMode(DONE_BUTTON, INPUT);
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(0);
  printLcdWelcome();
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);

}

bool validated(){
  //if we are not waiting for a successful validation to complete
  if (validationTurns!=-1)
  {
    editSetting(validationTurns, 1, -1);
  }
  //if turned 5 times, return true for validated
  return(validationTurns>=5);
}


template <typename T>
void editSetting(T & value, double increment, double decrement) {
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

void printSettingTitle(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ***  SETTINGS  *** ");
    lcd.setCursor(0,1);
}


template <typename T>
void displaySetting(const char* title, T value) {
  if (updateScreen) {
    printSettingTitle();
    lcd.print(title);
    lcd.setCursor(0, 2);
    lcd.print(value);
    updateScreen = false;
  }
}

// void openDoorScreen(){
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
//     lcd.setCursor(3,1);
//     lcd.print("Door Opened!");
//     digitalWrite(DOOR_LOCK,HIGH);
//     validationTurns = -1; //successful validation
//     updateScreen = false;
//     delay(5000);
//     digitalWrite(DOOR_LOCK,LOW);
//     currentSetting +=1;

//   }
// } 

void openDoor(){
  digitalWrite(DOOR_LOCK, HIGH);
  delay(2000);
  digitalWrite(DOOR_LOCK, LOW);
}

void doOnSwipe(String title, String action, std::function<void()> onSwipe) {
  if (updateScreen) {
    printSettingTitle();
    lcd.setCursor(0, 1);
    lcd.print("Swipe Card To");
    lcd.setCursor(0, 2);
    lcd.print(title); // make manager, //remove customer //open door
    Serial.print("in update screen: New Tap is: ");
    Serial.println(newTap);
    newTap = false;
    updateScreen = false;
  }
  readTag = rfidScan();
  if(readTag!=""){
    Serial.println("in readTag if");
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
      lcd.setCursor(0,2);
      lcd.print(action); //Made Manager! or Removed!
    } 

  }
  if(rfidTriggerTime + 1500 < millis() && newTap){
    newTap = false;
    updateScreen = true;
  }

}



void addMoneyOnSwipe(){
  editSetting(addAmount,1,1);
  if (updateScreen) {
    printSettingTitle();
    lcd.setCursor(0, 1);
    lcd.print("Swipe Card To");
    lcd.setCursor(0, 2);
    lcd.print("Add: $");
    lcd.print(addAmount);
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
      lcd.print("Customer ");
      lcd.setCursor(0,1);
      lcd.print("Added $");
      lcd.print(addAmount);
      customers[currentScannedIndex].balance +=addAmount;
    }
    else{
      newAddedTap();
      lcd.print("New Card");
      lcd.print("Given $");
      lcd.print(addAmount);
      customers.push_back(Customer(readTag, addAmount));
    } 

  }
  if(rfidTriggerTime + 1500 < millis() && newTap){
    newTap = false;
    updateScreen = true;
  }
}


void settingsScreen(){
  delay(100);
  settingsTriggeredTime = millis();
  //exits the while loop when current setting exceeds the NUM_SETTINGS
  while(settingsTriggeredTime + 10000 > millis() && currentSetting < NUM_SETTINGS){

    //ESP32_ISR_Disable(ENCODER_BUTTON);
    encoderButton.update();
    if(encoderButton.isSingleClick()){
      currentSetting +=1;
      validationTurns = 0;
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
      case 2:
        displaySetting("Max drinks:", maxDrinks);
        editSetting(maxDrinks, 1, 1);
        break;
      case 3:
        doOnSwipe("Remove Customer", "Removed!", removeCustomer);
        break;
      case 4:
        doOnSwipe("Make Manager", "Made Manager!", makeManager);
        break;
        //open door
      case 5:
        doOnSwipe("Open Door", "Door Opened!", openDoor);
        break;
      case 6:
        addMoneyOnSwipe();
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


void waitScreen(){
  
  if (updateScreen){
    updateScreen = false;
    printLcdWelcome();
  }
  encoderButton.update();

  readTag = rfidScan();

  encoderButton.update();
  if(encoderButton.isSingleClick()){
    updateScreen = true;
    currentSetting = 0;
    currentScreen = 2;
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
      Serial.println("(For testing purposes, customer is added) ");
      customers.push_back(Customer(readTag, 10));
    } 
      
    }
  if(rfidTriggerTime + 4000 < millis() && newTap){
    newTap = false;
    updateScreen = true;
  }
}

void dispenseScreen(){
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


void run(){
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
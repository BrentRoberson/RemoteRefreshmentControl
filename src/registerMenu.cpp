#include "registerMenu.h"

double payment_count = 0;
bool scan;

unsigned long startTime;
unsigned long startTimeScan;
double payment_amount = 0;
int buttonState;

registerMenu::registerMenu(){}
struct_message message;

void registerMenu::setup()  {
  pinMode(ENCODER_BUTTON, INPUT_PULLUP);
  pinMode(registerButton,INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), registerMenu :: triggerMenu, FALLING);
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(0);
  printLcdWelcome();
  NUM_SCREENS = 3;
}

void registerMenu::set_num_screens(int screens){
  NUM_SCREENS = screens;
}

void registerMenu::displayMenu() {
  Serial.print(currentScreen);
  switch (currentScreen) {
    case 0:
      add_a_tag();
      break;
    case 1:
      refund_tag();
      break;
    case 2:
      check_balance();
      break;
    //make a case to resetAll
  }
  waiting = false;
}

void registerMenu:: triggerMenu()
{
  if(menuTriggeredTime + 500 < millis()){
    if(menuTriggeredTime == 0) {
      initPosition = oldPosition;
    }
    menuTriggeredTime = millis();
    currentScreen++;
    if(currentScreen >= NUM_SCREENS) {
      currentScreen = -1;
      // menuTriggeredTime = -50000;
    }
    validationTurns = 0;
    updateScreen = true;
  }
}

void registerMenu::run() {
  if(menuTriggeredTime != 0 && currentScreen != -1) {
    displayMenu();
    if(menuTriggeredTime + 30000 < millis()) {
    // if(menuTriggeredTime + 4000 < millis()) {

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

void registerMenu::waitScreen() {
  waiting = true;
  

  printLcdWelcome();
}

void registerMenu::printLcdWelcome(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to RRC!");
  lcd.setCursor(0, 1);
  lcd.print("Register Tags Here!");
  lcd.setCursor(0, 2);
  lcd.print("Payment Count");
  lcd.setCursor(0, 3);
  lcd.print(payment_count);
}

void registerMenu:: add_a_tag(){
  register_sent = false;
  payment_amount = 1;

  printSettingTitle();
  lcd.setCursor(0, 1);
  lcd.print("Add A Tag");
  lcd.setCursor(0, 2);
  lcd.print("Scan a Tag to add");
  startTime = millis();
  while (millis() - startTime < 5000 && currentScreen==0) {
    readTag = rfidScan();
    if(readTag!="") {
      newTap = true;
      rfidGoodTap();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Tag Read!");
      // Send message to add balance 
      while (!register_sent) {
        buttonState = digitalRead(registerButton);
        displaySetting("Payment Amount",payment_amount);
        editSetting(payment_amount, .25, .25);
        if (buttonState == LOW && payment_amount >0){
          message.amount = payment_amount;
          message.rfid = readTag;
          espNow.sendData( (uint8_t *) &message,sizeof(message));
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Sent");
          register_sent = true;
        }
      }
      startTimeScan = millis();
      while (millis() - startTimeScan < 4000 && register_sent == true) {
        if (bal_received == false  )
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Waiting.........");
        }
        else if (returned_balance >0 ){
          Serial.print(returned_balance);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Success!!!");
          lcd.setCursor(0,1);
          lcd.print("Current Fund:: ");
          lcd.print(returned_balance);
          delay(2000);
          bal_received = false;
          register_sent = false;
          payment_count++;
        }
        else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Error");
          lcd.setCursor(0,1);
          lcd.print("Payment Failed!!!");
          delay(2000);
          bal_received = false;
          register_sent = false;
        }
      }   
    if (millis() - startTimeScan > 4000){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Timeout!!");
      lcd.setCursor(0,1);
      lcd.print("Coms Failed!!!");
      delay(2000);
      bal_received = false;
      register_sent = false;
    }   
                
    Serial.println(readTag);
    Serial.println(rfidTriggerTime);

    Serial.println();
    Serial.println();
    }
  }
}

void registerMenu:: refund_tag(){
  printSettingTitle();
  lcd.setCursor(0, 1);
  lcd.print("Refund Tag");
  lcd.setCursor(0, 2);
  lcd.print("Scan a Tag");
  startTime = millis();
  while (millis() - startTime < 5000 && currentScreen==1) { // && refund_amount
    readTag = rfidScan();
    if (readTag != "") {
      rfidGoodTap();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Tag Read!");
      message.rfid = readTag;
      // How do i send this amount and then wait for the response to update the screen?
      message.amount = -1;
      espNow.sendData( (uint8_t *) &message,sizeof(message));
      lcd.clear();
      startTimeScan = millis();
      // while (millis() - startTimeScan < 2000){
        startTimeScan = millis();
        while (millis() - startTimeScan < 4000 && refund_received == false){
          lcd.setCursor(0,0);
          lcd.print("Waiting.........");
        }
        if (millis() - startTimeScan > 4000){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Timeout!!");
          lcd.setCursor(0,1);
          lcd.print("Coms Failed!!!");
          delay(2000);
          bal_received = false;
          register_sent = false;
        }   
        // if the refund went wrong 
        // maybe not a user
        if (refund_amount == -10.00){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Refund Error!!");
          lcd.setCursor(0,1);
          lcd.print("Card Not Found");
          delay(2000);
          refund_amount = 0;
          refund_received = false;
          break;
          
        }
        else{
          if (refund_amount >0){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Refund Received!");
            lcd.setCursor(0,1);
            lcd.print("Refund Amount: ");
            lcd.print(refund_amount);
            delay(2000);
            refund_received = false;
            break;
          }
          else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Refund Error!");
            lcd.print(refund_amount);
            refund_amount = 0 ;
            refund_received = false;
            break;
          }
        }
    }
  }
}
void registerMenu:: check_balance(){
  printSettingTitle();
  lcd.setCursor(0, 1);
  lcd.print("Check Balance");
  lcd.setCursor(0, 2);
  lcd.print("Scan a Tag");
  startTime = millis();
  while (millis() - startTime < 8000 && currentScreen==2) { // && refund_amount
    readTag = rfidScan();
    if (readTag != "") {
      rfidGoodTap();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Tag Read!");
      message.rfid = readTag;
      // How do i send this amount and then wait for the response to update the screen?
      message.amount = -5;
      espNow.sendData( (uint8_t *) &message,sizeof(message));
      lcd.clear();
      startTimeScan = millis();
      // while (millis() - startTimeScan < 2000){
        startTimeScan = millis();
      while (millis() - startTimeScan < 4000 && bal_received == false){
        lcd.setCursor(0,0);
        lcd.print("Waiting.........");
      }
      if (millis() - startTimeScan > 4000){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Timeout!!");
        lcd.setCursor(0,1);
        lcd.print("Coms Failed!!!");
        delay(2000);
        bal_received = false;
        register_sent = false;
      }   
      // if the refund went wrong 
      // maybe not a user
      if (returned_balance == -5.00){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Balance Error!!");
        lcd.setCursor(0,1);
        lcd.print("Card Not Found");
        delay(2000);
        returned_balance = 0;
        bal_received = false;
        break;}
        else{
          if (returned_balance >=0){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Balance Received!");
            lcd.setCursor(0,1);
            lcd.print("Balance: ");
            lcd.print(returned_balance);
            delay(2000);
            bal_received = false;
            break;
          }
          else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Balance Error!");
            lcd.print(returned_balance);
            returned_balance = 0 ;
            bal_received = false;
            break;
          }
        }
    }
  }
 }
#include "registerMenu.h"


double payment_count = 0;

unsigned long startTime;
unsigned long startTimeScan;
double payment_amount = 0;
int buttonState;

struct_message message;

registerMenu::registerMenu(){ 
  
}

void registerMenu::setup()  {
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(0);
  printLcdWelcome();
  currentScreen = 0;
  updateScreen = false;
}

void registerMenu::set_num_screens(int screens){
  NUM_SCREENS = screens;
}



void registerMenu::run() {

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
  payment_amount = 1;
  if (updateScreen){
    printLcdWelcome();
    updateScreen = false;
  }
  encoderButton.update();
  if(encoderButton.isSingleClick()){
    updateScreen = true;
    currentScreen = 1;
  }
 
  startTime = millis();
  readTag = rfidScan();
  if(readTag!="") {
    while (millis() - startTime < 5000 && currentScreen==0) {
      
      if(readTag!="") {
        newTap = true;
        rfidGoodTap();
        updateScreen = true;
        while (!register_sent && currentScreen ==0) {
          encoderButton.update();
          if(encoderButton.isSingleClick()){
            updateScreen = true;
            currentScreen = 1;
          }
          if(digitalRead(registerButton)==LOW && payment_amount>0){
            message.amount = payment_amount;
            message.rfid = readTag;
            Serial.println("read");

            espNow.sendData( (uint8_t *) &message,sizeof(message));
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Sent");
            register_sent = true;
            Serial.println("Sent");
            Serial.println(register_sent);

            // break;
          }
          if (!register_sent){
            displaySetting("Payment Amount",payment_amount);
            editSetting(payment_amount, .25, .25);
            // Serial.println(register_sent);
          }
        }
        startTimeScan = millis();
        bool waitprnt = false;
        while (millis() - startTimeScan < 4000 && register_sent == true) {
          if (bal_received == false && !waitprnt )
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Waiting.........");
            waitprnt = true;
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
            readTag = "";
            updateScreen = true;
            payment_count++;
            // break;
          }
          else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error");
            lcd.setCursor(0,1);
            lcd.print("Payment Failed!!!");
            delay(2000);
            bal_received = false;
            readTag = "";
            updateScreen = true;
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
        currentScreen = 0;
        updateScreen = true;
        bal_received = false;
        readTag = "";
        register_sent = false;
      }   
                  
      Serial.println(readTag);
      Serial.println(rfidTriggerTime);

      Serial.println();
      Serial.println();
      }
    }
  }
}

void registerMenu:: refund_tag(){
  register_sent = false;
  printSettingTitle();
  lcd.setCursor(0, 1);
  lcd.print("Refund Tag");
  lcd.setCursor(0, 2);
  lcd.print("Scan a Tag");
  startTime = millis();
  while (millis() - startTime < 5000 && currentScreen==1) { // && refund_amount
    encoderButton.update();
    readTag = rfidScan();
    encoderButton.update();
    if(encoderButton.isSingleClick()){
      updateScreen = true;
      currentScreen = 2;
    }
    if(encoderButton.isLongClick()){
      currentSetting = 0; //effectively returns to waitScreen
    }
    if (readTag != "") {
      rfidGoodTap();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Tag Read!");
      message.rfid = readTag;
      // How do i send this amount and then wait for the response to update the screen?
      message.amount = -1;
      // TODO: Create a click to send the message 
      while (!register_sent) {
        if(encoderButton.isSingleClick()){
          updateScreen = true;
          currentScreen = 2;
        }
        espNow.sendData( (uint8_t *) &message,sizeof(message));
        register_sent = true;
      }


      lcd.clear();
      startTimeScan = millis();
      // while (millis() - startTimeScan < 2000){
        startTimeScan = millis();
        bool waitprnt = false;
        while (millis() - startTimeScan < 4000 && refund_received == false){
          if (!waitprnt){
            lcd.setCursor(0,0);
            lcd.print("Waiting.........");
            waitprnt = true; 
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
    encoderButton.update();
    readTag = rfidScan();
    encoderButton.update();
    if(encoderButton.isSingleClick()){
      updateScreen = true;
      currentScreen = 0;
    }
    if(encoderButton.isLongClick()){
      currentSetting = 0; //effectively returns to waitScreen
    }
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
      bool waitprnt = false;
      while (millis() - startTimeScan < 4000 && bal_received == false){
        if (!waitprnt){
            lcd.setCursor(0,0);
            lcd.print("Waiting.........");
            waitprnt = true; 
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

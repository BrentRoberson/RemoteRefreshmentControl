#include "registerMenu.h"

double total_customers = 0;
bool scan;
ESP32NOW espnow; 

unsigned long startTime;
unsigned long startTimeScan;


registerMenu::registerMenu(){}
struct_message message;

void registerMenu::setup()  {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), registerMenu :: triggerMenu, FALLING);
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(0);
  printLcdWelcome();
  NUM_SCREENS = 2;
}

void registerMenu::set_num_screens(int screens){
  NUM_SCREENS = screens;
}

void registerMenu::displayMenu() {
  Serial.print(currentScreen);
  switch (currentScreen) {
    case 0:
      printSettingTitle();
      lcd.setCursor(0, 1);
      lcd.print("Refund Tag");
      lcd.setCursor(0, 2);
      lcd.print("Scan a Tag");
      startTime = millis();
      while (millis() - startTime < 5000 && currentScreen==0) { // && refund_amount
        readTag = rfidScan();
        if (readTag != "") {
          readTag = rfidScan();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Tag Read!");
          
          lcd.print(refund_amount);
          message.rfid = readTag;
          // How do i send this amount and then wait for the response to update the screen?
          message.amount = -1;
          espnow.sendData( (uint8_t *) &message,sizeof(message));
          lcd.clear();
          startTimeScan = millis();
          while (millis() - startTimeScan < 2000){
            if (refund_received == false)
            {
              lcd.setCursor(0,0);
              lcd.print("Waiting.........");
            }
            else{
              if (refund_amount >0){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Refund Received!");
                lcd.setCursor(0,1);
                lcd.print("Refund Amount: ");
                lcd.print(refund_amount);
              }
              else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Refund Error!");

              }
            }
           }
         }
      }
      break;
    case 1:
      displaySetting("Check Balance", pricePerOunce);
      editSetting(pricePerOunce, .01);
      break;
    //make a case to resetAll
  }
}

void registerMenu::waitScreen() {
  waiting = true;
  printLcdWelcome();

  while(menuTriggeredTime==0 ){
    readTag = rfidScan();
    
    if(readTag!=""){
      newTap = true;
      lcd.clear();
      lcd.setCursor(0,0);
      // Send message to add balance 
      rfidGoodTap();
      double payment_amount = 0;
      displaySetting("Payment Amount:", payment_amount);
      editSetting(payment_amount, .25);
      message.rfid = readTag;
      message.amount =+ payment_amount;
      espnow.sendData( (uint8_t *) &message,sizeof(message));
    }
        
    // else{
    //   rfidBadTap();
    //   lcd.print("Unknown ID");
    //   lcd.setCursor(0,1);
    //   lcd.print("Please check in");
    //   Serial.print("Customer not added");
    //   Serial.println("(For testing purposes, customer is added) ");
    //   customers.push_back(Customer(readTag, rand()%50));
    // } 
        
    Serial.println(readTag);
    Serial.println(rfidTriggerTime);

    Serial.println();
    Serial.println();
    if(rfidTriggerTime + 4000 < millis() && newTap){
      printLcdWelcome();
      newTap = false;
    }
  }
}

void registerMenu::printLcdWelcome(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to RRC!");
  lcd.setCursor(0, 1);
  lcd.print("Scan a tag to Deposit!");
  lcd.setCursor(0, 2);
  lcd.print("Total Current Users");
  lcd.setCursor(0, 3);
  lcd.print(total_customers);
}
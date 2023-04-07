#include <WiFi.h>
#include <message.h>
#include <Arduino.h>
#include <NFC.h>
#include <Drink.h>
#include <Customer.h>
#include <Pitches.h>
#include <LiquidCrystal_I2C.h>
//#include <Menu.h>
#include <ESPNow.h>
#include <newMenu.H>
#include <SDCard.h>
#include <Rainbow.h>
#include <ESP32API.h>
#define CLK_PIN 14
#define DT_PIN 26
#define SW_PIN 13

PinButton encoderButton(ENCODER_BUTTON);
PinButton doneButton(DONE_BUTTON);
float pricePerOunce= .30;
float totalQuarts = 10;
int maxOunces = 80;

ESP32Encoder encoder;
bool readError;
Customer curr_cust = Customer();
DynamicArray<Customer> customers;
int currentScannedIndex;
bool new_sd_data = false;
struct_message myData;
struct_message response_message;
LiquidCrystal_I2C lcd(0x27, 20, 4);
//Menu menu;
SDCard SdData("/DATA.txt");
ESP32NOW espNow;
/// Make globals for this address 
uint8_t Register_broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0x74, 0xE1, 0xC0};
NewMenu Menu;

// Callback function executed when data is received
// When data is received from the other controller this function will run automatically
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  int customer_index = customers.search(myData.rfid);
  if (myData.amount > 0){
    // new customer
    if (customer_index ==-1){
    Serial.print("New Customer");
      customers.push_back(Customer(myData.rfid,myData.amount));
      response_message.amount =  myData.amount; 
      response_message.rfid = String("Bal");
      espNow.sendData( (uint8_t *) &response_message,sizeof(response_message));
      new_sd_data = true;
      currentScannedIndex = customers.getSize()-1;
      curr_cust  = Customer(myData.rfid,myData.amount);
    }
    // Add to balance
    else {
      Serial.print("Balance Increased");
      customers[customer_index].balance += myData.amount;
      response_message.amount =  customers[customer_index].balance;
      response_message.rfid = String("Bal");
      espNow.sendData( (uint8_t *) &response_message,sizeof(response_message));
      new_sd_data = true;
      currentScannedIndex = customer_index;
      curr_cust  =  customers[customer_index];

    }

  }
  // Balance Checks
  else if (myData.amount==-5.00)
  {
    if (customer_index ==-1){
      response_message.amount =  -5; 
      response_message.rfid = String("Bal_err");
      espNow.sendData( (uint8_t *) &response_message,sizeof(response_message));
    }
    else {
      response_message.amount =  customers[customer_index].balance; 
      response_message.rfid = String("Bal");
      espNow.sendData( (uint8_t *) &response_message,sizeof(response_message));

    }
  }
  // Refunds
  else if (myData.amount==-1.00)
  {
    // delete the customer and send refund message
    if (customer_index ==-1){
      // no customer found to refund
      Serial.print("refund Error");
      response_message.amount =  -10; 
      response_message.rfid = String("err");
      espNow.sendData( (uint8_t *) &response_message,sizeof(response_message));

    }
    else {
      Serial.print("Refund issued");
      response_message.amount =  customers[customer_index].balance;
      customers[customer_index].balance = 0;
      customers[customer_index].ouncesDrank = 0;
      currentScannedIndex = customer_index;
      new_sd_data = true;
      curr_cust = customers[customer_index];
      response_message.rfid = String("rfnd");
      espNow.sendData( (uint8_t *) &response_message,sizeof(response_message));

    }
  }
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("RFID Value: ");
  Serial.println(myData.rfid);
  Serial.print("Amount Transfered Value: ");
  Serial.println(myData.amount);
    // response_message.rfid = "";
  // espNow.sendData((uint8_t *) &response_message, sizeof(response_message));
  
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Set up Serial Monitor
  
  Serial.begin(115200);
  pinMode(DISPENSE_BUTTON, INPUT);
  pinMode(DONE_BUTTON, INPUT);
  // Set ESP32 as a Wi-Fi Station
  // WiFi.mode(WIFI_STA);
  // WiFi.mode(WIFI_MODE_STA);
  pinMode(DOOR_LOCK, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);

  // espNow.init();
  // espNow.addPeer(Register_broadcastAddress);
  // espNow.registerDataSentCallback(OnDataSent);
  // espNow.registerDataReceivedCallback(OnDataRecv);
  // attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  
  setupAPI();
  lcd.init();
  lcd.backlight();
  // esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Dispenser setup");
  lcd.print("Startup Completed!");
  LEDSetup();
  startup();
  NFCsetup();
  if(!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  SdData.readInSD();
  Menu.printLcdWelcome();



}


void loop() {
  Menu.run();
}
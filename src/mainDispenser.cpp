#include <WiFi.h>
#include <message.h>
#include <Arduino.h>
#include <RFID.h>
#include <Drink.h>
#include <Customer.h>
#include <Pitches.h>
#include <LiquidCrystal_I2C.h>
#include <Menu.h>
#include <ESPNow.h>

#define CLK_PIN 14
#define DT_PIN 26
#define SW_PIN 13


double pricePerOunce = .40;
double totalQuarts = 20;
int maxDrinks = 6;
String drinkOTD = "NA";
ESP32Encoder encoder;
bool readError;
DynamicArray<Customer> customers;
Customer lastCustomerScanned;
struct_message myData;
struct_message response_message;
LiquidCrystal_I2C lcd(0x27, 16, 4);
Menu menu;
ESP32NOW espNow;
/// Make globals for this address 
uint8_t Register_broadcastAddress[] = {0xCC, 0xDB, 0xA7, 0x14, 0xF4, 0x58};

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
      response_message.amount =  customers[customer_index].balance; 
    }
    // Add to balance
    else {
      Serial.print("Balance Increased");
      customers[customer_index].balance += myData.amount;
      response_message.amount =  customers[customer_index].balance;
    }

  }
  // Refunds
  else if (myData.amount==-1)
  {
    // delete the customer and send refund message
    if (customer_index ==-1){
      // no customer found to refund
      Serial.print("refund Error");
      response_message.amount =  -10; 
    }
    else {
      Serial.print("Refund issued");
      response_message.amount =  customers[customer_index].balance;
      customers.deleteAt(customer_index);

    }
  }
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("RFID Value: ");
  Serial.println(myData.rfid);
  Serial.print("Amount Transfered Value: ");
  Serial.println(myData.amount);
  response_message.rfid = "";
  espNow.sendData((uint8_t *) &response_message, sizeof(response_message));
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  espNow.init();
  espNow.addPeer(Register_broadcastAddress);
  espNow.registerDataReceivedCallback(OnDataRecv);
 
  RFIDsetup();
  lcd.init();
  lcd.backlight();
  // esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Dispenser steup");
  lcd.print("Startup Completed!");

  startup();

  menu.setup();
}


void loop() {
  menu.run();
  
}
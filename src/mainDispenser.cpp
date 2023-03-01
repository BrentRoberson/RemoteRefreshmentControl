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
LiquidCrystal_I2C lcd(0x27, 16, 4);
Menu menu;
/// Make globals for this address 
uint8_t Register_broadcastAddress[] = {0xCC, 0xDB, 0xA7, 0x14, 0xF4, 0x58};

// Callback function executed when data is received
// When data is received from the other controller this function will run automatically
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  if (myData.amount > 1){
    // Add to balance

  }
  else if (myData.amount==-1)
  {
    // delete the customer and send refund message
    
  }
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("RFID Value: ");
  Serial.println(myData.rfid);
  Serial.print("Amount Transfered Value: ");
  Serial.println(myData.amount);
  int customer_index = customers.search(myData.rfid);
  // if theres a new customer 
  customers[customer_index].balance += myData.amount; 
  
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  ESP32NOW espNow;
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
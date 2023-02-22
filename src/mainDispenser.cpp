#include <esp_now.h>
#include <WiFi.h>
#include <message.h>
#include <Arduino.h>
#include <RFID.h>
#include <Drink.h>
#include <Customer.h>
#include <Pitches.h>
#include <LiquidCrystal_I2C.h>
#include <Menu.h>

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
struct_message myData;
LiquidCrystal_I2C lcd(0x27, 16, 4);
String screens[NUM_SCREENS] = {
  "Drinks Left", 
  "Total Customers", 
  "Edit Quarts",
  "Drink Limit", 
  "Edit Customers"};

Menu menu;

// Callback function executed when data is received
// When data is received from the other controller this function will run automatically
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("RFID Value: ");
  Serial.println(myData.rfid);
  Serial.print("Amount Transfered Value: ");
  Serial.println(myData.amount);
  int customer_index = customers.search(myData.rfid);
  customers[customer_index].balance += myData.amount; 
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
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
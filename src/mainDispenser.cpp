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

#define SOLENOID 2
#define buzzerPin 4

double pricePerOunce = .40;
double ouncesLeft = 640;
double drinksLeft = ouncesLeft/12;

bool readError;
String readTag;
DynamicArray<Customer> customers;
struct_message myData;
LiquidCrystal_I2C lcd(0x27, 16, 4);
Encoder myEnc(ENC_DATA,ENC_CLOCK);
String screens[NUM_SCREENS][2] = {
  {"Ignition Time","Minutes"}, 
  {"Stabilization", "Minutes"}, 
  {"Cleaning","Minutes"},
  {"Start dose","Seconds"}, 
  {"Cut-off Temperature","Celsius"}, 
};
int parameters[NUM_SCREENS];
//Menu menu(NUM_SCREENS, lcd)

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
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Dispenser steup");
  lcd.print("Startup Completed!");
  startup();

}


void loop() {
  readTag = waitForTag();
    if(readTag!=""){
      int customerIndex = customers.search(readTag);
      
      if(customerIndex>-1)
      {
        rfidGoodTap();
        customers[customerIndex].lcdPrint();
        //print "press green button to dispense
        //enter drink menueditCustomer/buy drink
        customers[customerIndex].drinks->push_back(Drink(rand()%24, rand()%2000));
        customers[customerIndex].balance -= rand()%6;
        customers[customerIndex].print();
      }
      else{
        rfidBadTap();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Card Unrecognized");
        lcd.setCursor(2,2);
        lcd.print("Please check in");
        lcd.setCursor(3,3);
        lcd.print("at front desk");
        Serial.print("Customer not added, please go to check in to add your card. ");
        Serial.println("For testing purposes, customer is added: ");
        customers.push_back(Customer(readTag, rand()%50));
      }
      Serial.println(readTag);
    }
}
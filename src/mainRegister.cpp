// Include Libraries
#include <RFID.h>
#include <WiFi.h>
#include <message.h>
#include <ESPNow.h>
 
// Variables for test data
int int_value;
float float_value;
bool bool_value = true;
String readTag = "";
 
// MAC Address of responder - edit as required
// uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0x74, 0xE1, 0xC0};

uint8_t broadcastAddress[] = {0xCC, 0xDB, 0xA7, 0x14, 0xF4, 0x58};


// Create a structured object
struct_message myData;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {
  
  // Set up Serial Monitor
  Serial.begin(115200);

 // rfid init 
  RFIDsetup();
  // Register the send callback
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  ESP32NOW espNow;
  espNow.init();
  espNow.addPeer(broadcastAddress);
  espNow.registerDataSentCallback(OnDataSent);
}


void loop() {
// Format structured data
  readTag = rfidScan();
  if (readTag!=""){
  float input_amount = 5.0;

  myData.rfid = readTag;
  myData.amount = input_amount;

  //I think it will need this to be able to edit vs add balance
  //Register has to have the entire customer array for this, but I 
  //maybe we can ping the dispenser asking if customer is in array
  // int customerIndex = customers.search(readTag);
  //   lcd.clear();
  //   lcd.setCursor(0,0);
  //   if(customerIndex>-1)
  //     {
  //       lastCustomerScanned = customers[customerIndex];
  //       rfidBadTap();
  //       lcd.print("Already Added");
  //       lcd.setCursor(0,1);
  //       lcd.print("Edit Balance: ");
  //       lcd.setCursor(0,2);
  //       lcd.print(lastCustomerScanned.balance);
  //       //needs implementing
  //       editCustomer(lastCustomerScanned);
  //     }
  //     else{
  //       rfidGoodTap();
  //       Serial.print("Welcome, New Customer!");
  //       //needs implementing
  //       addCustomer();
  //       lastCustomerScanned = customers.pop_back();
  //     } 
  
  // Send message via ESP-NOW
  Serial.println("SENT!!!!");
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
    readTag = "";
  }
  else {
    Serial.println("Sending error");
    readTag= "";
  }
  delay(2000);
  }

}
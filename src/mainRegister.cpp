// Include Libraries
#include <esp_now.h>
#include <RFID.h>
#include <WiFi.h>
#include <message.h>
 
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

// Peer info
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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

  // rfid init 
  RFIDsetup();
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  Serial.println("Register setup1");


  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  Serial.println("Register setup2");
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Register setup");
}


void loop() {
// Format structured data
  readTag = waitForTag();
  if (readTag!=""){
  float input_amount = 5.0;

  myData.rfid = readTag;
  myData.amount = input_amount;
 
  
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
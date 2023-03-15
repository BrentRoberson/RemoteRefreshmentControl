// Include Libraries
#include <registerMenu.h>
 
// Variables for test data
float pricePerOunce = 0;
float totalQuarts = 0;
int maxDrinks = 0;

ESP32NOW espNow;
registerMenu menu;
ESP32Encoder encoder;
LiquidCrystal_I2C lcd(0x27, 16, 4);
struct_message myData;
float refund_amount = 0;
bool refund_received = false;
bool bal_received;
float returned_balance;
bool register_sent = false;
DynamicArray<Customer> customers;
int currentScannedIndex;
uint8_t Dispenser_broadcastAddress[] = {0xCC, 0xDB, 0xA7, 0x14, 0xF4, 0x58};
PinButton encoderButton(ENCODER_BUTTON);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  register_sent = true;
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("RFID Value: ");
  Serial.println(myData.rfid);
  Serial.print("Amount Transfered Value: ");
  Serial.println(myData.amount);
  // Issue Refund
  // set for refunds
  if (myData.rfid == "rfnd") {
  
    refund_amount = myData.amount;
    refund_received = true;
  }
  // set for bad refund
  else if (myData.amount == -10.00){
    refund_amount = myData.amount;
    refund_received = true;
  }
  // set for bal received
  else if (myData.rfid == String("Bal")){
    returned_balance = myData.amount;
    bal_received = true;
  }
  else if (myData.rfid == String("Bal_err")){
    returned_balance = myData.amount;
    bal_received = true;
  }

}

void setup() {
  
  // Set up Serial Monitor
  Serial.begin(115200);
  delay(400);

 // rfid init 
  RFIDsetup();
  // Register the send callback
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // WiFi.mode(WIFI_MODE_STA);

  
  espNow.init();
  espNow.addPeer(Dispenser_broadcastAddress);
  espNow.registerDataSentCallback(OnDataSent);
  espNow.registerDataReceivedCallback(OnDataRecv);
  lcd.init();
  lcd.backlight();

  Serial.println("Register steup");
  lcd.print("Startup Completed!");
  startup();
  menu.setup();
  pinMode(registerButton,INPUT_PULLUP);
}


void loop() {
  menu.run();
// Format structured data
  // readTag = waitForTag();
  // if (readTag!=""){
  // float input_amount = 5.0;

  // myData.rfid = readTag;
  // myData.amount = input_amount;
 
  
  // // Send message via ESP-NOW
  // Serial.println("SENT!!!!");
  // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  // if (result == ESP_OK) {
  //   Serial.println("Sending confirmed");
  //   readTag = "";
  // }
  // else {
  //   Serial.println("Sending error");
  //   readTag= "";
  // }
  // delay(2000);
  // }

}
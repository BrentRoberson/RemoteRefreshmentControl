#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22          // RST Pin for RFID
#define CS_PIN_RFID          18         // SS Pin for RFID

MFRC522 mfrc522(CS_PIN_RFID, RST_PIN);   // Create an MFRC522 instance

void setup() {
  Serial.begin(115200);              // Initialize serial communication
  SPI.begin();                     // Initialize SPI communication
  mfrc522.PCD_Init();              // Initialize MFRC522
}

void loop() {
  // Check for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Print the card UID
    Serial.print("UID tag : ");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println(content);
  }
  delay(500);
}

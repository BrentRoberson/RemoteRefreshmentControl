/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-rfid-nfc
 */

#include<RFID.h>
#include <LiquidCrystal_I2C.h>
bool tagRead = false;
MFRC522 rfid(CS_PIN_RFID, RST_PIN);

void RFIDsetup() {
  SPI.begin();
  rfid.PCD_Init(); // init MFRC522
  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

String rfidScan() {
  
  String temp = "";
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
    for (int i = 0; i < rfid.uid.size; i++) {
      temp+=(rfid.uid.uidByte[i]);
      }
    }
  }
  rfid.PICC_HaltA(); // halt PICC
  rfid.PCD_StopCrypto1(); // stop encryption on PCD
  //if a scan found a customer and temp was changed
  return temp;
}



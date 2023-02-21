/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-rfid-nfc
 */

#include<RFID.h>
#include <LiquidCrystal_I2C.h>

MFRC522 rfid(SS_PIN, RST_PIN);

void RFIDsetup() {
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  pinMode(solenoid, OUTPUT);
  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
  
}

String waitForTag() {
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
   
  return temp;
}


// void printTag(MFRC522 rfid,LiquidCrystal_I2C lcd ){
//   // print UID in Serial Monitor in the hex format
//   Serial.print("UID:");
//   lcd.print("UID:");
//   for (int i = 0; i < rfid.uid.size; i++) {
//     Serial.print(rfid.uid.uidByte[i] );
//   }
//   Serial.println();
  
//   rfid.PICC_HaltA(); // halt PICC
//   rfid.PCD_StopCrypto1(); // stop encryption on PCD
// }

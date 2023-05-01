/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-rfid-nfc
 */

#include <NFC.h>
#include <LiquidCrystal_I2C.h>

bool tagRead = false;
//these dont mean anything for our implementation but they are needed for initializing the nfc reader
#define PN532_IRQ   (2)
#define PN532_RESET (3)  

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);


void NFCsetup() {
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A Card ...");
}

String rfidScan(int timeout) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  String hexUID = "";
  // Wait for an NTAG203 card.  When one is found 'uid' will be populated with
  // the UID, and uidLength will indicate the size of the UUID (normally 7)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, timeout);

  if (success) {
    // Display some basic information about the card
    for (int i = 0; i < uidLength; i++) {
      // convert byte to uppercase hex string with leading zeros
      String byteHex = String(uid[i], HEX);
      if (byteHex.length() < 2) {
        byteHex = "0" + byteHex;
      }
      byteHex.toUpperCase();
      // append hex representation to hexUID string
      hexUID += byteHex;
    }  
  }
  // Serial.println(hexUID);
  return hexUID;
}



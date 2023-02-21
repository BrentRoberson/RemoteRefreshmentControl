#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  32  // ESP32 pin GIOP5 
#define RST_PIN 33 // ESP32 pin GIOP27 
#define solenoid 2



void RFIDsetup();
void printTag();
String waitForTag();

#endif
#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>
#include <Globals.h>




void RFIDsetup();
void printTag();
String waitForTag();

#endif
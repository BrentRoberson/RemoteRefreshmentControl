#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>
#include <Customer.h>
#include <Rainbow.h>


void RFIDsetup();
void printTag();

//returns the ID of a scan
String rfidScan();


#endif
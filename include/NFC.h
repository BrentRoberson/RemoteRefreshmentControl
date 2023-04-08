#ifndef RFID_H
#define RFID_H


#include <Customer.h>
#include <Rainbow.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

void NFCsetup();
void printTag();

//returns the ID of a scan
String rfidScan(int timeout = 50);


#endif
#ifndef CUSTOMERAPI_H
#define CUSTOMERAPI_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Customer.h>
#include <SDCard.h>
#include <WiFiAP.h>

void setupAPI();
void handlePostCustomer();
void handleGetCustomer();
void handleGetAllCustomers();
void handleClient();
void handleEditCustomer();


#endif

#ifndef ESPSERVER_H
#define ESPSERVER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Customer.h>
#include <SDCard.h>
#include <WiFiAP.h>
#include <ESPAsyncWebServer.h>

void setupAPI();
void handlePostCustomer();
void handleGetCustomer();
void handleGetAllCustomers();
void handleEditCustomer();


#endif

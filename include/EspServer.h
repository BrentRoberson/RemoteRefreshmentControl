#ifndef ESPSERVER_H
#define ESPSERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Customer.h>
#include <SDCard.h>
#include <WiFiAP.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

void setupServer();
void handlePostCustomer(AsyncWebServerRequest *request);
void handleGetCustomer(AsyncWebServerRequest *request);
void handleGetAllCustomers(AsyncWebServerRequest *request);
void handleEditCustomer(AsyncWebServerRequest *request);


#endif

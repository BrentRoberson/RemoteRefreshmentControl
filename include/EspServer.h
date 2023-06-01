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
#include <AsyncWebSocket.h>

void setupServer();
void handlePostCustomer(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total);
void handleGetCustomer(AsyncWebServerRequest *request);
void handleGetAllCustomers(AsyncWebServerRequest *request);
void handleEditCustomer(AsyncWebServerRequest *request);
void handlePostSettings(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total);
void handleGetSettings(AsyncWebServerRequest *request);


#endif

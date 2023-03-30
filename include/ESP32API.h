#ifndef CUSTOMERAPI_H
#define CUSTOMERAPI_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Customer.h>

void setupAPI();
void handlePostCustomer();
void handleGetCustomer();
void handleClient();

#endif

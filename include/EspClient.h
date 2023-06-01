#ifndef ESPCLIENT_H
#define ESPCLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Globals.h>
#include <AsyncTCP.h>

Customer requestCustomerData(String id);
void postCustomerData(Customer & customer);

void updateSettings();
void setupClient();
void postSettings();

#endif

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <ESP32API.h>

const char* ssid = "FBI-Surveillence-Van";
const char* password = "brentiepoo";

WebServer server(80);

void handlePost() {
  String body = server.arg("plain");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);

  String id = jsonDoc["id"];
  String name = jsonDoc["name"];
  float balance = jsonDoc["balance"];

  // Save the customer data in the ESP32's memory
  // ...

  server.send(200);
}

void handleGet() {
  Serial.print("got customer");
  String id = server.arg("id");
  int customerIndex = customers.search(id);
  
  // Get the customer data from the ESP32's memory
  // ...

  // Send the customer data as a JSON response
  DynamicJsonDocument jsonDoc(1024);
  jsonDoc["name"] = "John Doe";
  jsonDoc["balance"] = 100.0;
  String jsonStr;
  serializeJson(jsonDoc, jsonStr);
  server.send(200, "application/json", jsonStr);
}

void handleGetAllCustomers() {
  Serial.print("got all customers");
  String response;
  Serial.println("Get for all customers received");
  for (int i = 0; i<customers.getSize(); i++) {
    response += "ID: " + customers[i].ID + ", Name: " + 
    "customers[i].name" + ", Balance: " + String(customers[i].balance) 
    + " Ounces Drank:"+ customers[i].ouncesDrank+ "\n";
  }
  server.send(200, "text/plain", response);
}

void setupAPI() {
  // Set up the ESP32 as an access point
  WiFi.softAP(ssid, password);
  // Print the IP address of the access point
  Serial.print("Access point IP address: ");
  Serial.println(WiFi.softAPIP());
  // Set up the API routes
  server.on("/customer", HTTP_POST, handlePost);
  server.on("/customer", HTTP_GET, handleGet);
  server.on("/allCustomers", HTTP_GET, handleGetAllCustomers);

  // Start the server
  server.begin();
}

void handleClient() {
  server.handleClient();
  
}

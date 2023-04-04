#include <ESP32API.h>

const char* ssid = "FBI-Surveillence-Van";
const char* password = "brentiepoo";

WebServer server(80);

void handlePostCustomer() {
  String body = server.arg("plain");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  Customer temp = Customer();
  String id = jsonDoc["ID"];
  float balance = jsonDoc["balance"].as<float>();
  bool manager = jsonDoc["manager"].as<bool>();
  String name = jsonDoc["name"];
  Serial.println(id);
  Serial.println(balance);
  Serial.println(manager);
  Serial.println(name);
  // Save the customer data in the ESP32's memory
  temp = Customer(id,balance,manager, name);
  customers.push_back(temp);
  SdData.addOrUpdateCustomer(temp);
  server.send(200);
}

//pass an existing customer in the body of the post request. If it is found, it will convert 
//all member variables to the ones passed in
void handleEditCustomer(){
  String body = server.arg("plain");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  String id = jsonDoc["ID"];
  float balance = jsonDoc["amount"].as<float>();
  bool manager = jsonDoc["manager"].as<bool>();
  String name = jsonDoc["name"];
  int addSetOrRefund = jsonDoc["addSetOrRefund"];
  int customerIndex = customers.search(id);
  if(customerIndex>-1)
  {
    customers[customerIndex].balance = balance;
    customers[customerIndex].manager = manager;
    customers[customerIndex].name = name;
  }
  else{
    Serial.print("CUSTOMER NOT FOUND");
  } 
  SdData.addOrUpdateCustomer(customers[customerIndex]);
  server.send(200);

}

void handleGetCustomer() {
  Serial.print("got customer");
  String id = server.arg("id");
  int customerIndex = customers.search(id);
  DynamicJsonDocument jsonDoc(1024);
  String jsonStr;

  if (customerIndex>-1){
    jsonDoc["id"] = customers[customerIndex].ID;
    jsonDoc["balance"] = customers[customerIndex].balance;
    jsonDoc["manager"] = customers[customerIndex].manager;
    jsonDoc["name"] = customers[customerIndex].name;
  }
  else
  {
    jsonDoc["id"] = "NOT FOUND";
    jsonDoc["balance"] = 0.0;
    jsonDoc["manager"] =false;
    jsonDoc["name"] ="";
  }

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
  server.on("/customer", HTTP_POST, handlePostCustomer);
  server.on("/customer", HTTP_GET, handleGetCustomer);
  server.on("/editCustomer", HTTP_POST, handleEditCustomer);
  server.on("/allCustomers", HTTP_GET, handleGetAllCustomers);

  // Start the server
  server.begin();
}

void handleClient() {
  server.handleClient();
}

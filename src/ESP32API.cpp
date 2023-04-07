#include <ESP32API.h>

const char* ssid = "FBI-Sting-Van";
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
  bool setVars = jsonDoc["setVars"].as<bool>();
  float ouncesDrank = jsonDoc["ouncesDrank"].as<float>();

  int customerIndex = customers.search(id);
  if (customerIndex >-1)
  {
    if (setVars){
      customers[customerIndex].balance = balance;
      customers[customerIndex].manager = manager;
      customers[customerIndex].name = name;
      customers[customerIndex].ouncesDrank = ouncesDrank;
      SdData.addOrUpdateCustomer(customers[customerIndex]);
      Serial.println("updated old customer with set!");
    }
    else {
      customers[customerIndex].balance =+ balance;
      customers[customerIndex].manager = manager;
      customers[customerIndex].name = name;
      customers[customerIndex].ouncesDrank = ouncesDrank;

      SdData.addOrUpdateCustomer(customers[customerIndex]);
      Serial.println("updated old customer with add!");
    }   
  }
  else {
    // Save the customer data in the ESP32's memory
    temp = Customer(id,balance,manager, name);
    customers.push_back(temp);
    SdData.addOrUpdateCustomer(temp);
    Serial.println("Added new customer!");
  }
  Serial.println(id);
  Serial.println(balance);
  Serial.println(manager);
  Serial.println(name);
  Serial.println(setVars);

  
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
  bool setVars = jsonDoc["setVars"].as<bool>();
  int addSetOrRefund = jsonDoc["addSetOrRefund"];
  int customerIndex = customers.search(id);
  if(customerIndex>-1)
  {
    if(setVars)
    {
      customers[customerIndex].balance = balance;
      customers[customerIndex].manager = manager;
      customers[customerIndex].name = name;
    }
    else {
      customers[customerIndex].balance + balance;
      customers[customerIndex].manager = manager;
      customers[customerIndex].name = name;
      customers[customerIndex].ouncesDrank + name;

    }
   
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
  DynamicJsonDocument doc(25000); // create a JSON document
  JsonArray customersJson = doc.createNestedArray("customers"); // create a nested array of customers
  Serial.println("Get for all customers received");
  for (int i = 0; i < customers.getSize(); i++) {
    JsonObject customerJson = customersJson.createNestedObject(); // create a nested object for each customer
    customerJson["ID"] = customers[i].ID;
    customerJson["Name"] = customers[i].name;
    customerJson["Balance"] = customers[i].balance;
    customerJson["Ounces Drank"] = customers[i].ouncesDrank;
  }
  String response;
  serializeJson(doc, response); // serialize the JSON document to a string
  Serial.println(response);
  Serial.println(customers.getSize());
  server.send(200, "application/json", response); // send the response as JSON
}
void handleRemoveCustomers() {
  String body = server.arg("plain");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  Customer temp = Customer();
  String id = jsonDoc["ID"];
  int idx = customers.search(id);
  Serial.println(id);
  if (idx >-1) {
    SdData.removeCustomer(customers[idx]);
    customers.deleteAt(idx);
    Serial.print("Customer Deleted!");
    server.send(200);
  }
  else {
    Serial.print("Customer deletion error!!");
    server.send(100);
  }

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
  server.on("/removeCustomers", HTTP_POST, handleRemoveCustomers);


  // Start the server
  server.begin();
}

void handleClient() {
  server.handleClient();
}

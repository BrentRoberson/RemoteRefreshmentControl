#include <ESP32API.h>

const char* routerSsid = "The Dawg House";
const char* routerPassword = "kodabear";
const char* ssid = "BarBox Wifi";
const char* password = "brentiepoo";

WebServer server(80);

void handlePostCustomer() {
  String body = server.arg("plain");
  Serial.print(body);
  Serial.println("POSTED");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  Customer temp = Customer();
  String id = jsonDoc["ID"];
  float balance = jsonDoc["balance"].as<float>();
  bool manager = jsonDoc["manager"].as<bool>();
  String name = jsonDoc["name"];
  float ouncesDrank = jsonDoc["ouncesDrank"].as<float>();

  int customerIndex = customers.search(id);
  if (customerIndex >-1)
  {
    customers[customerIndex].balance = balance;
    customers[customerIndex].manager = manager;
    customers[customerIndex].name = name;
    customers[customerIndex].ouncesDrank = ouncesDrank;
    SdData.addOrUpdateCustomer(customers[customerIndex]);
    Serial.println("updated customer with set!");
  }
  else {
    // Save the customer data in the ESP32's memory
    temp = Customer(id,balance,manager,ouncesDrank , name);
    customers.push_back(temp);
    Serial.println("Added new customer!");
    SdData.addOrUpdateCustomer(temp);
  }
  Serial.println(id);
  Serial.println(balance);
  Serial.println(manager);
  Serial.println(name);

  
  server.send(200);
}

//pass an existing customer in the body of the post request. If it is found, it will convert 
//all member variables to the ones passed in
void handleEditCustomer(){
  String body = server.arg("plain");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  String id = jsonDoc["ID"];
  float balance = jsonDoc["balance"].as<float>();
  float ouncesDrank = jsonDoc["ouncesDrank"].as<float>();
  bool manager = jsonDoc["manager"].as<bool>();
  String name = jsonDoc["name"];
  bool setVars = jsonDoc["setVars"].as<bool>();
  int addSetOrRefund = jsonDoc["addSetOrRefund"];
  int customerIndex = customers.search(id);
  if(customerIndex>-1)
  {
      customers[customerIndex].balance = balance;
      customers[customerIndex].manager = manager;
      customers[customerIndex].name = name;
      customers[customerIndex].ouncesDrank = ouncesDrank;
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
    jsonDoc["ouncesDrank"] = customers[customerIndex].ouncesDrank;
  }
  else
  {
    jsonDoc["id"] = "NOT FOUND";
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
    customerJson["OuncesDrank"] = customers[i].ouncesDrank;
  }
  String response;
  serializeJson(doc, response); // serialize the JSON document to a string
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

void handlePostSettings() {
  String body = server.arg("plain");
  Serial.print(body);
  Serial.println("POSTED Settings\n");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  pricePerOunce = jsonDoc["pricePerOunce"].as<float>();
  totalQuarts = jsonDoc["totalQuarts"].as<float>();
  maxOunces = jsonDoc["maxOunces"].as<float>();
  SdData.updateSettings();


  Serial.println("New Settings Found!");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to BarBox!");
  lcd.setCursor(0, 1);
  lcd.print("Scan to Begin!");
  lcd.setCursor(0, 2);
  lcd.print("Price/Oz: $");
  lcd.print(pricePerOunce);
  lcd.setCursor(0, 3);
  lcd.print("Oz Left: ");
  lcd.print(totalQuarts*32.0);
  lcd.print("oz");

  server.send(200);
}

void handleGetSettings() {
  Serial.print("got customer");
  DynamicJsonDocument jsonDoc(1024);
  String jsonStr;

  jsonDoc["pricePerOunce"] = pricePerOunce;
  jsonDoc["totalQuarts"] = totalQuarts;
  jsonDoc["maxOunces"] = maxOunces;

  serializeJson(jsonDoc, jsonStr);
  server.send(200, "application/json", jsonStr);
}

void setupAPI() {
  // Set up the ESP32 as an access point
  // WiFi.begin(routerSsid, routerPassword);
  // while (WiFi.status() != WL_CONNECTED) {
  //     delay(1000);
  //     Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi!");
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
  server.on("/settings", HTTP_GET, handleGetSettings);
  server.on("/settings", HTTP_POST, handlePostSettings);
  
  // Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void handleClient() {
  server.handleClient();
}

#include <EspServer.h>

AsyncWebServer server(80);

void handlePostCustomer(AsyncWebServerRequest *request) {
  String body = request->arg("plain");
  Serial.print(body);
  Serial.println("POSTED");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  Customer temp = Customer();
  String id = jsonDoc["ID"];
  float balance = jsonDoc["B"].as<float>();
  bool manager = jsonDoc["M"].as<bool>();
  String name = jsonDoc["N"];
  float ouncesDrank = jsonDoc["OD"].as<float>();

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
  request->send(200);
}

//pass an existing customer in the body of the post request. If it is found, it will convert 
//all member variables to the ones passed in
void handleEditCustomer(AsyncWebServerRequest *request){
  String body = request->arg("plain");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  String id = jsonDoc["ID"];
  float balance = jsonDoc["B"].as<float>();
  float ouncesDrank = jsonDoc["OD"].as<float>();
  bool manager = jsonDoc["M"].as<bool>();
  String name = jsonDoc["N"];
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
   request->send(200);

}

void handleGetCustomer(AsyncWebServerRequest *request) {
  Serial.print("got customer");
  String id = request->arg("id");
  int customerIndex = customers.search(id);
  DynamicJsonDocument jsonDoc(1024);
  String jsonStr;

  if (customerIndex>-1){
    jsonDoc["ID"] = customers[customerIndex].ID;
    jsonDoc["B"] = customers[customerIndex].balance;
    jsonDoc["M"] = customers[customerIndex].manager;
    jsonDoc["N"] = customers[customerIndex].name;
    jsonDoc["OD"] = customers[customerIndex].ouncesDrank;
  }
  else
  {
    jsonDoc["ID"] = "NOT FOUND";
  }

  serializeJson(jsonDoc, jsonStr);
  request->send(200, "application/json", jsonStr);
}

void handleGetAllCustomers(AsyncWebServerRequest *request) {
  DynamicJsonDocument doc(JSON_SIZE); // create a JSON document
  JsonArray customersJson = doc.createNestedArray("customers"); // create a nested array of customers
  Serial.println("Get for all customers received");
  for (int i = 0; i < customers.getSize(); i++) {
    JsonObject customerJson = customersJson.createNestedObject(); // create a nested object for each customer
    customerJson["ID"] = customers[i].ID;
    customerJson["N"] = customers[i].name;
    customerJson["B"] = customers[i].balance;
    customerJson["M"] = customers[i].manager;
    customerJson["OD"] = customers[i].ouncesDrank;
  }
  String response;
  serializeJson(doc, response); // serialize the JSON document to a string
  request->send(200, "application/json", response);
}


void handleRemoveCustomers(AsyncWebServerRequest *request) {
  String body = request->arg("plain");
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, body);
  String id = jsonDoc["ID"];
  int idx = customers.search(id);
  Serial.println(id);
  if (idx >-1) {
    SdData.removeCustomer(customers[idx]);
    customers.deleteAt(idx);
    Serial.print("Customer Deleted!");
     request->send(200);

  }
  else {
    Serial.print("Customer deletion error!!");
     request->send(100);

  }

}

void handlePostSettings(AsyncWebServerRequest *request) {
  String body = request->arg("plain");
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
  request->send(200);
}

void handleGetSettings(AsyncWebServerRequest *request) {
  Serial.print("got settings");
  DynamicJsonDocument jsonDoc(1024);
  String jsonStr;

  jsonDoc["pricePerOunce"] = pricePerOunce;
  jsonDoc["totalQuarts"] = totalQuarts;
  jsonDoc["maxOunces"] = maxOunces;

  serializeJson(jsonDoc, jsonStr);
  request->send(200, "application/json", jsonStr);
}

void handleCheckUpdates(AsyncWebServerRequest *request){

}

void setupServer() {
  // Set up the ESP32 as an access point
  WiFi.begin(routerSSID, routerPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  WiFi.softAP(accessPointSSID, accessPointPassword);
  // Print the IP address of the access point
  Serial.print("Access point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Set up the API routes
  server.on("/updateCheck", HTTP_GET, [](AsyncWebServerRequest *request) {handleCheckUpdates(request);});
  server.on("/postCustomer", HTTP_POST, [](AsyncWebServerRequest *request) {handlePostCustomer(request);});
  server.on("/getCustomer", HTTP_GET, [](AsyncWebServerRequest *request) {handleGetCustomer(request);});
  server.on("/editCustomer", HTTP_POST, [](AsyncWebServerRequest *request) {handleEditCustomer(request);});
  server.on("/allCustomers", HTTP_GET, [](AsyncWebServerRequest *request) {handleGetAllCustomers(request);});
  server.on("/removeCustomers", HTTP_POST, [](AsyncWebServerRequest *request) {handleRemoveCustomers(request);});
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {handleGetSettings(request);});
  server.on("/settings", HTTP_POST, [](AsyncWebServerRequest *request) {handlePostSettings(request);});
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}



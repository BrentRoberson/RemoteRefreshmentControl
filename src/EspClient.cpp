#include <EspClient.h>

Customer requestCustomerData(String id) {
  HTTPClient http;
  String requestURL = String("http://") + serverIP + "/getCustomer?id="+ String(id);
  http.begin(requestURL);
  http.addHeader("Content-Type", "application/json");
  Customer temp = Customer();

  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, payload);
    temp.ID = jsonDoc["ID"].as<String>();
    if(temp.ID!="NOT FOUND"){
      Serial.println(temp.ID);
      temp.balance = jsonDoc["B"].as<float>();
      temp.manager = jsonDoc["M"].as<bool>();
      temp.ouncesDrank = jsonDoc["OD"].as<float>();
    }
  } else {
    Serial.println("Error requesting customer data: " + String(httpCode));
  }
  http.end();
  return temp;
}

void postCustomerData(Customer & customer){
    HTTPClient http;
    // Prepare customer data
    DynamicJsonDocument jsonDoc(1024);
    jsonDoc["ID"] = customer.ID;
    jsonDoc["B"] = customer.balance;
    jsonDoc["M"] = customer.manager;
    jsonDoc["N"] = customer.name;
    jsonDoc["OD"] = customer.ouncesDrank;
    String jsonStr;
    serializeJson(jsonDoc, jsonStr);

    // Send POST request with customer data
    http.begin("http://" + String(serverIP) + "/customer");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(jsonStr);

    if (httpCode > 0) {
      Serial.println("Successfully posted customer data");
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
}


void updateSettings(){
  HTTPClient http;
  String url = String("http://") + serverIP + "/settings";
  Serial.println("requesting settings");

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<1024> doc;
      deserializeJson(doc, payload);

      // Update settings from the received JSON
      pricePerOunce = doc["pricePerOunce"];
      totalQuarts = doc["totalQuarts"];
      maxOunces = doc["maxOunces"];

      // Print updated settings for debugging purposes
      Serial.print("Price per ounce: ");
      Serial.println(pricePerOunce);
      Serial.print("Total quarts: ");
      Serial.println(totalQuarts);
      Serial.print("Max ounces: ");
      Serial.println(maxOunces);
    }
  } else {
    Serial.printf("Request failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void postSettings(){
     if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://" + String(serverIP) + "/postSettings";
    String body;

    DynamicJsonDocument jsonDoc(1024);
    jsonDoc["pricePerOunce"] = pricePerOunce;
    jsonDoc["totalQuarts"] = totalQuarts;
    jsonDoc["maxOunces"] = maxOunces;

    serializeJson(jsonDoc, body);

    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(body);

    if (httpCode >0) {
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("Settings updated successfully");
      } else {
        Serial.print("Error updating settings, HTTP code: ");
        Serial.println(httpCode);
      }
    } else {
      Serial.print("Error updating settings, HTTPClient error: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("Not connected to WiFi, unable to update settings");
  }
}

void setupClient(){
    Serial.begin(115200);
    if(usingAP){
        WiFi.begin(accessPointSSID, accessPointPassword);
        Serial.println("connecting to another ESP32 wifi");
    }
    else{
        WiFi.begin(routerSSID, routerPassword);
        Serial.println("connecting to router wifi");
    }
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}
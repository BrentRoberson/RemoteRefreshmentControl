#include "SDCard.h"


SDCard::SDCard(const String filename) {
  this->filename = filename;
}

void SDCard::updateSettings() {
  digitalWrite(CS_SD, LOW);
  File dataFile = SD.open(filename);
  if (dataFile) {
    DynamicJsonDocument doc(JSON_SIZE);
    DeserializationError error = deserializeJson(doc, dataFile);
    if (error) {
      Serial.println("Failed to deserialize existing JSON data");
      return;
    }
    Serial.println("Successfully deserialized existing JSON data");

    // Update the properties if they exist
      doc["pricePerOunce"] = pricePerOunce;
      doc["totalQuarts"] = totalQuarts;
      doc["maxOunces"] = maxOunces;

    // Serialize the updated JSON data into a string
    dataFile.close();
    dataFile = SD.open(filename, FILE_WRITE);

    if (serializeJson(doc, dataFile) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    dataFile.close();
    Serial.println("Closed file");
    // Write the updated JSON data to the SD card file
  } else {
    Serial.println("Could not open file");
  }
  digitalWrite(CS_SD, HIGH);
}



void SDCard::addOrUpdateCustomer(Customer & customer) {
  digitalWrite(CS_SD, LOW);
  File dataFile = SD.open(filename);
  if (dataFile) {
    DynamicJsonDocument doc(JSON_SIZE);
    DeserializationError error = deserializeJson(doc, dataFile);
    if (error) {
      Serial.println("Failed to deserialize existing JSON data");
      return;
    }
    Serial.println("Successfully deserialized existing JSON data");


    // Check if the customer already exists in the data
    bool foundCustomer = false;
    JsonArray jsonCustomers = doc["customers"];
    for (JsonVariant jsonCustomer : jsonCustomers) {
      String id = jsonCustomer["ID"].as<String>();
      if (id == customer.ID) {
        Serial.println("Customer found");
        jsonCustomer["balance"] = customer.balance;
        jsonCustomer["ouncesDrank"] = customer.ouncesDrank;
        jsonCustomer["manager"] = customer.manager;
        foundCustomer = true;
        break;
      }
    }
    // If the customer does not exist, add it to the JSON data
    if (!foundCustomer) {
      Serial.println("Customer not found");
      JsonObject newJsonCustomer = jsonCustomers.createNestedObject();
      newJsonCustomer["ID"] = customer.ID;
      newJsonCustomer["balance"] = customer.balance;
      newJsonCustomer["ouncesDrank"] = customer.ouncesDrank;
      newJsonCustomer["manager"] = customer.manager;
      newJsonCustomer["name"] = customer.name;
    }
  
    // Serialize the updated JSON data into a string
    dataFile.close();
    dataFile = SD.open(filename, FILE_WRITE);

    if (serializeJson(doc, dataFile) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    dataFile.close();
    Serial.println("Closed file");
    // Write the updated JSON data to the SD card file
  } else {
    Serial.println("Could not open file");
  }
  digitalWrite(CS_SD, HIGH);
}



//only on setup
void SDCard::readInSD() {
  // Open the SD card file and read its contents into a String variable
  digitalWrite(CS_SD,LOW);
  File dataFile = SD.open(filename);
  if (dataFile) {
    DynamicJsonDocument doc(JSON_SIZE);
    DeserializationError error = deserializeJson(doc, dataFile);
    if (error) {
      Serial.println("Failed to deserialize JSON data");
    }
    else{
      Serial.print("no deserialization error\n");
    }
    // Extract the JsonArray from the JSON data
    JsonArray jsonCustomers = doc["customers"];

    // Loop over each JsonObject in the JsonArray and create a new Customer object for each one
    for (JsonVariant jsonCustomer : jsonCustomers) {
      
      String id = jsonCustomer["ID"].as<String>();
      float balance = jsonCustomer["balance"].as<float>();
      float ouncesDrank = jsonCustomer["ouncesDrank"].as<float>();
      bool manager = jsonCustomer["manager"].as<bool>();
      customers.push_back(Customer(id, balance,manager));
    }
    pricePerOunce = doc["pricePerOunce"].as<float>();
    totalQuarts = doc["totalQuarts"].as<float>();
    maxOunces = doc["maxOunces"].as<int>();


  } else {
    Serial.println("Could not open file");
  }
  dataFile.close();
  digitalWrite(CS_SD,HIGH);
}

void SDCard::removeCustomer(Customer & customer) {
  digitalWrite(CS_SD, LOW);

  File dataFile = SD.open(filename);
  if (!dataFile) {
    Serial.println("Could not open file");
    digitalWrite(CS_SD, HIGH);
    return;
  }

  DynamicJsonDocument doc(JSON_SIZE);

  DeserializationError error = deserializeJson(doc, dataFile);
  if (error) {
    Serial.println("Failed to deserialize existing JSON data");
    dataFile.close();
    digitalWrite(CS_SD, HIGH);
    return;
  }
  Serial.println("Successfully deserialized existing JSON data");

  bool customerRemoved = false;
  JsonArray jsonCustomers = doc["customers"];
  String customerID = customer.ID;

  for (size_t i = 0; i < jsonCustomers.size(); i++) {
    JsonObject jsonCustomer = jsonCustomers[i].as<JsonObject>();
    if (jsonCustomer.containsKey("ID") && jsonCustomer["ID"] == customerID) {
      Serial.println("Customer found, removing...");
      jsonCustomers.remove(i);
      customerRemoved = true;
      break;
    }
  }

  if (!customerRemoved) {
    Serial.println("Customer not found");
    dataFile.close();
    digitalWrite(CS_SD, HIGH);
    return;
  }

  // Serialize the updated JSON data into a string
  dataFile.close();
  dataFile = SD.open(filename, FILE_WRITE);

  if (serializeJson(doc, dataFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  dataFile.close();
  Serial.println("Closed file");

  digitalWrite(CS_SD, HIGH);
}



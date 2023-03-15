#include "SDCard.h"

#define JSON_SIZE 1024

SDCard::SDCard(const char* filename) {
  this->filename = filename;
}

void SDCard::addOrUpdateCustomer(Customer* customer) {
  // Open the SD card file and read its contents into a String variable
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    // Parse the existing data into a DynamicJsonDocument object
    DynamicJsonDocument doc(JSON_SIZE);
    if (dataFile.available()) {
      String jsonString = "";
      while (dataFile.available()) {
        jsonString += dataFile.readString();
      }
      DeserializationError error = deserializeJson(doc, jsonString);
      if (error) {
        Serial.println("Failed to deserialize existing JSON data");
        return;
      }
    }

    // Check if the customer already exists in the data
    bool foundCustomer = false;
    JsonArray jsonCustomers = doc["customers"];
    for (JsonVariant jsonCustomer : jsonCustomers) {
      String id = jsonCustomer["ID"].as<String>();
      if (id == customer->ID) {
        jsonCustomer["balance"] = customer->balance;
        foundCustomer = true;
        break;
      }
    }

    // If the customer does not exist, add it to the JSON data
    if (!foundCustomer) {
      JsonObject newJsonCustomer = doc.createNestedObject();
      newJsonCustomer["ID"] = customer->ID;
      newJsonCustomer["balance"] = customer->balance;
    }

    // Serialize the updated JSON data into a string
    String newJsonString;
    serializeJson(doc, newJsonString);

    // Write the updated JSON data to the SD card file
    dataFile.seek(0);
    if (dataFile.print(newJsonString)) {
      Serial.println("Successfully wrote to file");
    } else {
      Serial.println("Failed to write to file");
    }
    dataFile.close();
  } else {
    Serial.println("Could not open file");
  }
}

void SDCard::readCustomers() {
  // Open the SD card file and read its contents into a String variable
  File dataFile = SD.open(filename);
  if (dataFile) {
    String jsonString = "";
    while (dataFile.available()) {
      jsonString += dataFile.readString();
    }
    dataFile.close();

    // Parse the JSON string into a DynamicJsonDocument object
    DynamicJsonDocument doc(JSON_SIZE);
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
      Serial.println("Failed to deserialize JSON data");
    }

    // Extract the JsonArray from the JSON data
    JsonArray jsonCustomers = doc["customers"];

    // Loop over each JsonObject in the JsonArray and create a new Customer object for each one
    for (JsonVariant jsonCustomer : jsonCustomers) {
      String id = jsonCustomer["ID"].as<String>();
      double balance = jsonCustomer["balance"].as<double>();
      customers.push_back(Customer(id, balance));
    }

  } else {
    Serial.println("Could not open file");
  }
}


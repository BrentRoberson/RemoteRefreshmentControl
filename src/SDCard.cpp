#include "SDCard.h"


SDCard::SDCard(const String filename) {
  this->filename = filename;
}

void SDCard::addOrUpdateCustomer(Customer & customer) {
  // Open the SD card file and read its contents into a String variable
  digitalWrite(CS_SD,LOW);
  File dataFile = SD.open(filename);
  if (dataFile) {
    // Serial.println("Successfully opened file: Printing out: ");
    // while (dataFile.available()) {
    // Serial.print((char)dataFile.read());
    // }
    // Serial.println();
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
        Serial.println("Id found");
        jsonCustomer["balance"] = customer.balance;
        jsonCustomer["ouncesDrank"] = customer.ouncesDrank;
        jsonCustomer["manager"] = customer.manager;
        Serial.print("After making json newobject");
        foundCustomer = true;
        break;
      }
    }
    
    // If the customer does not exist, add it to the JSON data
    if (!foundCustomer) {
        Serial.print("customer not found, Mem usage: ");
        Serial.println(doc.memoryUsage());
        JsonObject newJsonCustomer = jsonCustomers.createNestedObject();
        newJsonCustomer["ID"] = customer.ID;
        newJsonCustomer["balance"] = customer.balance;
        newJsonCustomer["ouncesDrank"] = customer.ouncesDrank;
        newJsonCustomer["manager"] = customer.manager;
        Serial.println(doc.memoryUsage());
        if(newJsonCustomer.isNull()){
          Serial.println("Memory alloc error");
        }
        jsonCustomers.add(newJsonCustomer);
        Serial.println("After making json newobject");
      }

    // Serialize the updated JSON data into a string
    dataFile.close();
    dataFile = SD.open(filename, FILE_WRITE);

    if (serializeJson(doc, dataFile) == 0) {
        Serial.println(F("Failed to write to file"));
      }
    dataFile.close();
    Serial.println("closed file");
    // Write the updated JSON data to the SD card file
  } else {
    Serial.println("Could not open file");
  }
  digitalWrite(CS_SD,HIGH);
}


//only on setup
void SDCard::readCustomers() {
  // Open the SD card file and read its contents into a String variable
  digitalWrite(CS_SD,LOW);
  File dataFile = SD.open(filename);
  if (dataFile) {
    // Serial.println("Successfully opened file: Printing out: ");
    // while (dataFile.available()) {
    // Serial.print((char)dataFile.read());}
    // Parse the JSON string into a DynamicJsonDocument object
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

  } else {
    Serial.println("Could not open file");
  }
  dataFile.close();
  digitalWrite(CS_SD,HIGH);
}


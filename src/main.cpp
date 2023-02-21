#include <Arduino.h>
#include <RFID.h>
#include <Drink.h>
#include <Customer.h>
#include <Pitches.h>
#include <LiquidCrystal_I2C.h>

#define SOLENOID 2
#define buzzerPin 4
#define LCDCOL 16;
#define  LCDROW 4;

int numOz; 
bool readError;
String readTag;
DynamicArray<Customer> customers;
DynamicArray<Customer> drinks;
unsigned long currentMillis; 

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); 
  RFIDsetup();
  currentMillis = millis();
  startup();
}

void loop() {
  readTag = waitForTag();
  if(readTag!=""){
    int customerIndex = customers.search(readTag);
    
    if(customerIndex>-1)
    {
      rfidGoodTap();
      //editCustomer/buy drink
      customers[customerIndex].drinks->push_back(Drink(rand()%24, rand()%2000));
      customers[customerIndex].print();
    }
    else{
      rfidBadTap();
      Serial.print("Customer not added, please go to check in to add your card. ");
      Serial.println("For testing purposes, customer is added: ");
      customers.push_back(Customer(readTag, 0));
    }
    Serial.println(readTag);

    // Serial.println("How many oz would you like?");
    // numOz = Serial.parseInt();
    // dispense(numOz, inputTag);
    // Serial.println("Finished dispensing, thank you!");
    // if (customers.getSize() >3)
    // {
    //   customers.printArrayDetails();
    // }
  }
  
}

#include <Arduino.h>
#include <RFID.h>
#include <Drink.h>
#include <Customer.h>
#include <Pitches.h>

#define solenoid 2
#define buzzerPin 4
int numOz; 
bool readError;
String readTag;
Customer Bob;


unsigned long currentMillis; 

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); 
  RFIDsetup();
  currentMillis = millis();
  startup();
}

void loop() {
  DynamicArray<Customer> customers;
  DynamicArray<Customer> drinks;


  // readTag = waitForTag();
  // if(readTag!=""){
  //   int customerIndex = customers.search(readTag);
    
  //   if(customerIndex>-1)
  //   {
  //     rfidGoodTap();
  //     //editCustomer/buy drink
  //     Serial.println("Customer already added: ");
  //   }
  //   else{
  //     rfidBadTap();
  //     Serial.print("Customer not added, please go to check in to add your card. ");
  //     Serial.println("For testing purposes, customer is added: ");
  //     customers.push_back(Customer(readTag, 0));
  //   }
  //   Serial.println(readTag);

  //   // Serial.println("How many oz would you like?");
  //   // numOz = Serial.parseInt();
  //   // dispense(numOz, inputTag);
  //   // Serial.println("Finished dispensing, thank you!");
  //   if (customers.getSize() >5)
  //   {
  //     customers.printArrayDetails();
  //   }
  // }
  
}

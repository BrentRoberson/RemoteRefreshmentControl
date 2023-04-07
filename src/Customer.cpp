#include <Customer.h>



Customer::Customer(String id, float balance, bool manager, String name) {
  this -> ID = id;
  this -> balance = balance;
  this -> ouncesDrank = 0;
  this -> manager = manager;
  this -> name = name;
}

void Customer::print() {
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("ID: ");
  Serial.print(this->ID);
  Serial.print("Balance: ");
  Serial.println(this->balance);
  Serial.print("Ounces Drank: ");
  Serial.println(this->ouncesDrank);

  
}

void removeCustomer() {
  customers[currentScannedIndex].balance = 0;
  customers[currentScannedIndex].ouncesDrank = 0;
  customers.deleteAt(currentScannedIndex);
  SdData.removeCustomer(customers[currentScannedIndex]);
}

void makeManager() {
  customers[currentScannedIndex].manager = true;
}



void Customer::lcdPrint() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Balance: $");
  lcd.print(this->balance);
  lcd.setCursor(0, 1);
  lcd.print("Price/oz: $");
  lcd.print(pricePerOunce);
  lcd.setCursor(0, 2);
  lcd.print("Ounces Left:");
  lcd.print(this->balance/pricePerOunce);
  lcd.setCursor(0, 3);
  lcd.print("Ounces Drank: ");
  lcd.print(this->ouncesDrank);

}


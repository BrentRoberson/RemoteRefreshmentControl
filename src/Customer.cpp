#include <Customer.h>
#include <Drink.h>
#include <Globals.h>
#include <LiquidCrystal_I2C.h>


Customer::Customer(String id, double balance) {
  this -> ID = id;
  this -> balance = balance;
  this -> drinks = new DynamicArray<Drink>(maxDrinks);
  this -> manager = false;
}

void Customer::print() {
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("ID: ");
  Serial.print(this->ID);
  Serial.print(" Ounces Left: ");
  Serial.println(this->balance);

  for(int i = 0; i<this->drinks->getSize(); i++ )
  {
    
    Serial.print("Drink Number: ");
    Serial.print(i+1);
    Serial.print(" Ounces purchased: ");
    Serial.print(drinks->array[i].getOunces());
    Serial.print(" Time Purchased: ");
    Serial.println(drinks->array[i].getTime());
  }
}

void removeCustomer() {
  customers.deleteAt(currentScannedIndex);
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
  lcd.print("Drinks Purchased: ");
  lcd.print(this->drinks->getSize());

}


#include <Customer.h>
#include <Drink.h>

Customer::Customer(String id, float oz) {
  this -> ID = id;
  this -> ozLeft = oz;
  this -> drinks = new DynamicArray<Drink>(10);
}

void Customer::print() {
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("ID: ");
  Serial.print(this->ID);
  Serial.print(" Ounces Left: ");
  Serial.println(this->ozLeft);

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


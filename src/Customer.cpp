#include <Customer.h>



Customer::Customer(String id, float oz) {
  this -> ID = id;
  this -> ozLeft = oz;
  this -> drinks = new DynamicArray<Drink>(10);
}

void Customer::print() {
  Serial.print("ID: ");
  Serial.print(this->ID);
  Serial.print(" Ounces Left: ");
  Serial.print(this->ozLeft);
  Serial.print(" Total Drinks Purchased: ");
  Serial.println(this->drinks->getSize());
}


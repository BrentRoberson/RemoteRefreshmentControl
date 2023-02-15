#include <Customer.h>


Customer::Customer(String id, float oz) {
  ID = id;
  ozLeft = oz;
  drinks = new DynamicArray<Drink>[1];
}

void Customer :: print() {
  Serial.print("ID: ");
  Serial.print(this->ID);
  Serial.print(" Ounces Left: ");
  Serial.print(this->ozLeft);
  Serial.print(" Total Drinks Purchased: ");
  Serial.println(this->drinks->getSize());
}


#include <Jug.h>

volatile int pulseCount;

// Calibration curve coefficients 
//calculated using least-squares regression analysis with numpy

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void openDoor(){
  digitalWrite(DOOR_LOCK, LOW);
  delay(1000);
  digitalWrite(DOOR_LOCK, HIGH);
  delay(500);
  return; //is this the solution?
}

void dispense() {
  pulseCount = 0;
  float calibrationFactor = 2;
  float toOz = 29.5735;
  float totalOz = 0; 
  float displayOz = 0;
  unsigned int dispenseTime = 0;
  float subtractFromBalance = 0;

  // reference to the customer object so it affects both server and client customers
  Customer & currentCustomer = customers[currentScannedIndex];

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dispensed amount: ");
  lcd.setCursor(0,1);
  lcd.print("Oz: ");
  lcd.print(String(totalOz));
  lcd.setCursor(0,2);
  lcd.print("Cost: $");
  lcd.print(String(totalOz/pricePerOunce));
  lcd.setCursor(0,3);
  lcd.print("Balance: $");
  lcd.print(String(currentCustomer.balance));

  while(digitalRead(DISPENSE_BUTTON)==LOW && currentCustomer.balance>totalOz*pricePerOunce){
    digitalWrite(PUMP,HIGH);
    rainbowCycle(15);

    if(dispenseTime+250<millis())
    {
      if(totalOz>10){
        displayOz = (totalOz - 10)*.5+ totalOz;
      }
      lcd.setCursor(4,1);
      lcd.print(String(displayOz));
      lcd.setCursor(7,2);
      lcd.print(String(displayOz*pricePerOunce));
      dispenseTime = millis();
      totalOz += pulseCount/calibrationFactor/toOz;
      displayOz = totalOz;
      pulseCount=0;
    }

  }

  digitalWrite(PUMP,LOW);
  
  subtractFromBalance = totalOz * pricePerOunce;
  currentCustomer.balance -= subtractFromBalance;
  currentCustomer.ouncesDrank += totalOz;
  if (currentCustomer.balance<0){
    currentCustomer.balance = 0.00;
  }
  totalQuarts -= (totalOz/32.0);
  
  SdData.addOrUpdateCustomer(currentCustomer);
  SdData.updateSettings();
  
  Serial.println(totalOz);
  Serial.println();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dispensed amount: ");
  lcd.setCursor(0,1);
  lcd.print("Oz: ");
  lcd.print(String(totalOz));
  lcd.setCursor(0,2);
  lcd.print("Cost: $");
  lcd.print(String(totalOz*pricePerOunce));
  lcd.setCursor(0,3);
  lcd.print("New Balance: $");
  lcd.print(String(currentCustomer.balance));
  Serial.println();
  Serial.println(totalOz);
  Serial.println();
  delay(500);
}

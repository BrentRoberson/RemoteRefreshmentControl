#include <Jug.h>

volatile int pulseCount;

// Calibration curve coefficients 
//calculated using least-squares regression analysis with numpy

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void dispense() {
  pulseCount = 0;
  float calibrationFactor = .6;
  float toOz = 29.5735;
  float totalOz = 0; 
  unsigned int currentTime = 0;
  float subtractFromBalance = 0;
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
  lcd.print(String(customers[currentScannedIndex].balance));
  while(digitalRead(DISPENSE_BUTTON)==LOW && customers[currentScannedIndex].balance>totalOz*pricePerOunce){
    digitalWrite(PUMP,HIGH);
    // if(currentTime+250<millis())
    // {
      lcd.setCursor(4,1);
      lcd.print(String(totalOz));
      lcd.setCursor(7,2);
      lcd.print(String(totalOz*pricePerOunce));
      currentTime = millis();
    // }
    totalOz += pulseCount/calibrationFactor/toOz;
    pulseCount=0;
    delay(250);
  }
  Serial.println();
  Serial.println(totalOz);
  Serial.println();

  digitalWrite(PUMP,LOW);
  if(totalOz<=4){
    totalOz -= (10-totalOz)*.2; //accounts for overshooting
      Serial.println("4");
      Serial.println(totalOz);
      Serial.println();
  }
  else if(totalOz<=8){
    totalOz -= (8-totalOz)*.1; //accounts for overshooting
    Serial.println("8");
    Serial.println(totalOz);
    Serial.println(); 

  }
  else if(totalOz>=14){
    totalOz += (totalOz-10)*.5; //undershooting
    Serial.println("14");
    Serial.println(totalOz);
    Serial.println();
  }
  else if(totalOz>=10){
    totalOz += (totalOz-10)*.3; //undershooting
    Serial.println("after 10");
    Serial.println(totalOz);
    Serial.println();
  }
  if(totalOz<0){
    totalOz = 0;
    Serial.println("after 0");
    Serial.println(totalOz);
    Serial.println();
  }
  Serial.println("before balance update:");
  Serial.println(totalOz);
  Serial.println();
  subtractFromBalance = totalOz * pricePerOunce;
  customers[currentScannedIndex].balance -=subtractFromBalance;
  if (customers[currentScannedIndex].balance<0){
    customers[currentScannedIndex].balance = 0.00;
  }
  Serial.println("after balance update:");
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
  lcd.print(String(customers[currentScannedIndex].balance));
  Serial.println();
  Serial.println(totalOz);
  Serial.println();
  delay(2000);
}

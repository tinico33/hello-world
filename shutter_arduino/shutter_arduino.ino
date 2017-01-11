#include "Volet.h"

const int chambreDownButton = 2;
const int chambreUpButton = 3;
const int mezzanineDownButton = 4;
const int mezzanineUpButton = 5;

Volet bureau;
Volet mezzanine;

void setup() {
  bureau.attach(12, 11, chambreUpButton, chambreDownButton);
  mezzanine.attach(10, 9, mezzanineUpButton, mezzanineDownButton);
  
  Serial.begin(9600);
}

void loop() {  
  bureau.action();
  mezzanine.action();
  
  delay(20);
}

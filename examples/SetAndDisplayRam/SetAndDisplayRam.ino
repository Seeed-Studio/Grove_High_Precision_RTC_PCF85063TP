#include "PCF85063TP.h"
PCF85063TP RTclock; // define a object of PCD85063TP class

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  RTclock.begin();
}

void loop() {
  uint8_t ram_value = RTclock.readRamReg();
  unsigned long elapsed_millis = millis();
  uint8_t value = (elapsed_millis / 1000) % 255;
  Serial.print("Millis: ");
  Serial.println(elapsed_millis);
  if (ram_value == value){
    Serial.println("Not setting ram because values are equal");
    delay(100);
    return;
  }
  Serial.print("Setting ram to: ");
  Serial.println(value);
  RTclock.setRam(value);
  Serial.print("Reading ram: ");
  ram_value = RTclock.readRamReg();
  Serial.println(ram_value);
  delay(500);
}

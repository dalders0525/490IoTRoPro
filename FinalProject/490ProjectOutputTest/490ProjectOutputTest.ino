//Simple Write Pins
int lightPin = 4;
int waterPin = 5;
int fanPin = 6;

//LCD Declarations
#include <SoftwareSerial.h>
#include <LiquidCrystal_PCF8574.h>
#include <stdlib.h>
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  Serial.begin(115200);
  lcd.setBacklight(255);
  lcd.begin(16, 2);
  lcd.setCursor(0,1);
  lcd.print("SETUP");
}

void loop() {

  //Simple LCD Test
  for (int x=0; x<20; x=x+1) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Count: "+String(x));
    delay(300);
  }

  //Flicking lights, fan, water
  digitalWrite(lightPin,HIGH);
  digitalWrite(waterPin,HIGH);
  digitalWrite(fanPin,HIGH);
  delay(5000);
  digitalWrite(lightPin,LOW);
  digitalWrite(waterPin,LOW);
  digitalWrite(fanPin,LOW);

  
}

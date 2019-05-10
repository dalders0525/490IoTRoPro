//Humidity & Temperature Sensor
#include <DHT.h>
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
float humi;
float temp;

//Light Sensor
int lightSensPin=A0;
float lightData;

//UV Sensor
float uvData;
int uvPin=A1;
long sum=0;
int sensorValue=0;

//Soil Sensor
int soilPin=A2;
int soilData=0;
const int AirValue = 520;   //you need to replace this value with Value_1
const int WaterValue = 260;  //you need to replace this value with Value_2
int dry = ((AirValue-WaterValue)*0.75)+WaterValue; // Value which is 3/4 of the way to dry from wet
int moist = ((AirValue-WaterValue)*0.25)+WaterValue; // Value which is 1/4 of the way to dry from wet


//Simple Write Pins
int lightPin = 4;
int waterPin = 5;
int fanPin = 6;

//LCD Declarations
#include <SoftwareSerial.h>
#include <LiquidCrystal_PCF8574.h>
#include <stdlib.h>
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Various other declarations
int fanTime = 0; //tracking if fan is not sufficient for temperature
bool lightState = true;
String dataAsCsv;
int messageIn;
int count = 0;
int letterCount = 0;
String message;


void setup() {
  //Set inputs
  Serial.begin(115200);
  pinMode(lightSensPin,INPUT);
  pinMode(uvPin,INPUT);
  pinMode(soilPin,INPUT);

  //Set LCD
  lcd.setBacklight(255);
  lcd.begin(16, 2);
  lcd.setCursor(0,1);
  lcd.print("SETUP");

  //Set initial states
  digitalWrite(lightPin,HIGH);
  digitalWrite(waterPin,LOW);
  digitalWrite(fanPin,LOW);

}

void lcdWrite(String message) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(message);
}

void loop() {
  // DATA READ
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  lightData = analogRead(lightPin);
  soilData = analogRead(soilPin);
  
  // UV READ
  sum = 0;
  sensorValue = 0;
  for(int i=0;i<1024;i++) {  
    sensorValue=analogRead(A1);
    sum=sensorValue+sum;
    delay(2);
   }   
  sum = sum >> 10;
  uvData = sum*4980.0/1023.0;


  // PRINTING FOR TEST
//  Serial.print("Humidity: ");
  Serial.println(humi);

  Serial.print("Temperature: ");
  Serial.println(temp);

  Serial.print("Light: ");
  Serial.println(lightData);
  
  Serial.print("Soil Humidity: ");
  Serial.println(soilData);

  Serial.print("UV: ");
  Serial.println(uvData);

  Serial.println("\n------------\n");

  // Fan Heat Logic
  if(temp>85) {
    digitalWrite(fanPin,HIGH);
    fanTime=fanTime+5; //5 is the second time of our whole loop. May need adjusting if we change delays.
    if (fanTime>=300) {
      digitalWrite(lightPin,LOW);
      lightState=false;
      Serial.println("LIGHT OFF FOR EXCESSIVE HEAT");
    }
  }
  if((temp<80) && (lightState==false)) {
    digitalWrite(lightPin,HIGH);
    lightState=true;
    Serial.println("LIGHT ON");
  }

  //Fan Moisture Logic
  if (humi>60) {
    digitalWrite(fanPin,HIGH);
    Serial.println("FAN ON DUE TO EXCESSIVE HUMIDITY");
  }
  if (humi<60 && temp<80) {
    digitalWrite(fanPin,LOW);
  }

  // Watering Logic
  if(soilData > dry) {
    digitalWrite(waterPin,HIGH);
    Serial.println("WATER ON");
  }
  else {
    digitalWrite(waterPin,LOW);
    Serial.println("WATER OFF");
  }
  //Water LCD Logic
  if (soilData > dry) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("The soil is dry");
  }
  else if (soilData > moist) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("The soil is lightly moist");
  }
  else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("The soil is wet");
  }

  //Here we Serial print our data, the raspberry pi will pick this up
  dataAsCsv = String(humi) + "," + String(temp) + "," +
              String(lightData) + "," + String(uvData) + "," + 
              String(soilData) + ",";
  Serial.println(dataAsCsv);

  delay(1000);

  //We listen for Serial communication from the pi
  //Possibly I will need a way to communicate success

  for (int i=0; i<3; i=i+1) {
    if(Serial.available()){
      messageIn = Serial.read();
      letterCount = letterCount + 1;
      char y = messageIn;
      message = message + y;
      if (letterCount == 3) {
       Serial.println(message);     
        if (message == "L1;") {
          //digitalWrite(lightPin,HIGH);
          Serial.println("LIGHT TURNED ON BY USER");
          }
        if (message == "L0;") {
          //digitalWrite(lightPin,LOW);
          Serial.println("LIGHT TURNED OFF BY USER");
          }
        if (message == "F1;") {
          //digitalWrite(fanPin,HIGH);
          Serial.println("FAN TURNED ON BY USER");
          }
        if (message == "F0;") {
          //digitalWrite(fanPin,HIGH);
          Serial.println("FAN TURNED OFF BY USER");
        }
       message = "";
       letterCount = 0;
  
      }
    }
  }




  
  if(Serial.available()){
    messageIn = Serial.read();
    Serial.println(messageIn);
  }

  delay(1000);
}

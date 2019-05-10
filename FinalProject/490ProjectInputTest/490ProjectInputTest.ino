//Humidity & Temperature Sensor
#include <DHT.h>
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
float humi;
float temp;

//Light Sensor
int lightPin=A0;
float lightData;

//UV Sensor
float uvData;
int uvPin=A1;
long sum=0;
int sensorValue=0;

//Soil Sensor
int soilPin=A2;
int soilData=0;


void setup() {
  Serial.begin(9600);
  pinMode(lightPin,INPUT);
  pinMode(uvPin,INPUT);
  pinMode(soilPin,INPUT);
}

void loop() {
  // DATA READ \\
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  lightData = analogRead(lightPin);
  soilData = analogRead(soilPin);
  
  // UV READ \\
  sum = 0;
  sensorValue = 0;
  for(int i=0;i<1024;i++) {  
    sensorValue=analogRead(A1);
    sum=sensorValue+sum;
    delay(2);
   }   
  sum = sum >> 10;
  uvData = sum*4980.0/1023.0;


  // PRINTING FOR TEST \\
  Serial.print("Humidity: ");
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

  delay(5000);
}

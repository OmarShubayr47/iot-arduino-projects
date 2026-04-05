#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("DHT11 Ready");
}

void loop() {
  delay(2000); // DHT11 needs 2 seconds between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius by default

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT11");
    return;
  }
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  Serial.println("--------------------");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %"); 
  
  Serial.print("Heat Index: ");
  Serial.print(heatIndex);
  Serial.println(" °C");

  if (temperature > 30.0) {
  Serial.println("WARNING: High temperature detected");

}
  Serial.println("--------------------");
}



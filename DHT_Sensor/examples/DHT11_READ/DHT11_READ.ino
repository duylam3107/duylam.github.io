// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

byte thermometro[8] = //icon for termometer
{
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};


byte igrasia[8] = //icon for water droplet
{
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
};

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hshop.vn  DHT11");
  dht.begin();
  lcd.createChar(1, thermometro);
  lcd.createChar(2, igrasia);
}

void loop() {
  delay(250);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //  float hif = dht.computeHeatIndex(f, h);
  //  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.println();
  lcd.setCursor(0, 1);
  lcd.write(1);
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");
  lcd.print(" ");
  lcd.write(2);
  lcd.print(h);
  lcd.print("%");

}

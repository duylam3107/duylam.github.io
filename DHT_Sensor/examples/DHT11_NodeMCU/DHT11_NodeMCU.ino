// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

/* LCD Library : https://bitbucket.org/hshopvn/liquidcrystal_i2c/downloads/liquidcrystal_i2c.zip
 * Toolboard : NodeMCU 1.0
 * Pin DHT : D3
 * LCD connect : SCL pin with NodeMCU D1 pin 
 * LCD connect : SDA pin with NodeMCU D2 pin
 */

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN D3
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
  lcd.print("NodeMCU  DHT11");
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

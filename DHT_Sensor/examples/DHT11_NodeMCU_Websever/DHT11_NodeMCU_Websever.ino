#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "SSID-here";  // Enter SSID here
const char* password = "Password-here";  //Enter Password here
ESP8266WebServer server(80);

float Temperature;
float Humidity;

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
  Serial.begin(115200);
  delay(100);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Wait Connect.....");
  lcd.createChar(1, thermometro);
  lcd.createChar(2, igrasia);
  delay(1000);
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print(".");
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Your IP Address");
  lcd.setCursor(1, 1);
  lcd.print(WiFi.localIP());
  delay(4000);
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
}
void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  Serial.println("ONConnect");
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("NodeMCU DHT-11");
  lcd.setCursor(0, 1);
  lcd.write(1);
  lcd.print(Temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.print(" ");
  lcd.write(2);
  lcd.print(Humidity);
  lcd.print("%");
  server.send(200, "text/html", SendHTML(Temperature, Temperature, Humidity));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float TempCstat, float TempFstat, float Humiditystat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>NodeMCU DHT11 Sensor</title>\n";
  ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {margin: 20px auto 30px;}\n";
  ptr += ".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  ptr += ".humidity-icon{background-color: #3498db;width: 120px;height: 120px;border-radius: 50%;line-height: 40px;}\n";
  ptr += ".humidity{font-weight: 300;font-size: 90px;color: #3498db;}\n";
  ptr += ".temperature-icon{background-color: #f39c12;width: 120px;height: 120px;border-radius: 50%;line-height: 40px;}\n";
  ptr += ".temperature{font-weight: 300;font-size: 90px;color: #f39c12;}\n";
  ptr += ".superscript{font-size: 40px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  ptr += ".data{padding: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";

  ptr += "<div id=\"webpage\">\n";

  ptr += "<h1>NodeMCU DHT11 Temperature & Humidity Sensor</h1>\n";
  ptr += "<div class=\"data\">\n";
  ptr += "<div class=\"side-by-side temperature-icon\">\n";
  ptr += "<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n";
  //   ptr +="width=\"48px\" height=\"70px\" viewBox=\"0 0 48 70\" enable-background=\"new 0 0 48 70\" xml:space=\"preserve\">\n";
  ptr += "width=\"60px\" height=\"90px\" viewBox=\"0 -2 10 22\" enable-background=\"new 0 -2 10 22\" xml:space=\"preserve\">\n";
  ptr += "<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142\n";
  ptr += "c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491\n";
  ptr += "c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463\n";
  ptr += "c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394\n";
  ptr += "c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>\n";
  ptr += "</svg>\n";
  ptr += "</div>\n";
  //   ptr +="<div class=\"side-by-side temperature-text\">Temperature</div>\n";
  ptr += "<div class=\"side-by-side temperature\">";
  ptr += " &nbsp &nbsp ";
  ptr += (int)TempCstat;
  ptr += "&nbsp <span class=\"superscript\"> *C</span></div>\n";
  ptr += "</div>\n";
  ptr += "<br>\n";
  ptr += "<br>\n";
  ptr += "<br>\n";
  ptr += "<div class=\"data\">\n";
  ptr += "<div class=\"side-by-side humidity-icon\">\n";
  ptr += "<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n\"; width=\"60px\" height=\"90px\" viewBox=\"0 0 13 14\" enable-background=\"new 0 0 13 14\" xml:space=\"preserve\">\n";
  ptr += "<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057\n";
  ptr += "c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>\n";
  ptr += "</svg>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"side-by-side humidity\">";
  ptr += " &nbsp &nbsp ";
  ptr += (int)Humiditystat;
  ptr += "&nbsp <span class=\"superscript\">%</span></div>\n";
  ptr += "</div>\n";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

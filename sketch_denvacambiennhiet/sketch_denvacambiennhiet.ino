 /*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-relay-module-ac-web-server/

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Biến nhiệt độ
float Temperature = 0;

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true

// Set number of relays
#define NUM_RELAYS  2

#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {5};

//Tắt mở thủ công
int manual = 1;

// Replace with your network credentials
const char* ssid = "Thor";
const char* password = "tumotdenchin";

const char* PARAM_INPUT_1 = "relay";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

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

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;background-image: url('https://png.pngtree.com/thumb_back/fw800/back_our/20190621/ourmid/pngtree-swallow-nest-nourishing-beauty-banner-image_180315.jpg');height: 92%; background-size:cover;}
    h4 {}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    * {
    margin: 0;
    padding-top: 10px;
    box-sizing: border-box;
}

body {
    height: 100vh;
    justify-content: center;
    align-items: center;
}

.container {
    position: relative;
    top: 50px;
}

input {
    position: relative;
    width: 200px;
    height: 100px;
    background-color: #3d3459;
    border-radius: 60px;
    box-shadow: inset 0 0 10px rgba(0, 0, 0, 0.2);
    -webkit-appearance: none;
    outline: none;
    cursor: pointer;
    transition: all 0.5s ease-in-out;
}

input::before {
    content: "";
    position: absolute;
    width: 100px;
    height: 100px;
    border-radius: 50%;
    top: 0;
    left: 0;
    background-color: #817d96;
    transform: scale(0.9);
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.2);
    transition: 0.5s;
}

input:checked::before {
    left: 100px;
    background-color: #fff;
    box-shadow: 0 0 60px 30px #699ba9, 0 0 100px 60px #303f52;
}

.thread {
    position: relative;
    width: 5px;
    height: 100px;
    background-color: #323941;
    left: 35px;
    bottom: 60px;
    z-index: -1;
    animation: off 0.5s ease;
}

.threadShadow {
    position: absolute;
    width: 50%;
    height: 100%;
    background-color: #39414a;
    right: 0;
    opacity: 0;
    transition: all 0.4s ease;
}

.threadEnd {
    position: relative;
    width: 25px;
    height: 40px;
    background-color: #3a309b;
    bottom: 65px;
    left: 25px;
    border-radius: 20px 20px 10px 10px;
    overflow: hidden;
    cursor: pointer;
    transition: all 0.4s ease;
    animation: off 0.5s ease;
}

.threadEndShadow {
    width: 0;
    height: 0;
    border-top: 30px solid #8378ff;
    border-left: 25px solid transparent;
    opacity: 0;
    transition: all 0.4s ease;
}

.container.on .threadEndShadow, .container.on .threadShadow{
    opacity: 1;
}

.container.on .threadEnd, .container.on .thread {
    animation: on 0.8s ease;
}

.container.on .threadEnd {
    background-color: #695dff;
}

@keyframes on {
    0% {
        transform: translateY(0px);
    }
    25% {
        transform: translateY(20px);
    }
    50% {
        transform: translateY(-10px);
    }
    65% {
        transform: translateY(0px);
    }
    80% {
        transform: translateY(-5px);
    }
    95% {
        transform: translateY(0px);
    }
    100% {
        transform: translateY(0px);
    }
}

@keyframes off {
    0% {
        transform: translateY(0px);
    }
    50% {
        transform: translateY(20px);
    }
    100% {
        transform: translateY(0px);
    }
}


  </style>
  
</head>
<body>
    <div class="tieude">
      <h2>NHÀ YẾN THÔNG MINH</h2>
    </div>
    <div class="container">
        <h4>Bật/Tắt Tự Động</h4>
        <input type="checkbox" onchange="toggleCheckbox1(this)">
  
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
  xhr.send();
}</script>

<script>function toggleCheckbox1(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/setauto?manual=1", true); }
  else { xhr.open("GET", "/setauto?manual=0", true); }
  xhr.send();
}</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    buttons = "<h4>Temperature: "+ String(Temperature)+ " C </h4>";
    for(int i=1; i<=NUM_RELAYS; i++){
      String relayStateValue = relayState(i);
      buttons+= "<h4>Bật/Tắt Đèn #" + String(i) + " - GPIO " + relayGPIOs[i-1] + "</h4><label class=\"container\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"></label>";
      return buttons;
    }
    
  }
  return String();
}

String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
  for(int i=1; i<=NUM_RELAYS; i++){
    pinMode(relayGPIOs[i-1], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relayGPIOs[i-1], HIGH);
      
    }
    else{
      digitalWrite(relayGPIOs[i-1], LOW);
      
    }
  }
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  // Send a GET request to <ESP_IP>/setauto?manual=<inputMessage>
  server.on("/setauto", HTTP_GET, [](AsyncWebServerRequest *request){
    String inputMessage;
    // GET input1 value on <ESP_IP>/setauto?manual=<inputMessage>
    if (request->hasParam("manual")) {
      inputMessage = request->getParam("manual")->value();
      manual = inputMessage.toInt();
    }
  });

  // Send a GET request to <ESP_IP>/update?relay=<inputMessage>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if(RELAY_NO){
        Serial.print("NO ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], !inputMessage2.toInt());
        
      }
      else{
        Serial.print("NC ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], inputMessage2.toInt());
        
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
      
    }
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
}
  
void loop() {
  Temperature = dht.readTemperature();
  //
  
  
  //
  if (isnan(Temperature)) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }
   if(isnan(Temperature) > 25){
     if(manual == 1){
      pinMode(5, OUTPUT);
      digitalWrite(5, HIGH);
     }
   }else{
     if(manual == 1){
      pinMode(5, OUTPUT);
      digitalWrite(5, LOW);
     }
   }
}

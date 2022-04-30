#define SW_VERSION " ThinkSpeak.com" // SW version will appears at innitial LCD Display
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <MCP3008.h>
#include <DHT.h>//sensor
#include <Wire.h> //lcd
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);//lcd

uint32_t tsLastReport = 0;
void onBeatDetected()
{
    ;
}
 
 
#define DHTPIN D4    // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321)
DHT dht(DHTPIN, DHTTYPE);
const char* MY_SSID = "NSR";   // turn on mobile hotspot and enter wifi name ok
const char* MY_PWD = "ssks6665";    // password
 
WiFiClient client;

const char* TS_SERVER = "api.thingspeak.com";
String TS_API_KEY = "B1KAOD1N1HFTRH8D";

const int xPin = 0; 
const int yPin = 1; 
#define CS_PIN    D8
#define CLOCK_PIN D5
#define MOSI_PIN  D7
#define MISO_PIN  D6
 
int gas=0;
int gas1=1;
int water=2;
int ldr=3;
int relay=D3;
int turbidity=4;

//WiFiClientSecure client;

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

/*
   Connecting WiFi
 **/
void connectWifi()
{
  Serial.print("Connecting to " + *MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("");
}
/*
   Sending Data to Thinkspeak Channel
 **/
void sendDataTS(void)
{ 
 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
 if (isnan(h) || isnan(t) || isnan(f)) {
   Serial.println(F("Failed to read from DHT sensor!"));
   return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.println(h);
  lcd.clear();
   lcd.print("Humidity: ");
  lcd.print(h);
  delay(1000);
      Serial.print(F(" Temperature: "));
  Serial.println(t);
    lcd.clear();
   lcd.print("temp: ");
  lcd.print(t);
  delay(1000);
      gas = adc.readADC(0);
 Serial.println("gas value");
   lcd.clear();
   lcd.print("gas: ");
  lcd.print(gas);
  delay(1000);
   int y = adc.readADC(1);
  Serial.print("gas1 value : ");
  Serial.println(y);
    lcd.clear();
   lcd.print("gas1: ");
  lcd.print(y);
  delay(1000);
  int z = adc.readADC(2);
  Serial.print("water level : ");
  Serial.println(z);
    lcd.clear();
   lcd.print("water level: ");
  lcd.print(z);
  delay(1000);
    int a = adc.readADC(3);
  Serial.print("ldr value : ");
  Serial.println(a);
    lcd.clear();
   lcd.print("ldr: ");
  lcd.print(a);
  delay(1000);
    int b = adc.readADC(4);
  Serial.print("turbidity value : ");
  Serial.println(b);
    lcd.clear();
   lcd.print("turbidity: ");
  lcd.print(b);
  delay(1000);
  if(h>45){
    digitalWrite(D3,LOW);
    delay(1000);
  }
else if(t>30){
    digitalWrite(D3,LOW);
    delay(1000);
  }


  else if(gas<250){
    digitalWrite(D3,LOW);
    delay(1000);
  }

else if(z<450){
    digitalWrite(D3,LOW);
    delay(1000);
  }

 else if(a<550){
    digitalWrite(D3,LOW);
    delay(1000);
  }
else if(b<550){
    digitalWrite(D3,LOW);
    delay(1000);
  }
  else{
    digitalWrite(D3,HIGH);
    delay(1000);
  }



 
  if (client.connect(TS_SERVER, 80))
  {
    String postStr = TS_API_KEY;
    postStr += "&field1=";
    postStr += String(h);
     postStr += "&field2=";
    postStr += String(t);
    postStr += "&field3=";
    postStr += String(z);
    postStr += "&field4=";
    postStr += String(a);
    postStr += "&field5=";
    postStr += String(b);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + TS_API_KEY + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(1000);
  }
  client.stop();
}
void setup()
{
  pinMode(gas,INPUT);
  pinMode(gas1,INPUT);
  pinMode(ldr,INPUT);
  pinMode(water,INPUT);
  pinMode(turbidity,INPUT);
  pinMode(relay,OUTPUT);
  Serial.begin(115200);
  delay(10);
  dht.begin();
      digitalWrite(D3,HIGH);
 
       lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();  delay(2000);
  connectWifi();

}
void loop()
{
  lcd.setCursor(0,0);
  sendDataTS();
  delay(10);  
}

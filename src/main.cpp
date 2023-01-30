#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int buttonLed = 0;
const int ledPin = LED_BUILTIN;

int val = 0;
int old_val = 0;
int str = 0; //val almacena el estado del botonint old_val = 0; // almacena el antiguo valor de valint str = 0; //

const char* ssid = "Comunidad IoT";
const char* pass = "Sistecredito2023*";

const char* mqtt_client = "192.168.0.105";
const char* mqtt_pub = "LED/3";

WiFiClient espclient;
PubSubClient client(espclient);

void setup_wifi(){
  delay(10);

  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Wifi Connected IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){

  while (!client.connected())
  {
    Serial.print("Attemting MQTT connection...");
    String clientId = "esp8266";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");

      if (client.subscribe(mqtt_pub))
      {
        Serial.print("Connecting to: subcriptor");
      }
      else{
        Serial.print("Failed, rc=");
        Serial.print(client.state());
        Serial.print("Try again in 5 seconds");
        delay(5000);
      } 
    }
  }  
}

void setup(){
  
  pinMode(buttonLed, INPUT_PULLUP);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_client, 1883);
}

void loop() {

  if(!client.connected()){
    reconnect();
  }
  if(client.connected()){
    val= digitalRead(buttonLed); // lee el estado del Boton    if ((val == HIGH) && (old_val == LOW))
    if((val == HIGH)&& (old_val == LOW))
    {
    str= 1 - str;
    delay(10);
    }
    old_val = val; // valor del antiguo estado    if (str == 0)
    if (str==0)
    {
      client.publish(mqtt_pub, "on");
      digitalWrite(ledPin, 0);
      delay(300);
    }
    else    {
      client.publish(mqtt_pub, "off");
      digitalWrite(ledPin, 1); 
      delay(300);
    }
  }
  
  client.loop();
}
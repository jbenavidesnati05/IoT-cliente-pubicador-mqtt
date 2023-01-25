#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Comunidad IoT";
const char* pass = "Sistecredito2023*";

const char* mqtt_client = "192.168.0.101";
const char* mqtt_pub = "LED/2";

const int button_led = 0;
int str = 0;

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
  
  pinMode(button_led, INPUT_PULLUP);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_client, 1883);
}

void loop() {

  if(!client.connected()){
    reconnect();
  }
  if(client.connected()){
    int str = digitalRead(button_led);

    if (str == 0)
    {
      client.publish(mqtt_pub, "on");
      delay(1000);
    }
    else if (str == 1)
    {
      client.publish(mqtt_pub, "off");
      delay(1000);
    }
  }
  client.loop();
}


#include <project.h>

const char* mqtt_server = "m2.wqtt.ru";
const int mqtt_port = 11799;
const char* mqtt_user = "u_UDQTRO";
const char* mqtt_password = "A3hO7Xfb";
const bool retain_flag = false;

// RELAY
const char* relay_topic = "WorkspaceLight/Light/Status";
const String Relay_Top =  "WorkspaceLight/Light";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
    
  String data_pay;
  for (int i = 0; i < length; i++) {
    data_pay += String((char)payload[i]);
  }

    Serial.print(String(topic));
    Serial.println(data_pay);
    
  if( String(topic) == Relay_Top ){
        if(data_pay == "ON" || data_pay == "1") LoadState = true;
        if(data_pay == "OFF" || data_pay == "0") LoadState = false;
    }

    updateStatePins();
}

void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266-" + WiFi.macAddress();

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password) ) 
    {
      Serial.println("connected");
      
      client.subscribe( (Relay_Top + "/#").c_str() );
      client.publish(relay_topic, String(LoadState).c_str(), retain_flag);

    } else 
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
  }
}

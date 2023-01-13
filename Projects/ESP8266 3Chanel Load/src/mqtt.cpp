#include <project.h>

#define NAMELENGHT 10

String DeviceName = "EspOneChanel_";

const char* mqtt_server = "m2.wqtt.ru";
const int mqtt_port = 11799;
const char* mqtt_user = "u_UDQTRO";
const char* mqtt_password = "A3hO7Xfb";
const bool retain_flag = false;
bool IsFirstTimeStart = true;

// Topics
String Relay_state_topic = "Relay/State";
String Relay_status_topic = "Relay/Status";
String Termometr_topic = "Termometr";
String Humidity_topic = "Humidity";
String Termostat_state_topic = "Termostat/State";
String Termostat_tempset_topic = "Termostat/TempSet";
String Termostat_status_topic = "Termostat/Status";

WiFiClient espClient;
PubSubClient client(espClient);

void NameGenerator()
{
  String letters[40] = 
  {"a", "b", "c", "d", "e", "f","g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", 
  "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

  for(unsigned int i = 0; i<NAMELENGHT; i++)
    {
     DeviceName = DeviceName + letters[random(0, 40)];
    }

    for (int i = 0; i < 20; i++)
    {
      EEPROM.write(50 + i, DeviceName[i]);
    }
    EEPROM.commit();
    Serial.print("Generated name: ");
    Serial.println(DeviceName);
    String Check = "";
    for (int  i = 0; i < 20; i++)
    {
      Check += (char)EEPROM.read(50 + i);
    }
    Serial.print("Checked name in EEPROM: ");
    Serial.println(Check);
    

}

void TopicGenerator()
{
  String Buf = "";

  Buf = DeviceName + "/" + Relay_state_topic;
  Relay_state_topic = Buf;
  Serial.println(Buf);

  Buf = DeviceName + "/" + Relay_status_topic;
  Relay_status_topic = Buf;
  Serial.println(Buf);

  Buf = DeviceName + "/" + Termometr_topic;
  Termometr_topic = Buf;
  Serial.println(Buf);

  Buf = DeviceName + "/" + Humidity_topic;
  Humidity_topic = Buf;
  Serial.println(Buf);

  Buf = DeviceName + "/" + Termostat_state_topic;
  Termostat_state_topic = Buf;
  Serial.println(Buf);

  Buf = DeviceName + "/" + Termostat_tempset_topic;
  Termostat_tempset_topic = Buf;
  Serial.println(Buf);

  Buf = DeviceName + "/" + Termostat_status_topic;
  Termostat_status_topic = Buf;
  Serial.println(Buf);
}

void callback(char* topic, byte* payload, unsigned int length) {
    
  String data_pay;
  for (int i = 0; i < length; i++) {
    data_pay += String((char)payload[i]);
  }

    Serial.print(String(topic));
    Serial.print(": ");
    Serial.println(data_pay);


  if( String(topic) == Relay_state_topic )
  {
      if(data_pay == "ON" || data_pay == "1")
      {
         LoadState = true;
         client.publish(Relay_status_topic.c_str(), String("1").c_str(), retain_flag);
      }

      if(data_pay == "OFF" || data_pay == "0")
      {
         LoadState = false;
         client.publish(Relay_status_topic.c_str(), String("0").c_str(), retain_flag);
      }

      updateStatePins();

  }

  if( String(topic) == Termostat_state_topic )
  {
    if(data_pay == "ON" || data_pay == "1")
    {
      Termostat_Set = true;
      client.publish(Termostat_state_topic.c_str(), String("1").c_str(), retain_flag);
    }

    if(data_pay == "OFF" || data_pay == "0")
    {
      Termostat_Set = false;
      client.publish(Termostat_state_topic.c_str(), String("0").c_str(), retain_flag);
    }
  }

  if( String(topic) == Termostat_tempset_topic )
  {
    Termostat_Set_Temp = data_pay.toInt();
  }


}

void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");

    if (client.connect(DeviceName.c_str(), mqtt_user, mqtt_password) ) 
    {
      Serial.println("connected");
      
      client.subscribe( (Relay_state_topic + "/#").c_str() );
      client.subscribe( (Termostat_state_topic + "/#").c_str() );
      client.subscribe( (Termostat_tempset_topic + "/#").c_str() );

      

    } else 
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
  }
}

void NameInit()
{
  String Name = "";
  for (int  i = 0; i < 20; i++)
    {
      Name += (char)EEPROM.read(50 + i);
      if(i == 2)
      {
        Serial.println(Name);

        if(Name == "Esp")
        {
          Serial.println("Second start");
        }
        else
        {
          Serial.println("First start");
          NameGenerator();
        }
      }
    }
    DeviceName = Name;
    Serial.println(DeviceName);
    TopicGenerator();

}

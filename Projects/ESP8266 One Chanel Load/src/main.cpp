// использование встроенного OTA update
// зайди на адрес x.x.x.x/ota_update для открытия страницы обновления
// Скетч/Экспорт бинарного файла (для получения файла прошивки)

#define ESPLED 14
#define ESPLED_ON 254
#define ESPLED_OFF 0
#define Load 13
#define Button 12
#define ON 1
#define OFF 0

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LittleFS.h>
#include <PubSubClient.h>
#include <GyverPortal.h>
#include <EEPROM.h>

// MQTT
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

struct LoginPass {
  char ssid[20];
  char pass[20];
};
LoginPass lp; 

boolean ButtonState = false;
boolean LoadState = false;
unsigned int Led_Status;

GyverPortal ui(&LittleFS); // для проверки файлов

// конструктор страницы
void build() 
{
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);

   if (ui.uri("/wifi")) 
   {
    GP.FORM_BEGIN("/wifi");
    GP.TEXT("lg", "Login", lp.ssid);
    GP.BREAK();
    GP.TEXT("ps", "Password", lp.pass);
    GP.SUBMIT("Submit");
    GP.BUTTON_LINK("/", "Back");
    GP.FORM_END();
   
   } else if(ui.uri("/Light"))
      {
        GP.FORM_BEGIN("/Light");
        GP.UPDATE("led");
        GP.BUTTON("btn", "Light");
        GP.BREAK();
        GP.LED_RED("led", LoadState);
        GP.BUTTON_LINK("/", "Back");
        GP.FORM_END();        
      } else 
        {
          GP.BUTTON_LINK("/wifi", "WiFi Setup");
          GP.BUTTON_LINK("/Light", "Light");
          GP.BUTTON_LINK("/ota_update", "OTA Update");
        
        }
  GP.BUILD_END();
}

void action()
{
  if (ui.click("btn"))
  {
    if(ButtonState == true)
    {
      ButtonState = false;
      analogWrite(ESPLED, ESPLED_OFF);
      Led_Status = ESPLED_OFF;
      digitalWrite(Load, OFF);
    }
    else
    {
      ButtonState = true;
      analogWrite(ESPLED, ESPLED_ON);
      Led_Status = ESPLED_ON;
      digitalWrite(Load, ON);
    }
    LoadState = ButtonState;
    client.publish(relay_topic, String(LoadState).c_str(), retain_flag);
    Serial.print("Button State: ");
    Serial.println(ButtonState);
  }
}

void buildWiFi(GyverPortal& p) 
{
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);

  GP.FORM_BEGIN("/login");
  GP.TEXT("lg", "SSID", lp.ssid);
  GP.BREAK();
  GP.TEXT("ps", "Password", lp.pass);
  GP.SUBMIT("Submit");
  GP.FORM_END();

  GP.BUILD_END();
}

void actionWiFi(GyverPortal& p) 
{
  if (p.form("/login")) {      // кнопка нажата
    p.copyStr("lg", lp.ssid);  // копируем себе
    p.copyStr("ps", lp.pass);
    EEPROM.put(0, lp);              // сохраняем
    EEPROM.commit();                // записываем
    WiFi.softAPdisconnect();        // отключаем AP
    ESP.restart();
  }
}


void updateStatePins(void){
    if(LoadState)
    {
      digitalWrite(Load, ON);
      analogWrite(ESPLED, ESPLED_ON);
      Led_Status = ESPLED_ON;
    }else
    {
      digitalWrite(Load, OFF);
      analogWrite(ESPLED, ESPLED_OFF);
      Led_Status = ESPLED_OFF;
    }
}

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


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266-" + WiFi.macAddress();
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password) ) {
      Serial.println("connected");
      
      client.subscribe( (Relay_Top + "/#").c_str() );
      client.publish(relay_topic, String(LoadState).c_str(), retain_flag);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() 
{
  pinMode(ESPLED, OUTPUT);
  pinMode(Load, OUTPUT);
  analogWrite(ESPLED, ESPLED_OFF);
  Led_Status = ESPLED_OFF;
  pinMode(Button, INPUT);
  digitalWrite(Load, LOW);

  Serial.begin(115200);
  
  EEPROM.begin(100);
  EEPROM.get(0, lp);

  Serial.print("Connect to: ");
  Serial.println(lp.ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(lp.ssid, lp.pass);
  int cnt = 0;


  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    cnt++;
    if(Led_Status == 0)
    {
      analogWrite(ESPLED, ESPLED_ON);
      Led_Status = ESPLED_ON;
    }
    else
    {
      analogWrite(ESPLED, ESPLED_OFF);
      Led_Status = ESPLED_OFF;
    }
    if(cnt > 50)
    {
      Serial.println("Portal start");

      // запускаем точку доступа
      WiFi.mode(WIFI_AP);
      WiFi.softAP("WiFi Config");

      // запускаем портал
      GyverPortal wifiui;
      wifiui.attachBuild(buildWiFi);
      wifiui.start();
      wifiui.attach(actionWiFi);

      // работа портала
      while (wifiui.tick());
      }
    }

  analogWrite(ESPLED, ESPLED_OFF);
  Led_Status = ESPLED_OFF;
  Serial.println();
  Serial.print("Connected! Local IP: ");

  Serial.println(WiFi.localIP());

  // подключаем конструктор и запускаем
  ui.attachBuild(build);
  ui.start();
  ui.attach(action);
  ui.enableOTA();   // без пароля

  if (!LittleFS.begin()) Serial.println("FS Error");
  ui.downloadAuto(true);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() 
{
  ui.tick();
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
}
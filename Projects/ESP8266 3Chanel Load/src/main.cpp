#include <project.h>


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
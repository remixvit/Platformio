#include <project.h>


float Temp;
unsigned int Humidity;

void setup() 
{
  HardwareSetup();

  
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
    OnBoardLed.Toggle();
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

  OnBoardLed.SetOff();
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
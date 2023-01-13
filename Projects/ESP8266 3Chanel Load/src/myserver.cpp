#include <project.h>
#define TERMOSTAT_DEFAULT_TEMP 25
#define TERMOSTAT_MIN_TEMP     20
#define TERMOSTAT_MAX_TEMP     30
#define TERMOSTAT_STEP_TEMP    1

LoginPass lp;
GyverPortal ui(&LittleFS);

#define PLOT_SIZE 100
int16_t arr[2][PLOT_SIZE];
uint32_t dates[PLOT_SIZE];
const char *names[] = {"kek", "puk",};


void build() 
{
  GP.BUILD_BEGIN(500);
  GP.THEME(GP_DARK);
  GP.PAGE_TITLE("ESP One Chanel", "tit");

  GP.UI_MENU("Menu", GP_RED);
  GP.UI_LINK("/", "Home");
  GP.UI_LINK("/settings", "Settings");
  GP.UI_LINK("/update", "Update");
  GP.UI_BODY();
  GP.GRID_RESPONSIVE(600);

  if (ui.uri("/"))
  {
    GP.HR();
    GP.HR();
    GP.BLOCK_TAB_BEGIN("Обогрев");
    M_BOX(GP_JUSTIFY, GP.LABEL("Обогреватель"); M_BOX(GP_RIGHT, GP.SWITCH("HeatButton"); GP.LED("");););
    M_BOX(GP_JUSTIFY, GP.LABEL("Термостат"); M_BOX(GP_RIGHT, GP.SWITCH("TermostatButton"); GP.LED("");););
    M_BOX(GP_JUSTIFY, GP.LABEL("Температура"); GP.SLIDER("sld", TERMOSTAT_DEFAULT_TEMP, TERMOSTAT_MIN_TEMP, TERMOSTAT_MAX_TEMP, TERMOSTAT_STEP_TEMP););
    GP.BLOCK_END();
    GP.HR();

    GP.HR();
    GP.BLOCK_TAB_BEGIN("Мониторинг");
    M_BOX(GP_JUSTIFY, GP.LABEL("Температура"); GP.LABEL("25,6*C", "HDC_Temp"););
    M_BOX(GP_JUSTIFY, GP.LABEL("Влажность");   GP.LABEL("49%", "HDC_Humi"););
    GP.PLOT_STOCK_DARK<2, PLOT_SIZE>("plot", names, dates, arr);
    GP.BLOCK_END();
    GP.HR();
    GP.HR();
  }
  else if(ui.uri("/settings"))
  {
    GP.HR();
    GP.HR();
    GP.BLOCK_TAB_BEGIN("WiFi Settings");
    M_BOX
    (
      GP_JUSTIFY, 
      GP.FORM_BEGIN("/wifi");
      GP.TEXT("lg", "Login", lp.ssid);
      GP.BREAK();
      GP.TEXT("ps", "Password", lp.pass);
      GP.SUBMIT("Submit"); 
      GP.FORM_END();
    );
    M_BOX(GP_RIGHT, GP.TITLE(DeviceName););
    GP.BLOCK_END();
    GP.HR();
    GP.HR();
  }
  else if(ui.uri("/update"))
  {
    GP.HR();
    GP.HR();
    GP.BLOCK_TAB_BEGIN("WiFi Settings");
    M_BOX
    (
      GP_CENTER, 
      GP.OTA_FIRMWARE();
      GP.OTA_FILESYSTEM();
      GP.BLOCK_END();
    );
    GP.HR();
    GP.HR();
    
  }

  GP.UI_END(); 
  GP.BUILD_END();
}

void action()
{
  if (ui.click("btn"))
  {
    if(ButtonState == true)
    {
      ButtonState = false;
      OnBoardLed.SetOff();
      digitalWrite(Load, OFF);
    }
    else
    {
      ButtonState = true;
      OnBoardLed.SetOn();
      digitalWrite(Load, ON);
    }
    LoadState = ButtonState;
    //client.publish(relay_topic, String(LoadState).c_str(), retain_flag);
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
    //ESP.restart();
    p.stop();
  }
}

bool WifiConnect()
{
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
    if(cnt > 50) break;
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    OnBoardLed.SetOff();
    Serial.println();
    Serial.print("Connected! Local IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  return false;
  
}

void WifiMeneger()
{
  if(WifiConnect() == false)
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

    WifiMeneger();

  }
  else
  {
    ui.attachBuild(build);
    ui.start();
    ui.attach(action);
    ui.enableOTA();   // без пароля

    if (!LittleFS.begin()) Serial.println("FS Error");
    //ui.downloadAuto(true);
  }

}
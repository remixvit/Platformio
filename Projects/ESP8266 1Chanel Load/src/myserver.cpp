#include <project.h>
#include "time.h"
#define TERMOSTAT_DEFAULT_TEMP 25
#define TERMOSTAT_MIN_TEMP     20
#define TERMOSTAT_MAX_TEMP     40
#define TERMOSTAT_STEP_TEMP    1

LoginPass lp;
GyverPortal ui(&LittleFS);

#define PLOT_SIZE 100
int16_t arr[2][PLOT_SIZE];
uint32_t dates[PLOT_SIZE];
const char *names[] = {"Temp", "Humi"};


void build() 
{
  GP.BUILD_BEGIN(500);
  GP.THEME(GP_DARK);
  GP.UPDATE("HDC_Temp, HDC_Humi, LedHeat");
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
    GP.BLOCK_TAB_BEGIN("Свет");
    M_BOX(GP_RIGHT, GP.SWITCH("Light", ButtonState); GP.LED("LedHeat"););
    GP.BLOCK_END();
    GP.HR();

    GP.HR();
    GP.BLOCK_TAB_BEGIN("Мониторинг");
    M_BOX(GP_JUSTIFY, GP.LABEL("Температура"); GP.LABEL("", "HDC_Temp"););
    M_BOX(GP_JUSTIFY, GP.LABEL("Влажность");   GP.LABEL("", "HDC_Humi"););
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
    GP.BLOCK_END();

    GP.HR();
    GP.HR();
    GP.BLOCK_TAB_BEGIN("System info");
    M_BOX(GP_CENTER, GP.TITLE(DeviceName););
    M_BOX(GP_JUSTIFY,  GP.SYSTEM_INFO("1.0"););
    GP.BLOCK_END();
    GP.HR();
    GP.HR();

    GP.BLOCK_TAB_BEGIN("System log");
    GP.AREA_LOG(10);
    GP.BLOCK_END();
    GP.HR();
    GP.HR();
  }
  else if(ui.uri("/update"))
  {
    GP.HR();
    GP.HR();
    GP.BLOCK_TAB_BEGIN("OTA");
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
  if (ui.clickBool("Light", ButtonState))
  {
    if(ButtonState)
    {
      ui.log.println("Load Switch ON");
    }
    else
    {
      ui.log.println("Load Switch OFF");
    }
  }

  if (ui.form("/wifi")) 
  {      // кнопка нажата
    ui.copyStr("lg", lp.ssid);  // копируем себе
    ui.copyStr("ps", lp.pass);
    EEPROM.put(0, lp);              // сохраняем
    EEPROM.commit(); 
    Serial.println(("SSID: '" + String(lp.ssid) + "'"));   
    Serial.println(("Pass: '" + String(lp.pass) + "'"));
    delay(1000);             
    // записываем
    ESP.restart();
  }


  if (ui.update())
  {
    if (ui.update("LedHeat")) ui.answer(LoadState);
    String Output = String(HDC_Temp, 2) + "°C";
    ui.updateString("HDC_Temp", Output);
    Output = String(HDC_Humi) + "%";
    ui.updateString("HDC_Humi", Output);
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
  if (p.form("/login")) 
  {      // кнопка нажата
    p.copyStr("lg", lp.ssid);  // копируем себе
    p.copyStr("ps", lp.pass);
    EEPROM.put(0, lp);              // сохраняем
    EEPROM.commit();                // записываем
    Serial.println(("SSID: '" + String(lp.ssid) + "'"));   
    Serial.println(("Pass: '" + String(lp.pass) + "'"));
    delay(1000);  
    WiFi.softAPdisconnect();        // отключаем AP
    p.stop();
  }
}

bool WifiConnect()
{
  EEPROM.get(0, lp);

  Serial.print("Connect to: ");
  Serial.println(("SSID: '" + String(lp.ssid) + "'"));   
  Serial.println(("Pass: '" + String(lp.pass) + "'"));
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
    Serial.println("");
    Serial.println("Cant Connect to WiFi");
    Serial.println("Portal start");

    // запускаем точку доступа
    WiFi.mode(WIFI_AP);
    WiFi.softAP("WiFi Config");

    memset(lp.ssid, 0, sizeof(lp.ssid));
    memset(lp.pass, 0, sizeof(lp.pass));

    // запускаем портал
    GyverPortal wifiui;
    wifiui.attachBuild(buildWiFi);
    wifiui.attach(actionWiFi);
    wifiui.start();

    Serial.println("Open WiFi Settings on PC ore mobile devise and connect to WiFiConwig AP");


    // работа портала
    while (wifiui.tick());

    Serial.println("WiFi Settings Saved");
    Serial.println("Device try to connect to new AP");

    WifiMeneger();

  }
  else
  {
    ui.attachBuild(build);
    ui.attach(action);
    ui.start();
    ui.enableOTA();   // без пароля
    ui.log.start(30);
    if (!LittleFS.begin()) Serial.println("FS Error");
    configTime(3 * 60ul * 60, 0, "pool.ntp.org");
  }

}

void AddToPlot()
{
  time_t rawtime;
  time(&rawtime);
  if (rawtime < 1600000000) return;
  GPaddInt(SystemTemp, arr[0], PLOT_SIZE);
  GPaddInt(HDC_Humi, arr[1], PLOT_SIZE);
  GPaddUnix(rawtime, dates, PLOT_SIZE);
}
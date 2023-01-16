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
  GP.UPDATE("WeightLable");
  GP.PAGE_TITLE("ESP32MASS", "tit");

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
    GP.BLOCK_TAB_BEGIN("Весы");
    M_BOX(GP_JUSTIFY, GP.LABEL("Текуший вес:"); M_BOX(GP_RIGHT, GP.LABEL("0.0 Kg", "WeightLable");););
    M_BOX(GP_JUSTIFY, GP.LABEL("Колличество:"); M_BOX(GP_JUSTIFY, GP.NUMBER("Count", "", 0); GP.BUTTON("CountMember", "Запомнить"););); 
    M_BOX(GP_JUSTIFY, GP.BUTTON("TareButton", "Оттарить"););
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
    M_BOX(GP_RIGHT, GP.SYSTEM_INFO("1.0"););
    M_BOX(GP_RIGHT, GP.TITLE(DeviceName););
    GP.BLOCK_END();


    GP.HR();
    GP.HR();
    GP.BLOCK_TAB_BEGIN("Калибровка весов");
    M_BOX
    (
      M_BOX(GP_CENTER, GP.LABEL("Уберите вес с весов!", "CalibrateMessage"););
      M_BOX(GP_CENTER, GP_BUTTON("CalibrateButton", "Откалибровать"););
      M_BOX(GP_CENTER, GP.NUMBER("CalibrationMass", "", 0); GP.BUTTON("CalibrationMemButton", "Запомнить"););
    );
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
  if (ui.click("TareButton"))
  {
    OnBoardLed.Toggle();
    Mass++;
  }

  if(ui.update())
  {
    if (ui.update("WeightLable"))
    {
      String MassString = "";
      MassString += String(Mass) + " Kg";
      ui.answer(MassString);  
      Serial.print("Mass: ");
      Serial.println(MassString);
    }
    
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
    if (!LittleFS.begin()) Serial.println("FS Error");
    ui.attachBuild(build);
    ui.attach(action);
    ui.start();

    ui.enableOTA();   // без пароля

    
  }

}
#include <project.h>


LoginPass lp;
GyverPortal ui(&LittleFS);

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
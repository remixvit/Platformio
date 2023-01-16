#include <project.h>


boolean ButtonState = false;
boolean LoadState = false;
bool Termostat_Set = false;
int Termostat_Set_Temp = 25;
int TermostatHyst = 2;
int SystemTemp;

Led OnBoardLed(ESPLED);


void updateStatePins(void)
{
  if(LoadState)
  {
    digitalWrite(Load, ON);
    OnBoardLed.SetOn();
  }
  else
  {
    digitalWrite(Load, OFF);
    OnBoardLed.SetOff();
  }

}

void HardwareSetup()
{
  pinMode(Load, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  digitalWrite(Load, LOW);
  Serial.begin(115200);
  Serial.println(" ");
  EEPROM.begin(100);
  HDC1080_Init();
}

void TermostatWork()
{
  if(Termostat_Set)
  {
    if(SystemTemp < (Termostat_Set_Temp + TermostatHyst))
    {
      LoadState = true;
    }
    else
    {
      LoadState = false;
    }
  }
  else
  {
    if(ButtonState)
    {
      LoadState = true;
    }
    else
    {
      LoadState = false;
    }
  }
}

void PinLog()
{
  String Out = "Button state: " + String(ButtonState);
  Serial.println(Out);

  Out = "Heater state: " + String(LoadState);
  Serial.println(Out);

  Out = "Termostat state: " + String(Termostat_Set);
  Serial.println(Out);

  Out = "Termostat set temp: " + String(Termostat_Set_Temp) + "*C";
  Serial.println(Out);

  Out = "Termostat Hysteresis: " + String(TermostatHyst) + "*C";
  Serial.println(Out);
}
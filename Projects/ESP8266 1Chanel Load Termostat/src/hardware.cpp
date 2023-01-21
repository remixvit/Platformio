#include <project.h>


boolean ButtonState = false;
boolean LoadState = false;
bool Termostat_Set = false;
int Termostat_Set_Temp = 25;
int TermostatHystUp = 2;
int TermostatHystDown = 1;
int TermostatState = 3;
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
    if(TermostatState == TERMOSTATSTATE_COLD) 
    {
      if(SystemTemp < (Termostat_Set_Temp - TermostatHystDown)) // Heaat initient
      {
        TermostatState = TERMOSTATSTATE_HEAT;
        LoadState = true;
      }

    }

    if(TermostatState == TERMOSTATSTATE_HEAT) 
    {
      if(SystemTemp > (Termostat_Set_Temp + TermostatHystUp)) // Cooling initient
      {
        TermostatState = TERMOSTATSTATE_COLD;
        LoadState = false;
      }
      else
      {
        if(LoadState == false)
        {
          LoadState = true;
        }
      }
            
    }

    if(TermostatState == TERMOSTATSTATE_START) 
    {
      
      LoadState = false;
      TermostatState = TERMOSTATSTATE_HEAT;
            
    }
  }
  else
  {
    if(TermostatState == TERMOSTATSTATE_STOP)
    {
      LoadState = false;
      TermostatState = TERMOSTATSTATE_OFF;      
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

  Out = "Termostat Hysteresis: " + String(TermostatHystUp) + "*C";
  Serial.println(Out);
}
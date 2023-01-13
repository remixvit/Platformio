#include <project.h>

boolean ButtonState = false;
boolean LoadState = false;
bool Termostat_Set = false;
int Termostat_Set_Temp = 25;

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
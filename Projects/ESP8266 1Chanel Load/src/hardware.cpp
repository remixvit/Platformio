#include <project.h>


boolean ButtonState = false;
boolean LoadState = false;
int SystemTemp;

Led OnBoardLed(ESPLED);


void updateStatePins()
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

void LoadWork()
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

void PinLog()
{
  String Out = "Button state: " + String(ButtonState);
  Serial.println(Out);

  Out = "Load state: " + String(LoadState);
  Serial.println(Out);

}
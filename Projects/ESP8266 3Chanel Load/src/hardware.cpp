#include <project.h>

boolean ButtonState = false;
boolean LoadState = false;

Led OnBoardLed(ESPLED);

void updateStatePins(void)
{
  switch (LoadState)
  {
    case true:
      digitalWrite(Load, ON);
      OnBoardLed.SetOn();
    break;

    case false:
      digitalWrite(Load, OFF);
      OnBoardLed.SetOff();
    break;
  
  default:
    digitalWrite(Load, OFF);
    OnBoardLed.SetOff();
    break;
  }
}

void HardwareSetup()
{
  pinMode(Load, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  digitalWrite(Load, LOW);
  Serial.begin(115200);
}
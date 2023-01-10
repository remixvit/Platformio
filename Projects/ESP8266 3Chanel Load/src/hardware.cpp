#include <project.h>


boolean ButtonState = false;
boolean LoadState = false;
unsigned int Led_Status;


void updateStatePins(void)
{
    if(LoadState)
    {
      digitalWrite(Load, ON);
      analogWrite(ESPLED, ESPLED_ON);
      Led_Status = ESPLED_ON;
    }else
    {
      digitalWrite(Load, OFF);
      analogWrite(ESPLED, ESPLED_OFF);
      Led_Status = ESPLED_OFF;
    }
}
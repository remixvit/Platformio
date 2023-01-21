#include "Leds.h"
#include <Arduino.h>



    Led::Led(int Pin)
    {
        LedPin = Pin;
        pinMode(Pin, OUTPUT);
        digitalWrite(Pin, LED_OFF);
    }
    

    void Led::Toggle()
    {
      switch(Led_State)
      {
        case true:
          Led_State = false;
          digitalWrite(LedPin, LED_OFF);
          break;

        case false:
          Led_State = true;
          digitalWrite(LedPin, LED_ON);
          break;

        default:
          Led_State = false;
          digitalWrite(LedPin, LED_OFF);
          break;
      }
    }

    void Led::SetOff()
    {
        Led_State = false;
        digitalWrite(LedPin, LED_OFF);
    }

    void Led::SetOn()
    {
        Led_State = true;
        digitalWrite(LedPin, LED_ON);
    }

    void Led::SetPWM(unsigned int SetLedPWM)
    {
      if(SetLedPWM > 0)
      {
        Led_State = true;
        analogWrite(LedPin, SetLedPWM);
      }
      else
      {
        SetOff();
      }
    }


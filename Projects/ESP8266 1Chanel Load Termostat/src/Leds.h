#define LED_ON  1
#define LED_OFF 0

class Led
{
  public:
    unsigned int Led_PWM;
    
    Led(int Pin);

    void Toggle();
    void SetOff();
    void SetOn();
    void SetPWM(unsigned int SetLedPWM);
    

  private:
    bool Led_State;
    int LedPin;
};
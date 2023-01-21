

#define ESPLED 14
#define Load   13
#define Button 12
#define ON     1
#define OFF    0


extern boolean ButtonState;
extern boolean LoadState;
extern int SystemTemp;


extern Led OnBoardLed;
//extern Scheduler TimeMeneger;
//extern Task TermostatCheck;



extern void updateStatePins();
extern void HardwareSetup();
extern void LoadWork();
extern void PinLog();


#define ESPLED 14
#define Load 13
#define Button 12
#define ON 1
#define OFF 0

extern boolean ButtonState;
extern boolean LoadState;
extern unsigned int Led_Status;

extern Led OnBoardLed;


extern void updateStatePins(void);
extern void HardwareSetup();
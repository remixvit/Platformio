

#define ESPLED 13
#define Load 25
#define Button 35
#define ON 1
#define OFF 0

extern boolean ButtonState;
extern boolean LoadState;
extern bool Termostat_Set;
extern int Termostat_Set_Temp;
extern int Mass;

extern Led OnBoardLed;



extern void updateStatePins(void);
extern void HardwareSetup();
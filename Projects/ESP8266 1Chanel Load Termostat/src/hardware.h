

#define ESPLED 14
#define Load   13
#define Button 12
#define ON     1
#define OFF    0

#define TERMOSTATSTATE_HEAT  1
#define TERMOSTATSTATE_COLD  0
#define TERMOSTATSTATE_START 3
#define TERMOSTATSTATE_STOP  4
#define TERMOSTATSTATE_OFF   5

extern boolean ButtonState;
extern boolean LoadState;
extern bool Termostat_Set;
extern int Termostat_Set_Temp;
extern int SystemTemp;
extern int TermostatState;

extern Led OnBoardLed;
//extern Scheduler TimeMeneger;
//extern Task TermostatCheck;



extern void updateStatePins(void);
extern void HardwareSetup();
extern void TermostatWork();
extern void PinLog();
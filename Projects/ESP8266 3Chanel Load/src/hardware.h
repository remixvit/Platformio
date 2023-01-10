

#define ESPLED 14
#define ESPLED_ON 254
#define ESPLED_OFF 0
#define Load 13
#define Button 12
#define ON 1
#define OFF 0

extern boolean ButtonState;
extern boolean LoadState;
extern unsigned int Led_Status;

void updateStatePins(void);
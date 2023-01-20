#include <GyverPortal.h>

struct LoginPass 
{
  char ssid[20];
  char pass[20];
};

extern LoginPass lp; 
extern GyverPortal ui;
extern void build();
extern void action();
extern void buildWiFi(GyverPortal& p);
extern void actionWiFi(GyverPortal& p);
extern bool WifiConnect();
extern void WifiMeneger();
extern void AddToPlot();
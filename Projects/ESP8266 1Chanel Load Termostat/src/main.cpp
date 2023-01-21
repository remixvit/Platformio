#include <project.h>
#include <TaskScheduler.h>
Scheduler TimeMeneger;

Task TermostatCheck(3000, TASK_FOREVER, &TermostatWork, &TimeMeneger, true);
Task HDC1080Check(5000, TASK_FOREVER, &HDC_GetData, &TimeMeneger, true);
Task PinStateSet(1000, TASK_FOREVER, &updateStatePins, &TimeMeneger, true);
Task PinStateLog(5000, TASK_FOREVER, &PinLog, &TimeMeneger, true);
Task MQTTUpdateCheck(3000, TASK_FOREVER, &MQTTUpdate, &TimeMeneger, true);
Task PlotUpdate(30000, TASK_FOREVER, &AddToPlot, &TimeMeneger, true);

void setup() 
{
  HardwareSetup();
  NameInit();
  WifiMeneger();
  MQTTinit();
  TimeMeneger.startNow();
//
  //client.setServer(mqtt_server, mqtt_port);
  //client.setCallback(callback);
}

void loop() 
{
  ui.tick();
  TimeMeneger.execute();
  if(!client.connected())
  {
    reconnect();
  }
  client.loop();
}
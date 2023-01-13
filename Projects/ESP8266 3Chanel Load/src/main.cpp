#include <project.h>


void setup() 
{
  HardwareSetup();
  NameInit();
  WifiMeneger();
  //WifiMeneger();

  //client.setServer(mqtt_server, mqtt_port);
  //client.setCallback(callback);
}

void loop() 
{
  ui.tick();
  /*if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();*/
}
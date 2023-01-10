#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <SPI.h>
#include <SD.h>
#include "SoftwareSerial.h"
#include "Nextion.h"


NexText t0 = NexText(0, 1, "t1");
char Buffer[100];


// Wifi network station credentials
#define WIFI_SSID "CM-SK"
#define WIFI_PASSWORD "internet2449226"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "5427041948:AAGxZ7okP3ph714BRzas93vtgMLHywkeiKA"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;          // last time messages' scan has been done
bool Start = false;

void handleNewMessages(int numNewMessages)
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    memset(Buffer, 0, sizeof(Buffer));
    t0.getText(Buffer, sizeof(Buffer));

    String PrintStr;
    PrintStr = Buffer;

    PrintStr += text + "\r\n";
    strcpy(Buffer, PrintStr.c_str());
    t0.setText(Buffer);
    Serial.print(Buffer);

    Serial.print(from_name); Serial.print(":"); Serial.println(text);

    if (from_name == "")
      from_name = "Guest";

    if (text == "/send_test_action")
    {
      bot.sendChatAction(chat_id, "typing");
      delay(4000);
      bot.sendMessage(chat_id, "Did you see the action message?");


      //more info here - https://core.telegram.org/bots/api#sendchataction
    }

    if (text == "/start")
    {
      String welcome = from_name + ", привет пидрила " +  ".\n";
      welcome += "Рад что ты теперь снами.\n\n";
      bot.sendMessage(chat_id, welcome);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial2.begin(115200, SERIAL_8N1, 25, 26);
  
  nexInit();

  t0.getText(Buffer, sizeof(Buffer));
  memset(Buffer, 0, sizeof(Buffer));
  t0.setText(Buffer);

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
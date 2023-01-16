

extern const char* mqtt_server;
extern const int mqtt_port;
extern const char* mqtt_user;
extern const char* mqtt_password;
extern const bool retain_flag;

// RELAY
extern String DeviceName;

extern WiFiClient espClient;
extern PubSubClient client;


extern void NameGenerator();
extern void callback(char* topic, byte* payload, unsigned int length);
extern void reconnect();
extern void NameInit();


extern const char* mqtt_server;
extern const int mqtt_port;
extern const char* mqtt_user;
extern const char* mqtt_password;
extern const bool retain_flag;

// RELAY
extern const char* relay_topic;
extern const String Relay_Top;

extern WiFiClient espClient;
extern PubSubClient client;



extern void callback(char* topic, byte* payload, unsigned int length);
extern void reconnect();
#include <string>
#include <cstring>
#include <cstdint>
#include "mqtt.h"

using namespace std;

          static string const   CLIENT_ID      = "Client_ID";
          static string const   BROKER_ADDRESS = "localhost";
constexpr static uint16_t const MQTT_PORT      = 1883;
          static string const   MQTT_TOPIC     = "EXAMPLE_TOPIC";

int main(int argc, char *argv[])
{


  class mqtt_client *iot_client;
  int rc;

  mosqpp::lib_init();

  string host;
  if (argc > 1){
    host = string(argv[1]);
  }
  iot_client = new mqtt_client(CLIENT_ID.c_str(), host.c_str(), MQTT_PORT);

  while(1){
    rc = iot_client->loop();
    if (rc){
      iot_client->reconnect();
    }else{
      iot_client->subscribe(nullptr, MQTT_TOPIC.c_str());
    }
  }

  mosqpp::lib_cleanup();

  return 0;
}


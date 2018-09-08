#include <string>
#include <cstring>
#include <cstdint>
#include "mqtt.h"

using namespace std;

          static string const   CLIENT_ID      = "Client_ID";
          static string const   BROKER_ADDRESS = "localhost";
constexpr static uint16_t const MQTT_PORT      = 1883;
          static string const   MQTT_TOPIC     = "EXAMPLE_TOPIC";

int main(int argc, char *argv[]){
  class mqtt_client *iot_client;
  int rc;

  mosqpp::lib_init();

  string host;
  if (argc > 1){
    host = string(argv[1]);
  }else{
    host = "iot.eclipse.org";
  }

  iot_client = new mqtt_client(CLIENT_ID, host, MQTT_PORT);

  uint32_t cnt = 0;
  while(true){
    rc = iot_client->loop(50000,10);
    if(rc){
      iot_client->reconnect();
    }else{
      iot_client->subscribe(nullptr, MQTT_TOPIC.c_str());
    }
    if(1000000<cnt++) break;
  }

  delete iot_client;

  mosqpp::lib_cleanup();

  return 0;
}


#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>

#include <sys/resource.h>

#include "mqtt.h"

using namespace std;

          static string const   TEST_BROKER_01 = "test.mosquitto.org";
          static string const   TEST_BROKER_02 = "iot.eclipse.org";

          static bool const     CLIENT_is_PUBLISHER  = true;
          static bool const     CLIENT_is_SUBSCRIBER = true;

          static string const   CLIENT_ID      = "Client_ID";
          static string const   BROKER_ADDRESS = "localhost";
constexpr static uint16_t const MQTT_PORT            = 1883;
constexpr static uint16_t const MQTT_SSL_PORT        = 8883;
constexpr static uint16_t const MQTT_WEBSOCKS_PORT   = 80;
constexpr static uint16_t const MQTT_SECUREWEBS_PORT = 443;
          static string const   MQTT_TOPIC     = "EXAMPLE_TOPIC";

void ressource_usage(){
  static struct rusage oldrinfo;
  struct rusage rinfo;
  if(0==getrusage(RUSAGE_SELF, &rinfo)){
    if(rinfo.ru_maxrss != oldrinfo.ru_maxrss){
      cout << "maximum resident set size    " << rinfo.ru_maxrss << " - " << (rinfo.ru_maxrss - oldrinfo.ru_maxrss) << "\n";
    }

    //cout << "-----------------------------------------------" << "\n";
    /*cout << "integral shared memory size  " << rinfo.ru_ixrss << "\n";
    cout << "integral unshared data size  " << rinfo.ru_idrss << "\n";
    cout << "integral unshared stack size " << rinfo.ru_isrss << "\n";

    cout << "messages sent                " << rinfo.ru_msgsnd << "\n";
    cout << "messages received            " << rinfo.ru_msgrcv << "\n";
    cout << "-----------------------------------------------" << "\n";*/

    oldrinfo = rinfo;
  }
  return;
}

int main(int argc, char *argv[]){
  class mqtt_client *iot_client;

  {
    int version[3];
    mosqpp::lib_version(&version[0],&version[1],&version[2]);
    cout << "libmosquitto Version: " << version[0] << "." << version[1] << "." << version[2] << "\n";
  }

  mosqpp::lib_init();
  string host;
  if (argc > 1){
    host = string(argv[1]);
  }else{
    host = TEST_BROKER_01;
  }
  {
    iot_client = new mqtt_client(CLIENT_ID, host, MQTT_PORT);
    int rc = 0;
    if(CLIENT_is_SUBSCRIBER){
      rc = iot_client->subscribe(nullptr, MQTT_TOPIC.c_str());
      iot_client->set_last_err(rc);
    }

    if(!iot_client->is_last_err()){
      uint32_t cnt = 0;
      constexpr uint32_t const maxcnt = 1000000;
      while(true){
        rc = iot_client->loop(50000,10);
        iot_client->set_last_err(rc);
        if(iot_client->is_last_err()){
          cout << "Loop Error: "<< iot_client->error_to_string() << "\n";

          rc = iot_client->reconnect();
          iot_client->set_last_err(rc);
          if(iot_client->is_last_err()){
            cout << "Reconnect Failed: " << iot_client->error_to_string() << "\n";
          }
        }
        if(CLIENT_is_PUBLISHER){
          if(!iot_client->is_last_err()){
            string payload1 = "STATUS";
            rc = iot_client->publish(nullptr,
                                     MQTT_TOPIC,
                                     static_cast<int>(payload1.size()+1),
                                     reinterpret_cast<const void *>(payload1.data()));
            iot_client->set_last_err(rc);
          }else{
            cout << "not Published 1: " << iot_client->error_to_string() << "\n";
          }
          if(!iot_client->is_last_err()){
            string payload2 = "ON";
            rc = iot_client->publish(nullptr,
                                     MQTT_TOPIC,
                                     static_cast<int>(payload2.size()+1),
                                     reinterpret_cast<const void *>(payload2.data()));
            iot_client->set_last_err(rc);
          }else{
            cout << "not Published 2: " << iot_client->error_to_string() << "\n";
          }
          if(!iot_client->is_last_err()){
            string payload3 = "OFF";
            rc = iot_client->publish(nullptr,
                                     MQTT_TOPIC,
                                     static_cast<int>(payload3.size()+1),
                                     reinterpret_cast<const void *>(payload3.data()));
            iot_client->set_last_err(rc);
          }else{
            cout << "not Published 3: " << iot_client->error_to_string() << "\n";
          }
        }
        if(maxcnt<cnt++) break;
        if((cnt%1000)==0) ressource_usage();
      }
    }
    delete iot_client;
  }
  mosqpp::lib_cleanup();

  return 0;
}


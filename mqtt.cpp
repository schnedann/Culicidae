#include <cstring>
#include <cstdio>
#include <iostream>


#include "mqtt.h"

using namespace std;

string const mqtt_client::mqtt_client::PUBLISH_TOPIC = "EXAMPLE_TOPIC";

mqtt_client::mqtt_client(const string id, const string host, uint16_t port) : mosquittopp(id.c_str()){
  uint16_t keepalive = DEFAULT_KEEP_ALIVE;
  connect(host.c_str(), static_cast<int>(port), static_cast<int>(keepalive));
  return;
}

mqtt_client::~mqtt_client(){
}

void mqtt_client::on_connect(int rc){
  if (!rc){
    if(1){
      cout << "Connected - code " << rc << "\n";
    }
  }
  return;
}

void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos){
  if(1){
    cout << "Subscription succeeded." << "\n";
  }
  return;
}

void mqtt_client::on_message(const struct mosquitto_message *message)
{
  string buff;
  buff.reserve(MAX_PAYLOAD);

  if(0==PUBLISH_TOPIC.compare(message->topic)){

    buff.copy(reinterpret_cast<char*>(message->payload),size_t(message->payloadlen),0);

    if(1){
      cout << buff.c_str() << "\n";
    }

    // Examples of messages for M2M communications...
    if (!strcmp(buff.c_str(), "STATUS")){
      buff = "This is a Status Message...";
      publish(nullptr, PUBLISH_TOPIC.c_str(), static_cast<int>(buff.size()), buff.c_str());
      if(1){
        cout << "Status Request Recieved." << "\n";
      }
    }

    if (!strcmp(buff.c_str(), "ON")){
      buff = "Turning on...";
      publish(nullptr, PUBLISH_TOPIC.c_str(), static_cast<int>(buff.size()), buff.c_str());
      if(1){
        cout << "Request to turn on." << "\n";
      }
    }

    if (!strcmp(buff.c_str(), "OFF")){
      buff = "Turning off...";
      publish(nullptr, PUBLISH_TOPIC.c_str(), static_cast<int>(buff.size()), buff.c_str());
      if(1){
        cout << "Request to turn off." << std:: endl;
      }
    }
  }
  return;
}

#include <cstring>
#include <cstdio>
#include <iostream>


#include "mqtt.h"

using namespace std;

string const mqtt_client::mqtt_client::PUBLISH_TOPIC = "EXAMPLE_TOPIC";

/**
 * @brief mqtt_client::mqtt_client
 * @param id
 * @param host
 * @param port
 */
mqtt_client::mqtt_client(string const& id, string const& host, uint16_t port) : mosquittopp(id.c_str()){
  int keepalive = static_cast<int>(DEFAULT_KEEP_ALIVE);
  connect(host, port, keepalive);
  return;
}

/**
 * @brief mqtt_client::on_connect
 * @param rc
 */
void mqtt_client::on_connect(int rc){
  if (!rc){
    if(true){
      cout << "Connected - code " << rc << "\n";
    }
  }
  return;
}

/**
 * @brief mqtt_client::on_subscribe
 * @param mid
 * @param qos_count
 * @param granted_qos
 */
void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos){
  if(true){
    cout << "Subscription succeeded." << "\n";
  }
  return;
}

/**
 * @brief mqtt_client::on_message
 * @param message
 */
void mqtt_client::on_message(const mosquitto_message &message){
  string buff;
  buff.reserve(MAX_PAYLOAD);

  if(0==PUBLISH_TOPIC.compare(message.topic)){

    buff.copy(reinterpret_cast<char*>(message.payload),size_t(message.payloadlen),0);

    if(true){
      cout << buff.c_str() << "\n";
    }

    // Examples of messages for M2M communications...
    if (!strcmp(buff.c_str(), "STATUS")){
      buff = "This is a Status Message...";
      publish(nullptr, PUBLISH_TOPIC.c_str(), static_cast<int>(buff.size()), buff.c_str());
      if(true){
        cout << "Status Request Recieved." << "\n";
      }
    }

    if (!strcmp(buff.c_str(), "ON")){
      buff = "Turning on...";
      publish(nullptr, PUBLISH_TOPIC.c_str(), static_cast<int>(buff.size()), buff.c_str());
      if(true){
        cout << "Request to turn on." << "\n";
      }
    }

    if (!strcmp(buff.c_str(), "OFF")){
      buff = "Turning off...";
      publish(nullptr, PUBLISH_TOPIC.c_str(), static_cast<int>(buff.size()), buff.c_str());
      if(true){
        cout << "Request to turn off." << std:: endl;
      }
    }
  }
  return;
}

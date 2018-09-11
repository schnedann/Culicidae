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
mqtt_client::mqtt_client(string const& id, string const& host, uint16_t port) : mosquittopp(id.c_str()),last_err(mqtt_errors::SUCCESS),connected(false){
  if(mqtt_errors::SUCCESS == static_cast<mqtt_errors>(connect(host, port, static_cast<int>(DEFAULT_KEEP_ALIVE)))){
    connected = true;
  }
  return;
}

//-----

void mqtt_client::set_last_err(int rc){
  last_err = static_cast<mqtt_errors>(rc);
}

mqtt_errors mqtt_client::get_last_err(){
  return last_err;
}

bool mqtt_client::is_last_err(){
  return (mqtt_errors::SUCCESS != last_err);
}

/**
 * @brief mqtt_client::error_to_string
 * @return
 */
string mqtt_client::error_to_string(){
  return mosqpp::strerror(static_cast<int>(last_err));
}

//-----

/**
 * @brief mqtt_client::on_connect
 * @param rc
 */
void mqtt_client::on_connect(int rc){
  last_err = static_cast<mqtt_errors>(rc);
  if(true){
    cout << "Connected - code " << rc << " - " << mosqpp::connack_string(rc) << "\n";
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
    cout << " -> Message ID        : " << mid << "\n";
    cout << " -> Quality of Service of subscriptions - " << qos_count << "\n";
    for(size_t ii=0; ii<static_cast<size_t>(qos_count); ++ii){
      cout << "   -> [" << ii << "]: " << granted_qos[ii] << "\n";
    }
  }
  return;
}

/**
 * @brief mqtt_client::on_message
 * @param message
 */
void mqtt_client::on_message(struct mosquitto_message const* message){
  string buff;
  //buff.reserve(MAX_PAYLOAD);

  if(0==PUBLISH_TOPIC.compare(message->topic)){

    char const* cptr = reinterpret_cast<char const*>(message->payload);
    size_t const count = size_t(message->payloadlen);
    buff.insert(0,cptr,count);

    if(true){
      cout << "[" << message->payloadlen << "] -" << buff.c_str() << "\n";
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

void mqtt_client::on_publish(int mid){
  if(true){
    cout << "Publishing succeeded." << "\n";
    cout << " -> Message ID: " << mid << "\n";
  }
  return;
}

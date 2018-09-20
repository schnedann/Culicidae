#include <cstring>
#include <cstdio>
#include <iostream>


#include "mqtt.h"

using namespace std;

string const mqtt::client::PUBLISH_TOPIC = "EXAMPLE_TOPIC";

/**
 * @brief mqtt::client
 * @param id
 * @param host
 * @param port
 */
mqtt::client::client(string const& id, string const& host, uint16_t port) : mosquittopp(id.c_str()),last_err(mqtt::errors::UNKNOWN),connected(false){
  if(mqtt::errors::SUCCESS == static_cast<mqtt::errors>(connect(host, port, static_cast<int>(DEFAULT_KEEP_ALIVE)))){
    connected = true;
  }
  return;
}

//-----

/**
 * @brief mqtt::client::do_subscribe
 * @param topic
 */
void mqtt::client::do_subscribe(string const& topic){
  int rc = subscribe(nullptr, topic);;
  last_err = static_cast<mqtt::errors>(rc);
  return;
}

/**
 * @brief mqtt::client::do_publish
 * @param topic
 * @param payload
 */
void mqtt::client::do_publish(string const& topic, string const& payload){
  int rc = publish(nullptr,topic,
                   static_cast<int>(payload.size()+1),
                   reinterpret_cast<const void *>(payload.data()));
  last_err = static_cast<mqtt::errors>(rc);
  return;
}

/**
 * @brief mqtt::client::do_reconnect
 */
void mqtt::client::do_reconnect(){
  int rc = reconnect();
  last_err = static_cast<mqtt::errors>(rc);
  return;
}

/**
 * @brief mqtt::client::do_loop
 * @param milliseconds
 */
void mqtt::client::do_loop(uint32_t milliseconds){
  int rc = loop(int(milliseconds),1);
  last_err = static_cast<mqtt::errors>(rc);
  return;
}

//-----

/**
 * @brief mqtt::client::set_last_err
 * @param rc
 */
void mqtt::client::set_last_err(int rc){
  last_err = static_cast<mqtt::errors>(rc);
}

/**
 * @brief mqtt::client::get_last_err
 * @return
 */
mqtt::errors mqtt::client::get_last_err(){
  return last_err;
}

/**
 * @brief mqtt::client::is_last_err
 * @return
 */
bool mqtt::client::is_last_err(){
  return (mqtt::errors::SUCCESS != last_err);
}

/**
 * @brief mqtt::client::error_to_string
 * @return
 */
string mqtt::client::error_to_string(){
  return mosqpp::strerror(static_cast<int>(last_err));
}

//-----



//-----

/**
 * @brief mqtt::client::on_connect
 * @param rc
 */
void mqtt::client::on_connect(int rc){
  last_err = static_cast<mqtt::errors>(rc);
  if(true){
    cout << "Connected - code " << rc << " - " << mosqpp::connack_string(rc) << "\n";
  }
  return;
}

/**
 * @brief mqtt::client::on_connect_with_flags
 * @param rc
 * @param flags
 */
void mqtt::client::on_connect_with_flags(int rc, int flags){
  last_err = static_cast<mqtt::errors>(rc);
  cout << strerror(rc) << "\n";
  cout << flags << "\n";
  return;
}

/**
 * @brief mqtt::client::on_disconnect
 * @param rc
 */
void mqtt::client::on_disconnect(int rc){
  last_err = static_cast<mqtt::errors>(rc);
  return;
}

/**
 * @brief mqtt::client::on_publish
 * @param mid
 */
void mqtt::client::on_publish(int mid){
  if(true){
    cout << "Publishing succeeded." << "\n";
    cout << " -> Message ID: " << mid << "\n";
  }
  return;
}

/**
 * @brief mqtt::client::on_message
 * @param message
 */
void mqtt::client::on_message(struct mosquitto_message const* message){
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

/**
 * @brief mqtt::client::on_subscribe
 * @param mid
 * @param qos_count
 * @param granted_qos
 */
void mqtt::client::on_subscribe(int mid, int qos_count, const int *granted_qos){
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
 * @brief mqtt::client::on_unsubscribe
 * @param mid
 */
void mqtt::client::on_unsubscribe(int mid){
  cout << "Unsubscribe" << "\n";
  cout << " -> Message ID: " << mid << "\n";
  return;
}

/**
 * @brief mqtt::client::on_log
 * @param level
 * @param str
 */
void mqtt::client::on_log(int level, std::string const& str){
  if(false){
    cout << "Log[" << level << "]: " << str << "\n";
  }
  return;
}

/**
 * @brief mqtt::client::on_error
 */
void mqtt::client::on_error(){
  last_err = mqtt::errors::UNKNOWN;
  cout << "!!!Error!!!" << "\n";
  return;
}

#ifndef SIMPLECLIENT_MQTT_H
#define SIMPLECLIENT_MQTT_H

#include <string>
#include <cstdint>

#include <mosquittopp.h>

class mqtt_client : public mosqpp::mosquittopp
{
public:

  constexpr static uint16_t const MAX_PAYLOAD = 50;
  constexpr static uint16_t const DEFAULT_KEEP_ALIVE = 60;
  static std::string const PUBLISH_TOPIC;

    mqtt_client (const std::string &id, const std::string &host, uint16_t port);
    ~mqtt_client()=default;

    void on_connect(int rc);
    void on_message(struct mosquitto_message const& message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
};

#endif //SIMPLECLIENT_MQTT_H

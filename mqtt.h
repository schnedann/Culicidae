#ifndef SIMPLECLIENT_MQTT_H
#define SIMPLECLIENT_MQTT_H

#include <string>
#include <cstdint>
#include <cstdint>

#include <mosquittopp.h>

enum class mqtt_errors:int{
  CONN_PENDING   = MOSQ_ERR_CONN_PENDING   ,
  SUCCESS        = MOSQ_ERR_SUCCESS        ,
  NOMEM          = MOSQ_ERR_NOMEM          ,
  PROTOCOL       = MOSQ_ERR_PROTOCOL       ,
  INVAL          = MOSQ_ERR_INVAL          ,
  NO_CONN        = MOSQ_ERR_NO_CONN        ,
  CONN_REFUSED   = MOSQ_ERR_CONN_REFUSED   ,
  NOT_FOUND      = MOSQ_ERR_NOT_FOUND      ,
  CONN_LOST      = MOSQ_ERR_CONN_LOST      ,
  TLS            = MOSQ_ERR_TLS            ,
  PAYLOAD_SIZE   = MOSQ_ERR_PAYLOAD_SIZE   ,
  NOT_SUPPORTED  = MOSQ_ERR_NOT_SUPPORTED  ,
  AUTH           = MOSQ_ERR_AUTH           ,
  ACL_DENIED     = MOSQ_ERR_ACL_DENIED     ,
  UNKNOWN        = MOSQ_ERR_UNKNOWN        ,
  ERRNO          = MOSQ_ERR_ERRNO          ,
  EAI            = MOSQ_ERR_EAI            ,
  PROXY          = MOSQ_ERR_PROXY          ,
  PLUGIN_DEFER   = MOSQ_ERR_PLUGIN_DEFER   ,
  MALFORMED_UTF8 = MOSQ_ERR_MALFORMED_UTF8 ,
  KEEPALIVE      = MOSQ_ERR_KEEPALIVE      ,
  LOOKUP         = MOSQ_ERR_LOOKUP
};

class mqtt_client : public mosqpp::mosquittopp
{
private:
  mqtt_errors last_err;
  bool connected;
public:

  constexpr static uint16_t const MAX_PAYLOAD = 50;
  constexpr static uint16_t const DEFAULT_KEEP_ALIVE = 60;
  static std::string const PUBLISH_TOPIC;

  mqtt_client (const std::string &id, const std::string &host, uint16_t port);
  ~mqtt_client()=default;

  std::string error_to_string();
  void set_last_err(int rc);
  mqtt_errors get_last_err();
  bool is_last_err();

  void on_connect(int rc);
  void on_subscribe(int mid, int qos_count, const int *granted_qos);
  void on_message(struct mosquitto_message const* message);
  void on_publish(int mid);

};

#endif //SIMPLECLIENT_MQTT_H

#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>
#include <ctime>
#include <array>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sched.h>

#include "mqtt.h"

using namespace std;

constexpr static bool   const USE_ASYNCAPI  = false;
constexpr static bool   const USE_RTCYCLE   = false;
constexpr static uint32_t const MAX_CYCLES  = 150;


constexpr static long   const NSEC_PER_SEC   = 1000000000;
//                                            ssmmmuuunnn
constexpr static size_t const INTERVAL       =   50000000; //1000ms = 1.0s
constexpr static size_t const MAX_SAFE_STACK = 8*1024;
//-----
          static string const   TEST_BROKER_00 = "127.0.0.1";
          static string const   TEST_BROKER_01 = "test.mosquitto.org";
          static string const   TEST_BROKER_02 = "iot.eclipse.org";

          static bool const     CLIENT_is_PUBLISHER  = true;
          static bool const     CLIENT_is_SUBSCRIBER = true;

          static string const   CLIENT_ID      = "Client_ID";
          static string const   BROKER_ADDRESS = "localhost";
constexpr static uint16_t const MQTT_PORT            = 1883;
//constexpr static uint16_t const MQTT_SSL_PORT        = 8883;
//constexpr static uint16_t const MQTT_WEBSOCKS_PORT   = 80;
//constexpr static uint16_t const MQTT_SECUREWEBS_PORT = 443;
          static string const   MQTT_TOPIC     = "EXAMPLE_TOPIC";
//-----

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

/**
 * Not understanding why this piece of code
 * should reserve stack... as dummy only lives
 * in the scope of stack_prefault()
 *
 * --> assume Code has no effect at last
 */
void stack_prefault(){
  array<char,MAX_SAFE_STACK> dummy;
  std::memset(dummy.data(), 0, dummy.size());
  return;
}

/**
 * correct time-struct on nanosecond overflow
 */
static void tsnorm(struct timespec& ts){
  while (ts.tv_nsec >= NSEC_PER_SEC) {
    ts.tv_nsec -= NSEC_PER_SEC;
    ++ts.tv_sec;
  }
  return;
}

int main(int argc, char *argv[]){
  int err = 0;
  uint32_t errcnt = 0;
  string host;

  /*
   * Prepare a defined Cycle Time
   */
  struct timespec time;
  struct sched_param param;

  time.tv_sec  = INTERVAL/NSEC_PER_SEC;
  time.tv_nsec = INTERVAL%NSEC_PER_SEC;

  if(USE_RTCYCLE){
    /* Declare ourself as a "real time" task */
    { //set to 90% of max Priority
      int const pmin = sched_get_priority_min(SCHED_FIFO);
      int const pmax = sched_get_priority_max(SCHED_FIFO);
      int const pdiff = ((pmax-pmin)*900000)/1000000;
      param.sched_priority = pmin+pdiff;
    }
    if(sched_setscheduler(0, SCHED_FIFO, &param) == -1){
      err=-1;
      goto lERR;
    }
    /* Lock memory */
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1){
      err=-2;
      goto lERR;
    }
    stack_prefault();                   //Stack reservieren
    clock_gettime(CLOCK_MONOTONIC, &time); //Guess CLOCK_PROCESS_CPUTIME_ID should work alike...
  }

  {
    int version[3];
    mosqpp::lib_version(&version[0],&version[1],&version[2]);
    cout << "libmosquitto Version: " << version[0] << "." << version[1] << "." << version[2] << "\n";
  }

  mosqpp::lib_init();

  if (argc > 1){
    host = string(argv[1]);
  }else{
    host = TEST_BROKER_00;
  }
  {
    mqtt::client iot_client = mqtt::client(CLIENT_ID, host, MQTT_PORT, USE_ASYNCAPI);
    if(CLIENT_is_SUBSCRIBER){
      iot_client.do_subscribe(MQTT_TOPIC);
      if(iot_client.is_last_err()){
        cout << errcnt++ << " - Subscribe Error: "<< iot_client.error_to_string() << "\n";
      }
    }

    if(!iot_client.is_last_err()){
      uint32_t cnt = 0;
      while(true){
        { //Do Work
          //rc = iot_client.loop(25,1);
          iot_client.do_loop(0);
          if(iot_client.is_last_err()){
            cout << errcnt++ << " - Loop Error: "<< iot_client.error_to_string() << "\n";

            iot_client.do_reconnect();
            if(iot_client.is_last_err()){
              cout << errcnt++ << " - Reconnect Failed: " << iot_client.error_to_string() << "\n";
            }
          }

          if(CLIENT_is_PUBLISHER){
            if((cnt%5)==0){
              if(!iot_client.is_last_err()){
                string payload1 = "STATUS";
                iot_client.do_publish(MQTT_TOPIC,payload1);
              }else{
                cout << errcnt++ << " - not Published 1: " << iot_client.error_to_string() << "\n";
              }
              if(!iot_client.is_last_err()){
                string payload2 = "ON";
                iot_client.do_publish(MQTT_TOPIC,payload2);
              }else{
                cout << errcnt++ << " - not Published 2: " << iot_client.error_to_string() << "\n";
              }
              if(!iot_client.is_last_err()){
                string payload3 = "OFF";
                iot_client.do_publish(MQTT_TOPIC,payload3);
              }else{
                cout << errcnt++ << " - not Published 3: " << iot_client.error_to_string() << "\n";
              }
            }
          }
        } //work

        { //Maximum Cycles
          if(MAX_CYCLES<cnt++) break;
          if((cnt%1000)==0) ressource_usage();
        }

        { //Cycle Control
          if(USE_RTCYCLE){
            //Cycle Time
            time.tv_nsec += INTERVAL; //Set next Wakeup Time
            tsnorm(time);             //Overflow handling
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &time, nullptr);
          }else{
            nanosleep(&time,nullptr);
          }
        }
      } //while
    }

    cout << "Error-Count: " << errcnt << "\n";
  }

lERR:
  if(err) cout << "Err: " << err << "\n";

  mosqpp::lib_cleanup();
  return 0;
}



#ifndef defines_h
#define defines_h

#include <Arduino.h>
#if defined(ESP32)
    #include <WiFi.h>   // not required it seems for esp8266
#endif

//#include "AsyncMqtt_Generic.h"

//#include <TZ.h>

//#define TZ_America_Chicago	PSTR("CST6CDT,M3.2.0,M11.1.0")
//#define TZ_Europe_London	PSTR("GMT0BST,M3.5.0/1,M10.5.0")
//#define TZ_America_New_York	PSTR("EST5EDT,M3.2.0,M11.1.0")

#define _ASYNC_MQTT_LOGLEVEL_ 1

#define MAX_CFGSTR_LENGTH 51
#define MAX_LOGSTRING_LENGTH 501

#define TELNET_PORT 23

//#define MY_TZ TZ_Europe_London
//#define MY_NTP_SERVER "at.pool.ntp.org"

#define p_mqttBrokerIP_Label "mqttBrokerIP"
#define p_mqttBrokerPort_Label "mqttBrokerPort"

// MQTT Topic Names
#define oh3CommandIOT "/house/service/iot-command"              // e.g. IOT-IDENTITY, IOT-RESET, IOT-RESTART, IOT-SWITCH-CONFIG
#define oh3StateLog "/house/service/log"                        // Log messages
#define oh3CommandTOD "/house/service/time-of-day"              // Time of day broadcast from OpenHab
#define oh3StateIOTRefresh "/house/service/iot-device-refresh"  // Request Refresh of Contol times ( not needed by all apps)
#define StateCntrlModeWD "/house/cntrl/%s/wd-control-operating-mode"	    // %s will be the controller name to make the topic unique. Mode values will be AUTOMODE, OFFMODE, ONMODE ...
#define StateCntrlModeWE "/house/cntrl/%s/we-control-operating-mode"	    // As Above

// FIXTHIS Got to be a smarter way
#define REPORT_INFO   1
#define REPORT_WARN   2
#define REPORT_ERROR  4
#define REPORT_DEBUG  8

//***************************************************************
// Controller Specific MQTT Topics and config
//***************************************************************
#define AUTOMODE    0 // Normal running mode - Heating times are based on the 3 time zones
#define NEXTMODE    1 // Advances the control time to the next zone. FIX-THIS: Crap description
#define ONMODE      2   // Permanently ON.  Heat is permanently requested. Zones times are ignored
#define OFFMODE     3  // Permanently OFF.  Heat is never requested. Zones times are ignored
#define UNKNOWNMODE 4 // set at start up 

#define SBUNKOWN 0
#define SBON 1
#define SBOFF 2

#define ZONE1 0
#define ZONE2 1
#define ZONE3 2

#define ZONEGAP 9

class devConfig
{
  String devName;
  String devType;
  bool configWifiOnboot;

public:
  devConfig() {}
  devConfig(String name, String type)
  {
    devName = name;
    devType = type;
    configWifiOnboot = false;
  }
  void setup(String name, String type)
  {
    devName = name;
    devType = type;
  }
  String getName()
  {
    return devName;
  }
  String getType()
  {
    return devType;
  }
  void setwifiConfigOnboot(bool bValue)
  {
    configWifiOnboot = bValue;
  }
  bool getwifiConfigOnboot(bool bValue)
  {
    return configWifiOnboot;
  }
};

class templateServices
{
  bool weekDay = false; // initialise FIXTHIS
  int dayNumber;
  int timeNow = 0;
  bool ohTODReceived = false;

public:
  templateServices() {}
  templateServices(int dn)
  {
    dayNumber = dn;
    if (dn >= 1 && dn <= 5)
      weekDay = true;
    else
      weekDay = false;
  }
  void setup(int dn)
  {
    dayNumber = dn;
  }
  void setDayNumber(int dn)
  {
    dayNumber = dn;
    if (dn >= 1 && dn <= 5)
      weekDay = true;
    else
      weekDay = false;
  }
  void setTimeNow( int tn)
  {
    timeNow = tn;
  }
  void setOHTODReceived( bool ohtod)
  {
    ohTODReceived = ohtod;
  }
  bool getWeekDayState()
  {
    return weekDay;
  }
  int getTimeNow()
  {
    return timeNow;
  }
  bool getOHTODReceived()
  {
    return ohTODReceived;
  }
};

#endif // defines_h
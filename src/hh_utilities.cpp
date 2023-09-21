#include "hh_defines.h"
#include <Arduino.h>
#include <time.h>
//#include <AsyncMqtt_Generic.h>
#include <AsyncMqttClient_Generic.hpp>

extern AsyncMqttClient mqttClient;
extern void printTelnet(String);      // defined in telnet.cpp


extern String deviceName;
extern String deviceType;
extern char ntptod[MAX_CFGSTR_LENGTH];
extern bool ntpTODReceived;
extern bool telnetReporting;

bool mqttLog(const char*, bool, bool);

byte reportLevel = REPORT_WARN + REPORT_ERROR;

// Publish a log message
// message  :  a string 
// type:    : INFO,WARN,ERROR,DEBUG
//
// Publish on MQTT and write to serial port and to Telnet depending on reporting config
// 
// TelnetReporting == true then send to Telnet port
// MQTTReporting == true then publish 
//  


bool mqttLog(const char* msg, bool mqtt, bool monitor)   // FIXTHIS
{
  //Serial.print("sensor Name: "); Serial.println(sensorName);

  if ( reportLevel & REPORT_INFO)
  {
    Serial.println("INFO REPORTING");
  }
  if ( reportLevel & REPORT_WARN)
  {
    Serial.println("WARNING REPORTING");
  }
    if ( reportLevel & REPORT_ERROR)
  {
    Serial.println("ERROR REPORTING");
  }
    if ( reportLevel & REPORT_DEBUG)
  {
    Serial.println("DEBUG REPORTING");
  }




  if (ntpTODReceived == false)
    sprintf(ntptod, "%s", "0000-00-00T00:00:00");

  char logMsg[MAX_LOGSTRING_LENGTH]; 
  memset(logMsg,0, sizeof logMsg);

  sprintf(logMsg, "%s,%s,%s,%s", ntptod, deviceType.c_str(), deviceName.c_str(), msg);    
  if (mqtt)
  {
        mqttClient.publish(oh3StateLog, 0, true, logMsg);
  }
  if (monitor)
  {
        Serial.println(logMsg);
  }
  if (telnetReporting == true)
  {
          printTelnet(logMsg);
  }

	return false;
}

int get_weekday(char * str) 
{
  struct tm tm;
  memset((void *) &tm, 0, sizeof(tm));
  if (strptime(str, "%Y-%m-%d", &tm) != NULL) 
  {
    time_t t = mktime(&tm);
    if (t >= 0) 
    {
      return localtime(&t)->tm_wday; // Sunday=0, Monday=1, etc.
    }
  }
  return -1;
}
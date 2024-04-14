#include "hh_defines.h"
#include <Arduino.h>
#include <time.h>

// read:  https://registry.platformio.org/libraries/khoih-prog/ESPAsync_WiFiManager
//#include <AsyncMqtt_Generic.h>
//////#include <AsyncMqttClient_Generic.hpp>
#include <AsyncMqttClient.h> 

extern AsyncMqttClient mqttClient;
extern void printTelnet(String);      // defined in telnet.cpp


extern String deviceName;
extern String deviceType;
extern char ntptod[MAX_CFGSTR_LENGTH];
extern bool ntpTODReceived;
extern bool telnetReporting;
extern byte reportFilter;

//bool mqttLog(const char*, bool, bool);
bool mqttLog(const char* msg, byte recordType, bool mqtt, bool monitor);


//#define N_ITEMS     (sizeof(items)/sizeof(char *))

//********************************************
// Publish a log message
// message  :  a string 
// type:    : INFO,WARN,ERROR,DEBUG
//
// Publish on MQTT and write to serial port and to Telnet depending on reporting config
// 
// TelnetReporting == true then send to Telnet port
// MQTTReporting == true then publish 
//
bool mqttLog(const char* msg, byte recordType, bool mqtt, bool monitor)   // FIXTHIS
{
  //Serial.print("sensor Name: "); Serial.println(sensorName);
  String logRecordType = "UNKNWN";
  
  if ( recordType & REPORT_INFO)
  {
    logRecordType = "INFO";
  }
  else if ( recordType & REPORT_WARN)
  {
    logRecordType = "WARN";
  }
  else if ( recordType & REPORT_ERROR)
  {
    logRecordType = "ERROR";
  }
  else if ( recordType & REPORT_DEBUG)
  {
    logRecordType = "DEBUG";
  }

  // Filter incomming log record (recordType) against log filter (reportFilter). 
  if ( reportFilter & recordType)
  {
    // Initialise loag date with zeros if Time is not yet known.
    if (ntpTODReceived == false)
    {
      sprintf(ntptod, "%s", "0000-00-00T00:00:00");
    }  
    // Make some space.  
    // FIXTHIS  check length or something similar
    char logMsg[MAX_LOGSTRING_LENGTH]; 
    memset(logMsg,0, sizeof logMsg);

    // Format log header and content
    sprintf(logMsg, "%s,%s,%s,%s,%s", ntptod, deviceType.c_str(), deviceName.c_str(), logRecordType.c_str(), msg);    
    if (mqtt)                         // the log is to be published to MQTT topic
    {
        mqttClient.publish(oh3StateLog, 0, true, logMsg);
    }
    if (monitor)                      // The log is be sent to Serial port
    {
        Serial.println(logMsg);
    }
    if (telnetReporting == true)      // The log is to be sent to Telnet port
    {
        printTelnet(logMsg);
    }
  }  
	return false;
}

// get day of week as an integer
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
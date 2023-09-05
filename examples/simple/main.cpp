// Useful sites:
// https://uk-air.defra.gov.uk/air-pollution/daqi?view=more-info&pollutant=pm10#pollutant

// https://wiki.dfrobot.com/SHT31_Temperature_Humidity_Sensor_Weatherproof_SKU_SEN0385



#include <ArduinoOTA.h>
#include <Ticker.h>
#include <AsyncMqttClient_Generic.hpp>
#if defined(ESP32)
    #define ESP_DRD_USE_LITTLEFS  true
    #define ESP_DRD_USE_SPIFFS    false
    #define ESP_DRD_USE_EEPROM    false
#else
  #define USE_LITTLEFS            true
  #define ESP_DRD_USE_LITTLEFS    true
#endif
#define DOUBLERESETDETECTOR_DEBUG true
#define DRD_TIMEOUT 4
#define DRD_ADDRESS 0
#include <ESP_DoubleResetDetector.h>


#include "hh_defines.h"


bool onMqttMessageAppExt(char *, char *, const AsyncMqttClientMessageProperties &, const size_t &, const size_t &, const size_t &);    // Required by template
void appMQTTTopicSubscribe();
void telnet_extension_1(char);      // Required by template
void telnet_extension_2(char);      // Required by template
void telnet_extensionHelp(char);    // Required by template
void processTOD_Ext();              // Required by template

// defined in asyncConnect.cpp
extern void platform_setup(bool);
extern void printTelnet(String);
extern void handleTelnet();
extern AsyncMqttClient mqttClient;

DoubleResetDetector *drd;
devConfig espDevice;

// Application Specific MQTT Topics and config
const char *oh3StateValue = "/house/example/exapmple-1/value";  //FIXTHIS
//const char *oh3StateValue = "/house/aqm/aqm-1/value";  //FIXTHIS
String deviceName = "ex-1";
String deviceType = "exmpl";

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{

    bool configWiFi = false;
    Serial.begin(115200);
    while (!Serial)
        delay(1000);
    
    // Set up DRD
    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
    if (drd->detectDoubleReset())
    {
        configWiFi = true;
        Serial.println("App: Double Reset detected");
    }

    // Set this deviceName and Type
    espDevice.setup(deviceName, deviceType);
    // Platform setup: Set up and manage all services: LittleFS, WiFi, MQTT and Telnet
    // The first time, there will be no WiFi confif so look foe SSID and configure device.
    platform_setup(configWiFi);
    





//************************************************************
// Process any application specific inbound MQTT messages
// Return False if none
// Return true if an MQTT message was handled here
//************************************************************
bool onMqttMessageAppExt(char *topic, char *payload, const AsyncMqttClientMessageProperties &properties, const size_t &len, const size_t &index, const size_t &total)
{
    return false;
}
//***********************************************************
// Subscribe to application specific topics
//***********************************************************
void appMQTTTopicSubscribe()
{
    // mqttTopicsubscribe(oh3StateValue, 2);
}

//********************************************************************
// Write out ove telnet session and application specific infomation
//********************************************************************
void telnet_extension_1(char c)
{
    
    char logString[MAX_LOGSTRING_LENGTH];
    memset(logString, 0, sizeof logString);
   
    sprintf(logString, "%s%s\r", "Temperature (°C):\t\t", buf);
    printTelnet((String)logString);
  
}

// Process any application specific telnet commannds
void telnet_extension_2(char c)
{
    printTelnet((String)c);
}

// Process any application specific telnet instructions
void telnet_extensionHelp(char c)
{
    printTelnet((String) "x\t\tSome description");
}

void drdDetected()
{
    Serial.println("Double resert detected");
}
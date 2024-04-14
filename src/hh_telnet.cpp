//#include <ESP8266WiFi.h>
#if !defined(ESP32)
  //#pragma message "Compiling for ESP8266"
  #include <ESP8266WiFi.h>
#endif  

#include "hh_defines.h"

// declare telnet server 
WiFiServer TelnetServer(TELNET_PORT);
WiFiClient Telnet;

void handleTelnet();
void printTelnet(String);
String reportingState();

extern String deviceName;
extern String deviceType;
extern char wifiConfigOnboot[];
extern char ntptod[];
extern void mqttDisconnect(); 
extern bool mqttGetConnectedStatus();
extern String mqttGetClientID();
extern String getFormattedTime();
extern void telnet_extension_1(char);
extern void telnet_extension_2(char);
extern void telnet_extensionHelp(char);
extern void setWiFiConfigOnBoot(String);
extern String MQTTDisconnectMessage;
extern int mqttDisconnectCounter;           // FIXTHIS Added to debug MQTT reconnect issue
extern int mqttConnectCounter;               // as above 

extern String Router_SSID;
int reporting = REPORT_INFO;
byte reportFilter = REPORT_WARN | REPORT_ERROR;
bool telnetReporting = false;

void printTelnet(String msg)
{
  Telnet.println(msg);
}

void telnetBegin()
{
   TelnetServer.begin(); 
}

void telnetStop()
{
   TelnetServer.stop(); 
}

void handleTelnet()
{
    char logString[MAX_LOGSTRING_LENGTH];

  if (TelnetServer.hasClient())
  {
  	// client is connected
    if (!Telnet || !Telnet.connected()){
      if(Telnet) Telnet.stop();          // client disconnected
      Telnet = TelnetServer.accept(); // ready for new client
    } else {
      TelnetServer.available().stop();  // have client, block new conections
    }
  }

  if (Telnet && Telnet.connected() && Telnet.available()){
    // client input processing
    while(Telnet.available())
     // Serial.write(Telnet.read()); // pass through
      // do other stuff with client input here
    //if (SERIAL.available() > 0) {
    {
        char c = Telnet.read();
        switch (c) {
          	case '\r':
        	  	Telnet.println();
            break;
          	case 'l':

        	  	int sigStrength;
        	  	sigStrength = WiFi.RSSI();
        	  	memset(logString,0, sizeof logString);
        	  	sprintf(logString,
        			  "%s%s\n\r%s%s\n\r%s%s\n\r%s%i\n\r%s%s\n\r%s%i\n\r%s%d.%d.%d.%d",
					    "Date:\t\t",getFormattedTime().c_str(),
        			    "Sensor Type:\t", deviceType.c_str(),
					    "Sensor Name:\t", deviceName.c_str(),
					    "WiFi Connect:\t", WiFi.status(),
					    "SSID:\t\t", Router_SSID.c_str(),
					    "Sig Strength:\t", sigStrength,
					    "ipAddr:\t\t", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
        	  	Telnet.println(logString);
        	  	
              if (!mqttGetConnectedStatus())
            	  	Telnet.println("MQTT Client:\tNOT CONNECTED");
        	  	else
        	  	{
        		  	sprintf(logString, "%s%s\n\r", "MQTT Client:\t",  mqttGetClientID().c_str());
        		  	Telnet.println(logString);
        	  	}
              sprintf(logString, "%s%s\n\r", "LOG Reporting:\t",  reportingState().c_str());
        		  Telnet.println(logString);
              sprintf(logString, "%s%s\r", "MQTT Disconnected Message:\t",  MQTTDisconnectMessage.c_str());
        		  Telnet.println(logString);
              sprintf(logString, "%s%i\r", "onMQTTDisconnect counter:\t",  mqttDisconnectCounter);
        		  Telnet.println(logString);
              sprintf(logString, "%s%i\n\r", "onMQTTConnect counter:\t",  mqttConnectCounter);
        		  Telnet.println(logString);
              

              // Call out to any Application specific content to append this telnet output
              telnet_extension_1(c);
            break;
           	case 'c':
                if (strcmp(wifiConfigOnboot, "NO") == 0)
                {
        	  	    Telnet.println("Set config WiFi credentials on next reboot");
                  setWiFiConfigOnBoot("YES");
                }  
                else 
                {
                  Telnet.println("Do not config WiFi credentials on next reboot - normal reboot");
                  setWiFiConfigOnBoot("NO");
                } 
            break;                        
          	case 'm':
        	  	  Telnet.println("Disconnecting from MQTT Broker ");
                mqttDisconnect(); 
            break;
          	case 'r':
        	  	  Telnet.println("you are restarting this device");
        	  	  ESP.restart();
            break;
            case 'd':
                reportFilter = reportFilter ^ REPORT_DEBUG;       //XOR
                Telnet.println(reportingState().c_str());
            break;
                case 'e':
                reportFilter = reportFilter ^ REPORT_ERROR;       //XOR
                Telnet.println(reportingState().c_str());
            break;
                case 'i':
                reportFilter = reportFilter ^ REPORT_INFO;       //XOR
                Telnet.println(reportingState().c_str());
            break;
                case 'w':
                reportFilter = reportFilter ^ REPORT_WARN;       //XOR
                Telnet.println(reportingState().c_str());
            break;
            case 't':
                if (telnetReporting == false )
                {
                    telnetReporting = true;
                    Telnet.println("Telnet message reporting ON. Check logging filters - i,w,e and d");
                } 
                else
                {
                    telnetReporting = false;
                    Telnet.println("Telnet message reporting OFF. Check logging filters - i,w,e and d");
                }
            break;
			case 'h':
        	  	Telnet.println("Help");
				      Telnet.println("l\t\tList status and IoT device information");
              Telnet.println("t\t\tToggle log messages over telnet");
              Telnet.println("t\t\t");
              Telnet.println("d\t\tToggle Debug message reporting on or off");
              Telnet.println("i\t\tToggle INFO message reporting on or off");
              Telnet.println("w\t\tToggle WARN message reporting on or off");
              Telnet.println("e\t\tToggle ERROR message reporting on or off");
              Telnet.println("t\t\t");
				      Telnet.println("m\t\tDisconnect MQTT broker. Should auto reconnect");
              Telnet.println("c\t\tSet config WiFi credentials on next reboot");
              Telnet.println("t\t\t");
				      Telnet.println("r\t\tRe-boot IoT device");

              telnet_extensionHelp(c);   // Extend menu via application user exit
            break;
          	default:
              telnet_extension_2(c); 	
            break;
        }
      }

  }
}

String reportingState()
{
  String output = "";
  if (reportFilter & REPORT_INFO)
  {
    output = output + "INFO:ON";
  }
  else
  {
    output = output + "INFO:OFF";            
  }
  if (reportFilter & REPORT_WARN)
  {
    output = output + ", WARN:ON";
  }
  else
  {
    output = output +  ", WARN:OFF";            
  }
  if (reportFilter & REPORT_ERROR)
  {
    output = output + ", ERROR:ON";
  }
  else
  {
    output = output +  ", ERROR:OFF";            
  }
  if (reportFilter & REPORT_DEBUG)
  {
    output = output + ", DEBUG:ON";
  }
  else
  {
    output = output +  ", DEBUG:OFF";            
  }
  return output;
}
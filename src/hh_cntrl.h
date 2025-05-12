

#ifndef cntrl_h
#define cntrl_h

#include "hh_defines.h"
#include <Arduino.h>
// read:  https://registry.platformio.org/libraries/khoih-prog/ESPAsync_WiFiManager
#include <AsyncMqttClient.h> 
//#include <AsyncMqttClient_Generic.hpp>


//*************************************
// defined in telnet.cpp
//*************************************
extern int reporting;
extern bool telnetReporting;
//*************************************
// defined in asyncConnect.cpp
//*************************************
extern AsyncMqttClient mqttClient;
extern templateServices coreServices;
extern void mqttTopicsubscribe(const char *topic, int qos);
extern char ntptod[MAX_CFGSTR_LENGTH];

//
// defined in telnet.cpp
//
extern void printTelnet(String);

// defined in ??
extern int ohTimenow;
extern bool ohTODReceived;
extern devConfig espDevice;

// defined in utilities.cpp
extern bool mqttLog(const char* msg, byte recordType, bool mqtt, bool monitor);

//
// Application User exits
//
extern bool processCntrlMessageApp_Ext(char *, const char *, const char *, const char *);
extern void app_WD_on(void *);
extern void app_WD_off(void *);
extern void app_WE_on(void *);
extern void app_WE_off(void *);
extern void app_WD_auto(void *);
extern void app_WE_auto(void *);

void processCntrlTimes(char *, char (&ptr)[6][10], int lptr[6]);
bool onMqttMessageCntrlExt(char *topic, char *payload, const AsyncMqttClientMessageProperties &properties, const size_t &len, const size_t &index, const size_t &total);
String runmodeText(int);

class cntrlState
{
	int outputState = 0; // Off
	int WDrunMode;
	int WDreturnToModeAfterNEXT;
	int WDswitchBack;
	int WDzone;
	int WDHoldState;
	bool WDcntrlTimesReceived = false;
	bool WDcommandReceived = false;
	//bool WDBypassMode = true;

	int WErunMode;
	int WEreturnToModeAfterNEXT;
	int WEswitchBack;
	int WEzone;
	int WEHoldState;
	bool WEcntrlTimesReceived = false;
	bool WEcommandReceived = false;
	//bool WEBypassMode = true;

	int WDlcntrlTimes[6];
	char WDcntrlTimes[6][10]{"0000", "0100", "0200", "0300", "0400", "0600"}; //  how big is each array element? 6 elements each element 10 characters long (9 + 1 for /0)
	int WElcntrlTimes[6];
	char WEcntrlTimes[6][10]{"0000", "0100", "0200", "0300", "0400", "0600"}; //  how big is each array element? 6 elements each element 10 characters long (9 + 1 for /0)

	String cntrlName;
	String refreshID;
	String WDCntrlRuntimeStateTopic;
	String WDcntrlTimesTopic;
	String WECntrlRuntimeStateTopic;
	String WEcntrlTimesTopic;
	String WDUICmdTopic;
	String WEUICmdTopic;
	String WDBypassTimesTopic;
	String WEBypassTimesTopic;

	cntrlState *cntrlObjRef;

	// Ticker configurationTimesReceived;
	bool timesReceived;

	// bool timeReceivedChecker();

public:
	cntrlState()
	{
		WDrunMode = UNKNOWNMODE; //AUTOMODE;
		WDreturnToModeAfterNEXT = UNKNOWNMODE;
		WDswitchBack = SBUNKOWN;
		WDzone = ZONEGAP;
		WDHoldState = 9;
		WDcntrlTimesReceived = false;
		WDcommandReceived = false;

		WErunMode = UNKNOWNMODE;
		WEreturnToModeAfterNEXT = UNKNOWNMODE;
		WEswitchBack = SBUNKOWN;
		WEzone = ZONEGAP;
		WEHoldState = 9;
		WEcntrlTimesReceived = false;
		WEcommandReceived = false;
		WDBypassTimesTopic = "UNDEF";
		WEBypassTimesTopic = "UNDEF";

		refreshID = "UNDEF";
		cntrlName = "UNDEF";

		outputState = 0; // Off
	}
	/*
	cntrlState(int rm, int sb, int zn)
	{
		WDrunMode = rm;
		WDswitchBack = sb;
		WDzone = zn;
		WDHoldState = 9;
		WDcntrlTimesReceived = false;
		WDcommandReceived = false;

		WErunMode = rm;
		WEswitchBack = sb;
		WEzone = zn;
		WEHoldState = 9;
		WEcntrlTimesReceived = false;
		WEcommandReceived = false;

		refreshID = "UNDEF";
		cntrlName = "UNDEF";
		outputState = 0;
	}
	*/
	/*
	void setup(int rm, int sb, int zn)
	{
		WDrunMode = rm;
		WDswitchBack = sb;
		WDzone = zn;
		WDHoldState = 9;
		WDcntrlTimesReceived = false;
		WDcommandReceived = false;

		WErunMode = rm;
		WEswitchBack = sb;
		WEzone = zn;
		WEHoldState = 9;
		WEcntrlTimesReceived = false;
		WEcommandReceived = false;

		refreshID = "UNDEF";
		cntrlName = "UNDEF";
		outputState = 0;
	}
	*/
	String getCntrlName()
	{
		return cntrlName;
	}
	int getWDRunMode()
	{
		return WDrunMode;
	}
	int getWDSwitchBack()
	{
		return WDswitchBack;
	}
	int getWDZone()
	{
		return WDzone;
	}
	int getOutputState()
	{
		return outputState;
	}
	void setOutputState(int os)
	{
		outputState = os;
	}
	void setWDRunMode(int rm)
	{
		WDrunMode = rm;
		// Publish to set OpenHab item
		char topic[100];
		snprintf(topic, sizeof(topic), StateCntrlModeWD, cntrlName.c_str());
		mqttClient.publish(topic, 0, false, runmodeText(rm).c_str()); 
	}
	void setWDreturnToModeAfterNEXT(int rman)
	{
		WDreturnToModeAfterNEXT = rman;
	}
	void setWDSwitchBack(int sb)
	{
		WDswitchBack = sb;
	}
	void setWDHoldState(int hs)
	{
		WDHoldState = hs;
	}
	void setWEHoldState(int hs)
	{
		WEHoldState = hs;
	}
	void setWDZone(int zn)
	{
		WDzone = zn;
	}

	int getWERunMode()
	{
		return WErunMode;
	}
	int getWESwitchBack()
	{
		return WEswitchBack;
	}
	int getWEZone()
	{
		return WEzone;
	}
	void setWERunMode(int rm)
	{
		WErunMode = rm;

		char topic[100];
		snprintf(topic, sizeof(topic), StateCntrlModeWE, cntrlName.c_str());
		mqttClient.publish(topic, 0, false, runmodeText(rm).c_str()); 
	}
	void setWEreturnToModeAfterNEXT(int rman)
	{
		WEreturnToModeAfterNEXT = rman;
	}
	void setWESwitchBack(int sb)
	{
		WEswitchBack = sb;
	}
	void setWEZone(int zn)
	{
		WEzone = zn;
	}

	void setCntrlObjRef(cntrlState &obj)
	{
		cntrlObjRef = &obj;
	}

	void setWDCntrlTimesTopic(String timesTopic)
	{
		WDcntrlTimesTopic = timesTopic;
		mqttTopicsubscribe(WDcntrlTimesTopic.c_str(), 2); // Subscribe
	}

	void setWDBypassTimesTopic(String bypassWDTimesTopic)
	{
		WDBypassTimesTopic = bypassWDTimesTopic;
		mqttTopicsubscribe(bypassWDTimesTopic.c_str(), 2); // Subscribe
	}

	void setWEBypassTimesTopic(String bypassWETimesTopic)
	{
		WEBypassTimesTopic = bypassWETimesTopic;
		mqttTopicsubscribe(bypassWETimesTopic.c_str(), 2); // Subscribe
	}

	void setWDUIcommandStateTopic(String UIcmdTopic)
	{
		WDUICmdTopic = UIcmdTopic;
		mqttTopicsubscribe(WDUICmdTopic.c_str(), 2); // Subscribe
	}
	void setWECntrlTimesTopic(String timesTopic)
	{
		WEcntrlTimesTopic = timesTopic;
		mqttTopicsubscribe(WEcntrlTimesTopic.c_str(), 2); // Subscribe
	}
	void setWEUIcommandStateTopic(String UIcmdTopic)
	{
		WEUICmdTopic = UIcmdTopic;
		mqttTopicsubscribe(WEUICmdTopic.c_str(), 2); // Subscribe
	}

	void setWDCntrlRunTimesStateTopic(String runtimeState)
	{
		WDCntrlRuntimeStateTopic = runtimeState;
	}
	void setWECntrlRunTimesStateTopic(String runtimeState)
	{
		WECntrlRuntimeStateTopic = runtimeState;
	}

	void setWDCntrlTimesReceived(bool ctr)
	{
		WDcntrlTimesReceived = ctr;
	}
	void setWECntrlTimesReceived(bool ctr)
	{
		WEcntrlTimesReceived = ctr;
	}
	void setRefreshID(String rID)
	{
		refreshID = rID;
	}
	void setCntrlName(String cID)
	{
		cntrlName = cID;
	}

	String getRefreshID()
	{
		return refreshID;
	}
	bool getWDCntrlTimesReceived()
	{
		return WDcntrlTimesReceived;
	}
	bool getWECntrlTimesReceived()
	{
		return WEcntrlTimesReceived;
	}
	String getWDCntrlTimesTopic()
	{
		return WDcntrlTimesTopic;
	}
	String getWECntrlTimesTopic()
	{
		return WEcntrlTimesTopic;
	}
	String getWDCntrlRunTimesStateTopic()
	{
		return WDCntrlRuntimeStateTopic;
	}
	String getWECntrlRunTimesStateTopic()
	{
		return WECntrlRuntimeStateTopic;
	}
	String getWDBypassTimesTopic()
	{
		return WDBypassTimesTopic;
	}
	String getWEBypassTimesTopic()
	{
		return WEBypassTimesTopic;
	}

	String getWDUIcommandStateTopic()
	{
		return WDUICmdTopic;
	}
	String getWEUIcommandStateTopic()
	{
		return WEUICmdTopic;
	}
	bool getWDCommandReceived()
	{
		return WDcommandReceived;
	}
	bool getWECommandReceived()
	{
		return WEcommandReceived;
	}
	int getWDHoldState()
	{
		return WDHoldState;
	}
	int getWEHoldState()
	{
		return WEHoldState;
	}
	int getWDreturnToModeAfterNEXT()
	{
		return WDreturnToModeAfterNEXT;
	}
	int getWEreturnToModeAfterNEXT()
	{
		return WEreturnToModeAfterNEXT;
	}
//	bool getWEBypassMode()
//	{
//		return WEBypassMode;
//	}
//	bool getWDBypassMode()
//	{
//		return WDBypassMode;
//	}
	//****************************************************************
	// Process any application specific inbound MQTT messages
	// Return False if none
	// Return true if an MQTT message was handled here
	//****************************************************************
	bool onMqttMessageCntrlExt(char *topic, char *payload, const AsyncMqttClientMessageProperties &properties, const size_t &len, const size_t &index, const size_t &total)
	{
		(void)payload;
		// char logString[MAX_LOGSTRING_LENGTH];

		char mqtt_payload[len + 1];
		mqtt_payload[len] = '\0';
		strncpy(mqtt_payload, payload, len);

		String msg = "Received MQTT. Topic: " + (String)topic + " Payload: " + (String)mqtt_payload;
        mqttLog(msg.c_str(), REPORT_DEBUG, true, true);

		/****************************************************
		 * Week days Times received
		 * array of three times zones : on,off,on,off,on,off
		 ****************************************************/
		if (strcmp(topic, getWDCntrlTimesTopic().c_str()) == 0)
		{
			mqttLog("WD Control times Received", REPORT_INFO, true, true);
			processCntrlTimes(mqtt_payload, WDcntrlTimes, WDlcntrlTimes);

			setWDCntrlTimesReceived(true);
			return true;
		}
		/****************************************************
		 * Week End Times received
		 * array of three times zones : on,off,on,off,on,off
		 ****************************************************/
		else if (strcmp(topic, getWECntrlTimesTopic().c_str()) == 0)
		{
			mqttLog("WE Control times Received", REPORT_INFO, true, true);
			processCntrlTimes(mqtt_payload, WEcntrlTimes, WElcntrlTimes);

			setWECntrlTimesReceived(true);
			return true;
		}
		/************************************************************************
		 * Weekday Command Control
		 * Message content:
		 * ON :  Set control permanently on. Ignore all time of day settings
		 * OFF:  Set control permanently off. Ignore all time of day settings
		 * ONOFF: open gates and allow to close normally
		 * NEXT: Move control setting to next time zone
		 * SET:  Set the heat times
		 ************************************************************************/
		else if (strcmp(topic, getWDUIcommandStateTopic().c_str()) == 0)
		{
			//if (reporting == REPORT_DEBUG)
			//{
				String msg = "Received getWDUIcommandStateTopic : " + getWDUIcommandStateTopic();
				mqttLog(msg.c_str(), REPORT_DEBUG, true, true);
			//}
			WDcommandReceived = true;

			if (processCntrlMessage(mqtt_payload, "ON", "OFF", getWDUIcommandStateTopic().c_str()) == true)
			{
				Serial.print("ERROR: Unknown message - ");
				Serial.print(mqtt_payload);
				Serial.print(" - received for topic ");
				Serial.println(getWDUIcommandStateTopic().c_str());
			}
			return true;
		}
		/************************************************************************
		 * Weekend Command Control
		 * Message content:
		 * ON :  Set control permanently on. Ignore all time of day settings
		 * OFF:  Set control permanently off. Ignore all time of day settings
		 * NEXT: Move control setting to next time zone
		 * SET:  Set the heat times
		 ************************************************************************/
		else if (strcmp(topic, getWEUIcommandStateTopic().c_str()) == 0)
		{
			//if (reporting == REPORT_DEBUG)
			//{
				String msg = "Received getWEUIcommandStateTopic : " + getWEUIcommandStateTopic();
				mqttLog(msg.c_str(), REPORT_DEBUG, true, true);
			//}
			WEcommandReceived = true;
			if (processCntrlMessage(mqtt_payload, "ON", "OFF", getWEUIcommandStateTopic().c_str()) == true)
			{
				//FIXTHIS
				Serial.print("ERROR: Unknown message - ");
				Serial.print(mqtt_payload);
				Serial.print(" - received for topic ");
				Serial.println(getWEUIcommandStateTopic().c_str());
			}
			return true;
		}
/*		else if (strcmp(topic, getWDBypassTimesTopic().c_str()) == 0)
		{
			String msg = "Received getWDBypassTimesTopic : " + getWDBypassTimesTopic();
			mqttLog(msg.c_str(), REPORT_DEBUG, true, true);

			if (strcmp(mqtt_payload, "BPON") == 0)
			{
				WDBypassMode = true;
			}
			else
			{
				WDBypassMode = false;
			}
		}
		else if (strcmp(topic, getWEBypassTimesTopic().c_str()) == 0)
		{
			String msg = "Received getWEBypassTimesTopic : " + getWEBypassTimesTopic();
			mqttLog(msg.c_str(), REPORT_DEBUG, true, true);

			if (strcmp(mqtt_payload, "BPON") == 0)
			{
				WEBypassMode = true;
			}
			else
			{
				WEBypassMode = false;
			}
		}
*/
		else
		{
			// mqttLog("Unknown message Received Topic and payload: ", true, true);
			// mqttLog(topic, true, true);
			// mqttLog(mqtt_payload, true, true);

			return false;
		}
		return true;
	}

	//***********************************************************************************
	// inStr:   INPUT.  A string containing the times
	// ptr:     INPUT.  points to the array of ON and OFF times as characters
	// lptr:    OUTPUT. The function populates the array with ON and OFF times as integers
	//************************************************************************************
	void processCntrlTimes(char *inStr, char (&ptr)[6][10], int lptr[6])
	{
		/************************************************************************
		 * Parse the string containing the on and off times into a string array.
		 * e.g. 06:30,09:20,15:10,17:40,20:00,23:50 : This means
		 *
		 *  	[0] 06:30 - ON		Time Zone 1
		 * 		[1] 09:20 - OFF		Time Zone 1
		 * 		[2] 15:10 - ON		Time Zone 2
		 * 		[3] 17:40 - OFF		Time Zone 2
		 * 		[4] 20:00 - ON		Time Zone 3
		 * 		[5] 23:50 - OFF		Time Zone 3
		 *
		 ************************************************************************/
		int i = 0;
		char *pch;
		char timeString[10];
		char inStrLocal[255]; // local copy}
		memset(inStrLocal, '\0', sizeof(inStrLocal));
		strcpy(inStrLocal, inStr);
		// mqttLog(inStrLocal, true, true);

		pch = strtok(inStrLocal, ",");
		while (pch != NULL)
		{
			if (pch != NULL)
			{
				char *output = &ptr[i][0];
				for (int i = 0, j = 0; i < 4; i++, j++) // Evaluate each character in the input
				{
					if (pch[i] != ':')		// If the character is not a space
						output[j] = pch[i]; // Copy that character to the output char[]
					else
						j--; // If it is a space then do not increment the output index (j), the next non-space will be entered at the current index
				}
				output[4] = '\0';

				memset(timeString, 0, sizeof(timeString));
				sprintf(timeString, "%s", output);
				// sprintf(timeString, "%s", &ptr[i][0]);

				// mqttLog(timeString, true, true);

				lptr[i] = atoi(timeString); // Store integer value of time
											// comment out
											// mqttLog(timeString,true);
			}
			pch = strtok(NULL, ",");
			i++;
		}
	}

	/************************************************************
	 * Process lighting control ON, OFF, SET and NEXT messages
	 * returns: false = ok
	 * 			true  = fail
	 ************************************************************/
	bool processCntrlMessage(char *mqttMessage, const char *onMessage, const char *offMessage, const char *commandTopic)
	{
		
		
		String msg = cntrlName + ",processCntrlMessage : " + (String)commandTopic + " " + (String)mqttMessage;
		mqttLog(msg.c_str(), REPORT_DEBUG, true, true);

		if (strcmp(mqttMessage, "ON") == 0)
		{
			if (strcmp(commandTopic, getWDUIcommandStateTopic().c_str()) == 0)
			{
				setWDRunMode(ONMODE);
				app_WD_on(cntrlObjRef);
			}
			else if (strcmp(commandTopic, getWEUIcommandStateTopic().c_str()) == 0)
			{
				setWERunMode(ONMODE);
				app_WE_on(cntrlObjRef);
			}
		}
		else if (strcmp(mqttMessage, "OFF") == 0)
		{
			if (strcmp(commandTopic, getWDUIcommandStateTopic().c_str()) == 0)
			{
				// String msg = "processCntrlMessage-1 : " + (String)commandTopic;
				// mqttLog(msg.c_str(), true, true);
				setWDRunMode(OFFMODE);
				setWDSwitchBack(SBUNKOWN);
				app_WD_off(cntrlObjRef);
			}
			else if (strcmp(commandTopic, getWEUIcommandStateTopic().c_str()) == 0)
			{
				// String msg = "processCntrlMessage-2 : " + (String)commandTopic;
				// mqttLog(msg.c_str(), true, true);
				setWERunMode(OFFMODE);
				setWESwitchBack(SBUNKOWN);
				app_WE_off(cntrlObjRef);
			}
		}
		else if (strcmp(mqttMessage, "ONOFF") == 0)
		{
			app_WD_on(cntrlObjRef);	 // FIXTHIS WD or WE
			delay(5000);			 // FIX this
			app_WD_off(cntrlObjRef); // FIXTHIS WD or WE

			// setWDRunMode(AUTOMODE); // FIX THIS : why am I doing this? leave as is
		}
		else if (strcmp(mqttMessage, "NEXT") == 0)
		{
			// FIXTHIS If the heating runstate is OFF (not SET) we want the heating to come ON and go back to OFF state. Dont go back to SET state
			// FIXTHIS If the heating runstate is ON (not SET) we want the heating to come OFF and go back to ON state. Dont go back to SET state

			if (strcmp(commandTopic, getWDUIcommandStateTopic().c_str()) == 0)	//Weekday
			{
				setWDreturnToModeAfterNEXT(getWDRunMode());

                String logRecord = "WD NEXT received. WDZone: " + (String)getWDZone() + ", WDRunMode: " + runmodeText(getWDRunMode()) + ", Output State: " + (String)getOutputState() + " Hold State: " + getWDHoldState();
				mqttLog(logRecord.c_str(), REPORT_INFO, true, true);

				setWDRunMode(NEXTMODE);
				setWDSwitchBack(SBOFF); // Switch back to AUTOMODE when Time of Day is next OFF (don't switch back when this zone ends)

				//String logRecord = "NEXT received. Zone = " + (String)getWDZone() + " RunMode: " + (String)getWDRunMode() + " Output State: " + (String)getOutputState();
				

				if (getOutputState() == 0) 		// 0 = Off
				//if (onORoff() == true) // if true then we are in a heating zone. Next means stay switched on until next zone.
				{
					setWDHoldState(1); 			// the state to hold while in NEXTMDDE (Heat ON)
					app_WD_on(cntrlObjRef); 	// Switch Heat on until the end of the next heaing zone OFF time
				} 
				else
				{
					setWDHoldState(0);		 // the state to hold while in NEXTMDDE (Heat OFF)
					app_WD_off(cntrlObjRef); // Set off because we want OFF until the end of the next ON
				}
				logRecord = "WD NEXT changed. WDZone:" + (String)getWDZone() + ", WDRunMode: " + runmodeText(getWDRunMode()) + ", Output State: " + (String)getOutputState() + " Hold State: " + getWDHoldState();
				mqttLog(logRecord.c_str(), REPORT_INFO, true, true);
			}
			else if (strcmp(commandTopic, getWEUIcommandStateTopic().c_str()) == 0) //Weekend
			{
				setWEreturnToModeAfterNEXT(getWERunMode());

				String logRecord = "WE NEXT received. WEZone = " + (String)getWEZone() + ", WERunMode: " + runmodeText(getWERunMode()) + ", Current Output State: " + (String)getOutputState();
				mqttLog(logRecord.c_str(), REPORT_INFO, true, true);

				setWERunMode(NEXTMODE);
				setWESwitchBack(SBOFF); // Switch back to AUTOMODE when Time of Day is next ON (don't switch back when this zone ends)

				//String logRecord = "NEXT received. Zone = " + (String)getWEZone() + " RunMode: " + (String)getWERunMode() + " Output State: " + (String)getOutputState();
				// FIXTHIS 2911logRecord = "NEXT received. WEZone = " + (String)getWEZone() + ", WERunMode: " + runmodeText(getWERunMode()) + ", Current Output State: " + (String)getOutputState();
				// Fixthis 2911 mqttLog(logRecord.c_str(), REPORT_INFO, true, true);	

				// mqttClient.publish(getWECntrlRunTimesStateTopic().c_str(), 0, true, "ON"); // FIXTHIS WD or WE
				//if (onORoff() == true) 		// if true then we are in a heating zone. Next means stay switched on until next zone.
				if (getOutputState() == 0)		// 0 = Off
				{
					setWEHoldState(1);			// the state to hold while in NEXTMDDE (Heat ON)
					app_WE_on(cntrlObjRef); 	// Set on because we want ON until the end of the next OFF
				}
				else
				{			
					setWEHoldState(0);			// the state to hold while in NEXTMDDE (Heat OFF)
					app_WE_off(cntrlObjRef);
				}
			}
		}
		else if (strcmp(mqttMessage, "SET") == 0)
		{
			// IF pressed SET then check the ON Close time and sent the appropriate message
			if (strcmp(commandTopic, getWDUIcommandStateTopic().c_str()) == 0)
			{
				String msg = cntrlName + ",processCntrlMessage: SET received from: " + getWDUIcommandStateTopic() ;
				mqttLog(msg.c_str(), REPORT_INFO, true, true);

				setWDRunMode(AUTOMODE);
				setWDHoldState(9);
				if (onORoff() == true)
				{
					app_WD_on(cntrlObjRef);
				}
				else
				{
					app_WD_off(cntrlObjRef);
				}
			}
			else if (strcmp(commandTopic, getWEUIcommandStateTopic().c_str()) == 0)
			{
				String msg = cntrlName + ",processCntrlMessage: SET received from: " + getWEUIcommandStateTopic() ;
				mqttLog(msg.c_str(), REPORT_INFO, true, true);

				setWERunMode(AUTOMODE);
				setWEHoldState(9);
				if (onORoff() == true)
				{
					app_WE_on(cntrlObjRef);
				}
				else
				{
					app_WE_off(cntrlObjRef);
				}
			}
		}
		else
		{
			// Application Specific handling of message as not for Contolller
			processCntrlMessageApp_Ext(mqttMessage, onMessage, offMessage, commandTopic);
			return true;
		}
		processCntrlMessageApp_Ext(mqttMessage, onMessage, offMessage, commandTopic);
		// mqttLog("Returning from processCntrlMessage: ", true, true);
		return false;
	}

	/***********************************************************************
	 * Determines if control should be ON or OFF based on current zone times
	 * and the day of the week i.e. weekday or weekend.
	 * Uses and updates global variables:
	 *    weekDay,
	 *    lcntrlTimes,
	 *    lcntrlTimesWE,
	 *    Zone,
	 *    ZoneWE.
	 *
	 * Returns true to switch ON, false to switch OFF
	 ************************************************************************/
	bool onORoff()
	{

		// char logString[MAX_LOGSTRING_LENGTH];
		//  debugPrint();
		// mqttLog("processCntrlMessage: onORoff checking", true, true);

		if (coreServices.getWeekDayState() == true)
			setWDZone(ZONEGAP); // not in a zone
		else
			setWEZone(ZONEGAP);

		bool state = false;

		if (readyCheck() == true) // All conditions to start are met
		{
			/****************************************************************
			 * Work out from the array of longs if the control is On or OFF
			 ****************************************************************/
			// Serial.print("PS ZONE CONFIG: ");

			// for (int i = 0, j = 0; i < 6; i++, j++)
			int j = 1;
			for (int i = 0; i < 6; i++)
			{
				// Serial.print (", Zone: ");
				// Serial.print ((int) i - j);
				// Serial.print ("[");
				// Serial.print ( lcntrlTimes[i]);
				// Serial.print (",");
				// Serial.print ( lcntrlTimes[i+1]);
				// Serial.print ("] ");

				// memset(logString,0, sizeof logString);
				// sprintf(logString, "%s,%s,%s,%s[%i: %i: %i]", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "ohTimenow: ", ohTimenow, lcntrlTimesWD[i], lcntrlTimesWE[i]);
				// mqttLog(logString,true, true);

				if (coreServices.getWeekDayState() == true)
				{
					if (ohTimenow >= WDlcntrlTimes[i] && ohTimenow < WDlcntrlTimes[i + 1])
					{
						// if (getOutputState() == 0)
						state = true; // Switch on
						// else
						//	state = false; // Switch off
						setWDZone(j); // Update which zone we are in (there maybe overlapping time zones)
						break;
					}
				}
				else
				{
					if (ohTimenow >= WElcntrlTimes[i] && ohTimenow < WElcntrlTimes[i + 1])
					{
						// if (getOutputState() == 0)
						state = true; // Switch on
						// else
						//	state = false; // Switch off
						setWEZone(j); // Update which zone we are in (there maybe overlapping time zones)
						break;
					}
				}
				i++;
				j++;
			}
		}
		return state;
	}

	//**********************************************************************
	// Controller TOD Porocessing
	// check current time against configutation and decide what to do.
	// Send appropriate MQTT command for each control.
	// Run this everytime the TOD changes
	// If TOD event is not received then nothing happens
	//**********************************************************************
	void processCntrlTOD_Ext()
	{
		bool onORoffstate = false;
		int wdzone = 9;
		int wezone = 9;

		String msg = cntrlName + " Contoller Processing TOD";
		mqttLog(msg.c_str(), REPORT_INFO, true, true);

		char logString[MAX_LOGSTRING_LENGTH];

		// Check all WD and WE times have been received before doing anything
		// Send the WD and WE messages to the application irrespective of the day..
		// Let the app decide what to do.
		if (runTimeReceivedCheck() == true)
		{
			String logRecord = "WDRunMode: " + (String)getWDRunMode() + ", WDSwitchBack: " + (String)getWDSwitchBack() + ", WDZone: " + (String)wdzone + ", WDonOroff: " + (String)onORoffstate + ", WDHold: " + getWDHoldState();
			mqttLog(logRecord.c_str(), REPORT_INFO, true, true);
			logRecord = "WERunMode: " + (String)getWDRunMode() + ", WESwitchBack: " + (String)getWESwitchBack() + ", WEZone: " + (String)wezone + ", WEonOroff: " + (String)onORoffstate + ", WEHold: " + getWEHoldState();
			mqttLog(logRecord.c_str(), REPORT_INFO, true, true);


			// if (getWDRunMode() == NEXTMODE && coreServices.getWeekDayState() == true)
			if (getWDRunMode() == NEXTMODE)
			{
				onORoffstate = onORoff(); //The Programmed values - true(1) to switch ON, false(0) to switch OFF
				wdzone = getWDZone();

				//String logRecord = "RunMode: " + (String)getWDRunMode() + " SwitchBack: " + (String)getWDSwitchBack() + " Zone: " + (String)wdzone + " onOroff: " + (String)onORoffstate + " Hold: " + getWDHoldState();
				//mqttLog(logRecord.c_str(), REPORT_INFO, true, true);

				if (getWDHoldState() == onORoffstate) // If same then in the next heating zone so go back and run like normal
				{
					//logRecord = "WD NEXT Mode returning to AUTO Mode";
					//mqttLog(logRecord.c_str(), REPORT_INFO, true, true);

					//setWDSwitchBack(SBON); // SBON means switch back to normal operation at the end of a gap period
					//FIXTHIS dont assume to switch bak AUTOMODE. When NEXT was initated original state could be OFF or ON
					//setWDRunMode(AUTOMODE);
					//app_WD_auto(cntrlObjRef);

					//FIXTHIS 01/05/2025
					if (getWDreturnToModeAfterNEXT() == AUTOMODE)
					{
						logRecord = "WD NEXT Mode returning to AUTO Mode";
						mqttLog(logRecord.c_str(), REPORT_INFO, true, true);

						setWDSwitchBack(SBON); // SBON means switch back to normal operation at the end of a gap period
						//FIXTHIS dont assume to switch bak AUTOMODE. When NEXT was initated original state could be OFF or ON
						setWDRunMode(AUTOMODE);
						app_WD_auto(cntrlObjRef);
					}
					else if (getWDreturnToModeAfterNEXT() == OFFMODE)
					{
						logRecord = "WD NEXT Mode returning to OFF Mode";
						mqttLog(logRecord.c_str(), REPORT_INFO, true, true);

						setWDSwitchBack(SBON); // SBON means switch back to normal operation at the end of a gap period
						//FIXTHIS dont assume to switch bak AUTOMODE. When NEXT was initated original state could be OFF or ON
						setWDRunMode(OFFMODE);
						app_WD_off(cntrlObjRef);
					}
					else
					{
						logRecord = "ERROR: WD NEXT Mode returning unknown";
						mqttLog(logRecord.c_str(), REPORT_INFO, true, true);
					}
				}
			}
			if (getWERunMode() == NEXTMODE)
			{
				onORoffstate = onORoff();
				wezone = getWEZone();

				//String logRecord = "SwitchBack: " + (String)getWESwitchBack() + " Zone: " + (String)wezone + " onOroff: " + (String)onORoffstate + " Hold: " + getWEHoldState();
				//mqttLog(logRecord.c_str(), REPORT_WARN, true, true);

				if (getWEHoldState() == onORoffstate)
				{
					setWESwitchBack(SBON); // SBON means switch back to normal operation at the end of a gap period
					//FIXTHIS dont assume to switch bak AUTOMODE. When NEXT was initated original state could be OFF or ON
					setWERunMode(AUTOMODE);
					app_WE_auto(cntrlObjRef);
				}
			}
			//else if (getWDRunMode() == AUTOMODE && coreServices.getWeekDayState() == true)
			if (getWDRunMode() == AUTOMODE)
			{
				if (onORoff() == true)
				{
					//FIXTHIS dont assume to switch bak AUTOMODE. When NEXT was initated original state could be OFF or ON
					// FIXTHIS if getWDreturnToModeAfterNEXT() == OFFMODE then app_WE_off(cntrlObjRef);
					app_WD_on(cntrlObjRef);
				}
				else
				{
					//FIXTHIS dont assume to switch bak AUTOMODE. When NEXT was initated original state could be OFF or ON
					// FIXTHIS if getWDreturnToModeAfterNEXT() == OFFMODE then app_WE_off(cntrlObjRef);
					app_WD_off(cntrlObjRef);
				}
			}
			//else if (getWERunMode() == AUTOMODE && coreServices.getWeekDayState() == false)
			if (getWERunMode() == AUTOMODE)
			{
				if (onORoff() == true)
				{
					// FIXTHIS dont assume to switch bak AUTOMODE. When NEXT was initated original state could be OFF or ON
					// FIXTHIS If state NEXT was requested when runstate was OFF then reurn back to OFF state after NEXT processing
					// FIXTHIS if getWEreturnToModeAfterNEXT() == OFFMODE then app_WE_off(cntrlObjRef);
					app_WE_on(cntrlObjRef);
				}
				else
				{
					// FIXTHIS dont assume to switch bak AUTOMODE. When NEXT was initated original state could be OFF or ON
					// FIXTHIS If state NEXT was requested when runstate was ON then reurn back to ON state after NEXT processing
					// FIXTHIS if getWEreturnToModeAfterNEXT() == ONMODE then app_WE_off(cntrlObjRef);
					app_WE_off(cntrlObjRef);
				}
			}
			// FIXTHIS  logic not sound - I think do each individually
			//else if (getWDRunMode() == ONMODE)
			if (getWDRunMode() == ONMODE)
			{
				app_WD_on(cntrlObjRef);
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s", getCntrlName().c_str(), espDevice.getName().c_str(), "WD Permanently ON");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			//else if (getWDRunMode() == OFFMODE)
			if (getWDRunMode() == OFFMODE)
			{
				app_WD_off(cntrlObjRef);
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s", getCntrlName().c_str(), espDevice.getName().c_str(), "WD Permanently OFF");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			//else if (getWERunMode() == ONMODE)
			if (getWERunMode() == ONMODE)
			{
				app_WE_on(cntrlObjRef);
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s", getCntrlName().c_str(), espDevice.getName().c_str(), "WE Permanently ON");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			//else if (getWERunMode() == OFFMODE)
			if (getWERunMode() == OFFMODE)
			{
				app_WE_off(cntrlObjRef);
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s", getCntrlName().c_str(), espDevice.getName().c_str(), "WE Permanently OFF");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			//else
			//{
			//	memset(logString, 0, sizeof logString);
			//	sprintf(logString, "%s,%s,%s", getCntrlName().c_str(), espDevice.getName().c_str(), "Unknown running mode ");
			//	mqttLog(logString, REPORT_WARN, true, true);
			//}
			// 11/05/2025Just call set to publish MQTT to UI every TOD tick FIXTHIS a bit untidy
			setWDRunMode(getWDRunMode());
			setWERunMode(getWERunMode());

		}
	}
	/*
	 * Check that the initialisation data all been received before processing
	 * Returns True if all checks have been received
	 */
	bool readyCheck()
	{
		if (ohTODReceived == true && getWDCntrlTimesReceived() == true && getWECntrlTimesReceived() == true)
		{
			// mqttLog("readyCheck = true", true, true);
			return true;
		}
		else
		{
			return false;
		}
	}

	// Triggerd by xxx ticker
	bool runTimeReceivedCheck()
	{
		char logString[MAX_LOGSTRING_LENGTH];

		if (getWDCntrlTimesReceived() == false || getWECntrlTimesReceived() == false || WDcommandReceived == false || WEcommandReceived == false || ohTODReceived == false)
		{
			memset(logString, 0, sizeof logString);
			sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Waiting for all initialisation messages to be received");
			mqttLog(logString, REPORT_INFO, true, true);

			if (ohTODReceived == false)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Time of Day not yet received");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			if (getWDCntrlTimesReceived() == false)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Weekday control times not yet received");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			if (getWECntrlTimesReceived() == false)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Weekend control times not yet received");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			if (WDcommandReceived == false) // && coreServices.getWeekDayState() == true)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Weekday operating mode not yet received");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			if (WEcommandReceived == false) // && coreServices.getWeekDayState() == false)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Weekend operating mode not yet received");
				mqttLog(logString, REPORT_INFO, true, true);
			}
			// FIXTHIS ... WD and WE python returns both
			String msg = "Requesting configuration from Database";
			mqttLog(msg.c_str(), REPORT_INFO, true, true);
			//FIXTHIS: Why Send QOS==0 and Retained==false? Was retained == true 
			mqttClient.publish(oh3StateIOTRefresh, 0, false, getRefreshID().c_str()); // QOS == 0, Reatined == false
			return false;
		}
		else
			return true;
	}

	//************************************************************************
	// Write out over telnet session and application specific infomation
	// FIXTHIS: Move to utilities
	//***********************************************************************
	void telnet_extension_1(char c)
	{
		// char logString[MAX_LOGSTRING_LENGTH];
		// memset(logString, 0, sizeof logString);
		// sprintf(logString, "%s%d\n\r", "Sensor Value:\t", sensorValue);
		// printTelnet((String)logString);

		String wd;
		String rmWD, rmWE;
		char stringOfTimesWD[70];
		char stringOfTimesWE[70];
		// get weekday
		wd = "Week Day";
		if (coreServices.getWeekDayState() == false)
			wd = "Weekend";

		// Get on, off times
		sprintf(stringOfTimesWD, "%s,%s,%s,%s,%s,%s", WDcntrlTimes[0], WDcntrlTimes[1], WDcntrlTimes[2], WDcntrlTimes[3], WDcntrlTimes[4], WDcntrlTimes[5]);
		sprintf(stringOfTimesWE, "%s,%s,%s,%s,%s,%s", WEcntrlTimes[0], WEcntrlTimes[1], WEcntrlTimes[2], WEcntrlTimes[3], WEcntrlTimes[4], WEcntrlTimes[5]);

		// get Run Mode
		if (getWDRunMode() == AUTOMODE)
			rmWD = "AUTOMODE";
		else if (getWDRunMode() == NEXTMODE)
			rmWD = "NEXTMODE";
		else if (getWDRunMode() == ONMODE)
			rmWD = "ONMODE";
		else if (getWDRunMode() == OFFMODE)
			rmWD = "OFFMODE";
		else
			rmWD = "UNKNOWN";

		if (getWERunMode() == AUTOMODE)
			rmWE = "AUTOMODE";
		else if (getWERunMode() == NEXTMODE)
			rmWE = "NEXTMODE";
		else if (getWERunMode() == ONMODE)
			rmWE = "ONMODE";
		else if (getWERunMode() == OFFMODE)
			rmWE = "OFFMODE";
		else
			rmWE = "UNKNOWN";

		// Print out on Telnet terminal
		char logString[MAX_LOGSTRING_LENGTH];
		// memset(logString, 0, sizeof logString);
		sprintf(logString, "%s%s\r", "Cntlr Name:\t", cntrlName.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "Refresh ID:\t", refreshID.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "Week Day:\t", wd.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WD Times:\t", stringOfTimesWD);
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WE Times:\t", stringOfTimesWE);
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WD Run Mode:\t", rmWD.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WE Run Mode:\t", rmWE.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%i\r", "WD SB Mode:\t", getWDSwitchBack());
		printTelnet((String)logString);
		sprintf(logString, "%s%i\r", "WE SB Mode:\t", getWESwitchBack());
		printTelnet((String)logString);
//		sprintf(logString, "%s%i\r", "WD Bypass mode:\t", getWDBypassMode());
//		printTelnet((String)logString);
//		sprintf(logString, "%s%i\r", "WE Bypass mode:\t", getWEBypassMode());
//		printTelnet((String)logString);
		sprintf(logString, "%s%i\r", "WD Mode after NEXT:\t", getWDreturnToModeAfterNEXT());
		printTelnet((String)logString);
		sprintf(logString, "%s%i\r", "WE Mode after NEXT:\t", getWEreturnToModeAfterNEXT());
		printTelnet((String)logString);
		sprintf(logString, "%s%s%i%s%i%s%i%s%i\r", "Cntrl config:\t", "WDT:", getWDCntrlTimesReceived(), " WET:", getWECntrlTimesReceived(), " WDC:", getWDCommandReceived(), " WEC:", getWECommandReceived());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WD CT Topic:\t", getWDCntrlTimesTopic().c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WE CT Topic:\t", getWECntrlTimesTopic().c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WD Cmd Topic:\t", getWDUIcommandStateTopic().c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WE Cmd Topic:\t", getWEUIcommandStateTopic().c_str());
		printTelnet((String)logString);
		printTelnet((String) " ");
	}
};

String runmodeText(int rm)
{
/*
#define AUTOMODE    0 Normal running mode - Heating times are based on the 3 time zones
#define NEXTMODE    1 Advances the control time to the next zone. FIX-THIS: Crap description
#define ONMODE      2 Permanently ON.  Heat is permanently requested. Zones times are ignored
#define OFFMODE     3 Permanently OFF.  Heat is never requested. Zones times are ignored
#define UNKNOWNMODE 4 set at start up 
*/

if (rm == 0) return "AUTOMODE";
else if (rm == 1) return "NEXTMODE";
else if (rm == 2) return "ONMODE";
else if (rm == 3) return "OFFMODE";
else return "UNKNOWNMODE";
}

#endif
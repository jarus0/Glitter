/*
 * Jfunctionality.h
 *
 *  Created on: 04-Mar-2020
 *      Author: suraj
 */

#ifndef MAIN_JFUNCTIONALITY_H_
#define MAIN_JFUNCTIONALITY_H_

#include "Accessories.h"

#undef INFO
#define INFO(MSG) jTool.info(__FILE__, (String (__LINE__)).c_str(), __func__, MSG );

#include <iostream>
#include <Arduino.h>

#include "FirmwareType.h"
// Future task why multiple includtion make it once

#include "cJSON.h"
#include "esp_task_wdt.h"

extern bool flagForFLowExecution;

enum returnType {
	jexecutionOK,
	jexecutionOKwithData,
	jexecutionProblem,
	jexecutionUndefine,
	jexecutionOKReturnRaw,
	jCopyDataPart
};

struct ACstatusStore{
	jStr lastCMD;
	jStr lastCmdTime;
	int type; 				//0-Schedule or 1-Flow
	jStr typeName;
};

#include "globalFunctions.h"

class Jfunctionality {
public:
	Jfunctionality();
	virtual ~Jfunctionality();

	Accessories jTool;

	std::mutex customMutez;           // dont know but later think about it

	appWithAction  Action_Temporary_Store_Flow;
	appWithAction  Action_Temporary_Store_Schedule;

	bool RefreshSchedule = false;
	bool flowFlag = false;

	jStr jexecutionOKReturnRawData = "";

	bool UpgradeDevice = false;
	jStr firmwareUpdateHttp = "";

	int funcReturnValue  = -1;
	bool RGBFLAg = false;

	int ActionMacro = -1;
	int cmdType = -1;

	File myfile;
	char text[200];

	cJSON *reply 		= NULL;
	cJSON *dataReply 	= NULL;
	cJSON *storedAction = NULL;

	jStr _str_receivedJson;
	jStr _reply_json;

	jStr favoFile = "/favoFile";
	jsonNested objNestedJsonToReply;

	typedef int (Jfunctionality::* nptr_func) ( cJSON* );
	nptr_func funcPtrList[5];

	struct brightnessInParameter{
		int GPIO_PIN;
	};
	struct brightnessOutParameter{
		int Intensity;
	};


	struct HMD_TEMPInParameter{
		int GPIO_PIN;
	};
	struct HMD_TEMPOutParameter{
		float HMD;
		float TEMP;
	};

	struct temperatureInParameter{
		int GPIO_PIN;
	};
	struct temperatureOutParameter{
		float Celcius;
	};

	struct pwrInParameter{
		uint8_t address;
	};
	struct pwrOutParameter{
		float RMS;
		float unit;
	};


//funtions 1 in & out
	struct funtion1in{
		int x;
		int y;
	};
	struct funtion1out{
		int x;
		int y;
	};

	union jinParameters{
		funtion1in f1in;
		brightnessInParameter bIP;
		temperatureInParameter tIP;
		HMD_TEMPInParameter htIP;
		pwrInParameter pIP;
	};
	union joutParameters{
		funtion1out f1out;
		brightnessOutParameter bOP;
		temperatureOutParameter tOP;
		HMD_TEMPOutParameter htOP;
		pwrOutParameter pOP;
	};

	typedef joutParameters (Jfunctionality::* jeventFunc) ( jinParameters );
	std::map<jStr, jeventFunc> eventHandlingFunctions; 	//map of string to functions

	struct jeventTasks{
		int interval;
		std::vector<jeventFunc> getFuntions;
		std::vector<jeventFunc> executeFuntions;
		jStr conditionsToEvaluate;
	};

	std::vector<jeventTasks> jTask;

	joutParameters funtion1(jinParameters myj){
		joutParameters jout;
		jout.f1out.x = 5;
		jout.f1out.y = 10;
		return jout;
	}

	joutParameters get_Brightness(jinParameters myj){
		joutParameters jout;
		jout.bOP.Intensity = 25;
		return jout;
	}

	joutParameters get_Temperature(jinParameters myj){
		joutParameters jout;

		//Get Temperature Data Code Write here

		jout.tOP.Celcius = 27.87;	//Garbage value
		return jout;
	}

	joutParameters get_HMD_TEMP(jinParameters myj){
		joutParameters jout;

		//Get Humidity & Temperature Data Code Write here

		jout.htOP.HMD = 24.69;	//Garbage value
		jout.htOP.TEMP = 28.4;
		return jout;
	}

	joutParameters get_PWR(jinParameters myj){
		joutParameters jout;

		//I2C call and get Power Data
		jout.pOP.RMS = 24.69;
		jout.pOP.unit = 134;
		return jout;
	}

	/*
	void loadingEventFromFile(){
		INFO("");
		cJSON* abc = readFileReturnJSON("/taskList");

		int lp = cJSON_GetArraySize(abc);
		for (int lp1 = 0; lp1 < lp; lp1++){
			jStr s = cJSON_GetArrayItem(abc, lp1)->valuestring;
			Serial.println(s sss);

			jeventTasks tempTask;
			cJSON* tempTaskJSON = readFileReturnJSON("/taskList");

			tempTask.interval 				= jTool.getInt(tempTask, "interval");
			tempTask.conditionsToEvaluate	= jTool.getStr(tempTask, "conditions");

		//getDataFuntions Loading
			cJSON *funtions = cJSON_GetObjectItem(tempTask, "getDataFuntions");
			for (cJSON* function = funtions->child; function; function = function->next){
				printJSON(function);
//				temp.getFuntions.push_back();
			}

		//executeFuntions Loading
			cJSON *funtions = cJSON_GetObjectItem(tempTask, "getDataFuntions");
			for (cJSON* function = funtions->child; function; function = function->next){
				printJSON(function);
//				temp.executeFuntions.push_back()
			}

			cJSON_Delete(tempTask);

		}

		cJSON_Delete(abc);
	}*/

	void StoreValueToMemory(jStr variableName, jStr varibaleValue){
		jTool.retValue["variableName"] = varibaleValue;
	}




	char* EnumPostmanFunction(jStr receChar);
	char* EnumPostmanFunction(cJSON *tempJSON);

	int onDevice(cJSON* ptrjarusJSON);
	int get(cJSON* ptrjarusJSON);
	int set(cJSON* ptrjarusJSON);
	int stop(cJSON* ptrjarusJSON);
	int run(cJSON* ptrjarusJSON);



	int doesTimePassed(jStr AlarmTime);
	void saveCurrentTime(time_t _currentTime = 0);
	void savoFlowStatusToFile (int count, jStr flowID);
	void setAlarmAfterSec(int seconds);
	void setAlarmAfterSec1(jStr strTime);

	jStr printDateTime(time_t t);
	jStr printDateTime(struct tm* temp);

	void jFlowHandler();
	void jScheduleHandler();



	cJSON* Get_Appliance_Status(int x);

	inline time_t getRtcTime(){
	    return RTC.get();
	}

	inline time_t getSysTime(){
	    return now();
	}

	inline cJSON* readFileReturnJSON(jStr fileName){
//		showln(fileName sss);
		jStr fileContain = jTool.jreadFile(fileName);
		if (fileContain.empty()){
			show("readFileReturnJSON Empty\t: ");
			showln (fileContain sss);
		}
		cJSON* root = cJSON_Parse(fileContain sss);
		if (!root) {
			show("readFileReturnJSON Deseril error\t: ");
			showln (fileContain sss);
			root = cJSON_CreateObject();
		}
		return root;
	}

	inline jStr printJSON(cJSON* json){
		char * jtext = cJSON_PrintUnformatted(json);
		jStr str = jStr (jtext);
		free (jtext);
		return str;
	}

	inline void jsonToFile(cJSON* root, jStr fileName){
		char *rendered = cJSON_PrintUnformatted(root);
		jTool.jwriteFile(fileName, rendered);
		free (rendered);
	}

	int flowInsert(jStr Appliace, cJSON* tpjson){
		#undef showlog
		#define showlog 0

		#undef showln
		#undef show
		#if showlog==1
			#define showln(x) 	Serial.println(x);
			#define show(x)		Serial.print(x);
		#else
			#define showln(x) 	;
			#define show(x)		;
		#endif

		cJSON* json = readFileReturnJSON (jTool.jFlowStatusFile);
		showln ("old jFlowStatusFile\t: " + String (printJSON(json) sss) );

		cJSON* abc = cJSON_GetObjectItem(json, Appliace sss);

		if (abc){
			showln("Not null");
			cJSON_ReplaceItemInObject(json, Appliace sss, tpjson);
		}else {
			showln("Null");
			cJSON_AddItemToObject(json, Appliace sss, tpjson);
		}

		int totalPostionSize = 0;
		for (cJSON *expe = json->child; expe; expe = expe->next) {
			totalPostionSize++;
		}showln("totalPostionSize\t: " + String (totalPostionSize) );

		showln ("updated jFlowStatusFile\t: " + String (printJSON(json) sss) );

		jsonToFile(json, jTool.jFlowStatusFile);
		cJSON_Delete(json);
		return totalPostionSize;

	}

	void flowDelete(jStr key){
		#undef showlog
		#define showlog 1

		#undef showln
		#undef show
		#if showlog==1
			#define showln(x) 	Serial.println(x);
			#define show(x)		Serial.print(x);
		#else
			#define showln(x) 	;
			#define show(x)		;
		#endif

		INFO("");

		cJSON* json = readFileReturnJSON (jTool.jFlowStatusFile);
		if (!json){
			showln ("JOSN not proper");
			return ;
		}

		cJSON_DeleteItemFromObject(json, key sss);
		jsonToFile(json, jTool.jFlowStatusFile);
		printJSON(json);
		cJSON_Delete(json);
	}



};

#endif /* MAIN_JFUNCTIONALITY_H_ */

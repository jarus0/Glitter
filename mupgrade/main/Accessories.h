/*
 * Accessories.h
 *
 *  Created on: 17-Sep-2020
 *      Author: suraj
 */

#ifndef MAIN_ACCESSORIES_H_
#define MAIN_ACCESSORIES_H_


enum State {
	None,
	Root,
	Node
};

#define jp(data) printf("%s %d\r\n", #data, data);

#define sss .c_str()

#include <Arduino.h>
#include <iostream>
#include <stdio.h>

#include <iterator>
#include <unordered_map>
#include <map>
#include <vector>

typedef std::string jStr;
typedef std::vector<uint16_t> IRContainer;
typedef std::map<jStr, jStr> jsonPair;
typedef std::vector<jStr> jsonNested;

#include <mutex>

#define INFO(MSG) objJarusAccessories.info(__FILE__, (String (__LINE__)).c_str(), __func__, MSG );
#define sINFO(MSG) info(__FILE__, (String (__LINE__)).c_str(), __func__, MSG );
//__DATE__, __TIME__ , __VA_ARGS__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "FS.h"
#include "SPIFFS.h"

// future modification needed
#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>      // http://arduiniana.org/libraries/streaming/
extern DS3232RTC RTC;

#include <Esp.h>
#include <sstream>
#include "cJSON.h"

#define JJLOG Serial

#define show(someMessage) 	Serial.print(someMessage)
#define showln(someMessage) 	Serial.println(someMessage)

#define showf(someMessage) 		Serial.print(F(someMessage))
#define showlnf(someMessage) 	Serial.println(F(someMessage))

#define INFO(MSG) objJarusAccessories.info(__FILE__, (String (__LINE__)).c_str(), __func__, MSG );


struct bdh22{
	float humu;
	float tempe;
};

struct appWithAction{
	jStr appliances;
	jStr action;
};

class Accessories {
public:
	Accessories();
	virtual ~Accessories();

	bdh22 something;
	int howManyAppliences = 2;		//given by BLE APP

	uint8_t deviceMacID[6];
	jStr deviceMacIDStr;

	float humidityValue = 0;
	float temperatureValue = 0;

	float minTemperature;
	float maxTemperature;
	float pivotTemperature;


	jStr flowCurrentAction = "";


	jStr orgId = "";
	jStr zoneId = "";

	int prot;
	int model;


	bool mqttStatus = false;
	bool actionDecoderFlag = false;
	appWithAction indirectActionDecoder;

	//short functions

	template<typename T> void someFuntion(T object){
//		if (!debug) return;
		Serial.println(object);
	}

	jStr arrayToStrMac(uint8_t *macAdrr){
		jStr macStr = "";

		for (int jloop = 0; jloop < 6; jloop++){
			macStr += String (macAdrr[jloop], HEX) .c_str ();
			if (jloop != 5) macStr +=":";
		}
		return macStr;
	}

	std::vector<jStr> SplitStringBy(char SplitBy, jStr name){
		std::vector<jStr> fileNames;
		std::stringstream ss(name);
		while (ss.good()) {
			jStr substr;
			getline(ss, substr, ',');
			fileNames.push_back(substr);
		}
		return fileNames;
	}

	std::map<jStr, jStr> retValue; 	//map of string to functions

	inline bool doRecursive(jStr str){
		int counter = 0;
		for (auto x : str){
			if (x == '(') counter++;
			if (counter > 1) break;
		}
		return counter > 1 ? true : false;
	}

	bool invert(jStr str){
		sINFO(str sss);
		if ( doRecursive(str) ){
			showln("need to recursive");
			return eval(str)[0];
		}else{
			showln("Evaluating");
			showln(str sss);
		}
		return false;
	}
	bool isjAnd(jStr str){
		sINFO(str sss);
		if ( doRecursive(str) ){
			showln("need to recursive");
			return eval(str)[0];
		}else{
			showln("Evaluating");
			showln(str sss);
		}
		return false;
	}
	bool OrOrOr(jStr str){
		sINFO(str sss);
		if ( doRecursive(str) ){
			showln("need to recursive");
			return eval(str)[0];
		}else{
			showln("Evaluating");
			showln(str sss);
		}
		return false;
	}
	bool isMore(jStr str){
		sINFO(str sss);
		if ( doRecursive(str) ){
			showln("need to recursive");
			return eval(str)[0];
		}else{
			showln("Evaluating");
			showln(str sss);
			std::vector<jStr> abcx = SplitStringBy(',', str);
			int x = stoi(retValue[abcx[0]]);
			int y = stoi(abcx[1]);
			showln(x);
			showln(y);
			bool xxxx = x > y ? true : false;
//			showln("Returning\t: " + String(xxxx));
			return xxxx;
		}
		return false;
	}

	bool iEqual(jStr str){
		sINFO(str sss);
		if ( doRecursive(str) ){
			showln("need to recursive");
			std::vector<bool> a = eval(str);
			bool temp = a[0];
			for (auto x : a){
				if (temp != x) return false;
			}
			return true;
		}else{
			showln("Evaluating");
			showln(str sss);
		}
		return false;
	}
	bool adding(jStr str){
		sINFO(str sss);
		if ( doRecursive(str) ){
			showln("need to recursive");
			return eval(str)[0];
		}else{
			showln("Evaluating");
			showln(str sss);
		}
		return false;
	}

	bool isLess(jStr str){
		sINFO(str sss);
		if ( doRecursive(str) ){
			showln("need to recursive");
			return eval(str)[0];
		}else{
			showln("Evaluating");
			showln(str sss);
			std::vector<jStr> abcx = SplitStringBy(',', str);
			int x = stoi(retValue[abcx[0]]);
			int y = stoi(abcx[1]);
			showln(x);
			showln(y);
			bool xxxx = x < y ? true : false;
//			showln("Returning\t: " + String(xxxx));
			return xxxx;
		}
		return false;
	}

	typedef bool (Accessories::* funPtr) ( jStr str );
	std::map<jStr, funPtr> funList; 	//map of string to functions

	std::vector<jStr> subFormulaList(jStr str){
		sINFO("")
//		showln("1");
//		showln(str sss);
		int firstMark=0;
		std::vector<jStr> something;
		while (!str.empty()){
			int o = 0; int postions = 0;
			for (auto xb : str){
				postions++;
				if (xb == '(') o++;
				if (xb == ')') {
					o--;
					if (o == 0){
						break;
					}
				}
			}
//			showln(postions);
			jStr str1 = str.substr(0, postions);
			if (firstMark++ != 0) str1.erase(str1.begin());
			something.push_back(str1);
			showln(str1 sss);
			str.erase(str.begin(), str.begin() + postions);
		}

//		showln("Strings showing");for_each(something.begin(), something.end(), [](jStr x){showln(x sss);});	showln("Strings showing over");
		return something;
	}

	std::vector<bool> eval(jStr str){
		sINFO("")
//	bool eval(jStr str){
		static int y = 0;
		showln("");
		showln (String(y++) + "\t\t: " + String(str sss) );

		std::vector<bool> returnValue;
		std::vector<jStr> jjabc = subFormulaList(str);
		showln("subFormula Size\t: " + String(jjabc.size()) );
		showln("Starting first formula");
		for (auto x : jjabc){
			showln(x sss);
			jStr joperator = x.substr(0, 6);
			jStr str2 = x.substr(joperator.size() +1, x.length()-joperator.size()-2);
			showln("operator\t: " + String(joperator sss));
			showln("recursive\t: " + String(str2 sss));
			bool something = (this->*funList[ joperator ]) (str2);
			showln(String(joperator sss) + " Result \t: " + String(something) );
			returnValue.push_back(something);
//			return something;
		}
		return returnValue;
//		return false;
	}

	std::mutex i2cMutex;
	std::mutex GlobalMutexLU11;           // mutex for critical section

	jStr currentUserToken = "jarus";		// if this is blank than problem create
	jStr currentUserTokenA[3] = {"", "", ""};		// if this is blank than problem create
	bool rgbBhutickFalg = true;
	jsonPair colorMap;

	String str_chipID; // find through constructor according to device
	jStr sstr_chipID;
	jStr DeviceName;

	jStr ffirmwareVer = "v0.4.0.5";
	jStr fhardwareVer = "v0.8.0.5";

	bool restartBit = false;

	jStr fileConfig	= "/config.json";
	jStr fileInfo = "/information";
	jStr recentFlow = "/recentFlow";
	jStr JcurrentFlow = "/JcurrentFlow";

	jStr jFlowStatusFile = "/jFlowStatusFile";

	jStr FlowTimeStoring = "/FlowTimeStoring";

	jStr lastCmd1 = "/lastCmd1";
	jStr lastCmdTime1 = "/lastCmdTime1";
	jStr lastCmd2 = "/lastCmd2";
	jStr lastCmdTime2 = "/lastCmdTime2";

	jStr pingTimeFile = "/pingTime";

	int pingTime;


	jStr jScheduleStatusFile = "/jScheduleFile";

	//future modification needed
//	int randNumber = 0;
//	int randomArray[4] = {};

	bool savingIRflag = false;

	void jprint(jStr sText);	//change with template <typename T>	void jPrint(T toPri);

	String getCurrentFileName(String FullFileAddress);
	void info(jStr StrFile, jStr Strline, jStr Strfunc, jStr StrMsg);



	void strTomac(std::string const& strMac, unsigned char *mac){
		int last = -1;
		int rc = sscanf(
				strMac.c_str(),
				"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n",
				mac + 0, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5,
				&last
			);
		if(rc != 6 || strMac.size() != last){
			//throw std::runtime_error("invalid mac address format " + strMac);
		}
	}

	template<typename T> std::string toString(const T& value);


//private:
	String getValueBySplit(String data, char separator, int index);

    static int StrToHex(const char str[]) {
	  return (int) strtol(str, 0, 16);
	}
	void macToStr(){

	}

// File operations
	void block1();
	void unblock1();

	std::mutex multiFileAccessProtection;
	void jwriteFile(jStr path, jStr message);

	void jappendFile(jStr path, jStr message);

	void jsaveVectorToFile(jStr path, std::vector<uint16_t> *toSaveVector);
	std::vector<uint16_t> jgetVectorFromFile(jStr path);

	std::string jreadFile(jStr path);
	void jrenameFile(jStr path1, jStr path2);

	void jdeleteFile(jStr path);
	void jdeleteFolder(jStr path);
	void jformat();
	jsonNested jlistDir(jStr dirname, uint8_t levels);
	void jtestFileIO(jStr path);
/*
	template<typename T> inline void saveStruct(jStr fileName, T &jstruct){
		File file = SPIFFS.open(fileName sss, FILE_WRITE);
		if(file && !file.isDirectory()){
			file.write((uint8_t*)&jstruct, sizeof(T));
		}
		file.close();
	}

	template<typename T> T loadStruct(jStr fileName){
		T jstruct;

	    File file = SPIFFS.open(fileName sss);
		if(file && !file.isDirectory()){
		    file.read((uint8_t*)&jstruct, sizeof(T));
		    file.close();
		}
		return jstruct;
	}
*/

//json Operations
	inline jStr getStr(cJSON *root, const char key[]){

		cJSON *tpJSON = cJSON_GetObjectItem(root, key);
		return tpJSON ? tpJSON->valuestring : "defaultValue";

	}
	inline int getInt(cJSON *root, const char key[]){

		cJSON *tpJSON = cJSON_GetObjectItem(root, key);
		return tpJSON ? tpJSON->valueint : -1 ;

	}

	inline double getDouble(cJSON *root, const char key[]){

		cJSON *tpJSON = cJSON_GetObjectItem(root, key);
		return tpJSON ? tpJSON->valuedouble: -1 ;

	}

};

#endif /* MAIN_ACCESSORIES_H_ */

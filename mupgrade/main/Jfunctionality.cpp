/*
 * Jfunctionality.cpp
 *
 *  Created on: 04-Mar-2020
 *      Author: suraj
 */

#include "Jfunctionality.h"


Jfunctionality::Jfunctionality() {

	INFO("");
	funcPtrList[0] = &Jfunctionality::onDevice;		//[012, restart, format, crash]
	funcPtrList[1] = &Jfunctionality::get;
	funcPtrList[2] = &Jfunctionality::set;
	funcPtrList[3] = &Jfunctionality::stop;
	funcPtrList[4] = &Jfunctionality::run;

//	nFunCollection["updateFirmware"]= &Jfunctionality::func_updateFirmware;
//	nFunCollection["ping"] 			= &Jfunctionality::nfunc_ping;			// cheacked
//	nFunCollection["quenuTest"] 			= &Jfunctionality::func_quenuTest;			// cheacked

	eventHandlingFunctions["funtion1"] 			= &Jfunctionality::funtion1;
	eventHandlingFunctions["get_Brightness"] 	= &Jfunctionality::get_Brightness;
	eventHandlingFunctions["get_Temperature"] 	= &Jfunctionality::get_Temperature;
	eventHandlingFunctions["get_HMD_TEMP"] 		= &Jfunctionality::get_HMD_TEMP;


	jeventTasks abc;
	abc.interval = 7;
	abc.conditionsToEvaluate = "hi how are you";
	jTask.push_back(abc);

}

Jfunctionality::~Jfunctionality()
{
	// TODO Auto-generated destructor stub
}

int Jfunctionality::onDevice(cJSON* ptrjarusJSON) {
	INFO ("msg")

//	::jarus2();


	cJSON *something= cJSON_GetObjectItem(ptrjarusJSON, "data");
	switch (cmdType)
	{
		case 0:{
			break;
		}
	    case 1:{
	    	showlnf ("Restart");
	    	jTool.restartBit = true;	// in loop chaekc and excute
	    	cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
	        break;
	    }
	    case 2:{
	    	showlnf ("Format");
	    	jTool.jformat();
	    	cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
	    	break;
	    }
	    case 3:{
	    	showlnf ("UpgradeDevice");
	    	firmwareUpdateHttp = jTool.getStr(something, "uri");
	    	showln (firmwareUpdateHttp sss);
	    	UpgradeDevice = true;
	    	break;
	    }
	    default:{
	    	showlnf ("Unknown CMD");
	    	cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
	    }
	}
	return jexecutionOK;
}
int Jfunctionality::get(cJSON* ptrjarusJSON) {
	INFO ("msg")

	switch (cmdType)
	{
	    case 0:{
	    	break;
	    }
	    case 1:{
	    	time_t currentTime = getRtcTime() - 19800;
	 		showln ("Time\t: " + String (printDateTime( currentTime ) sss) );
	    	cJSON_AddItemToObject(dataReply, "rtcTime", cJSON_CreateNumber( currentTime  ) );
			break;
	    }
	    case 2:{
//	    	showln ("AC status Returning");
//			cJSON *arrya2 = cJSON_CreateArray();
//			cJSON_AddItemToArray(arrya2 , cJSON_CreateString("on/off"));
//			cJSON_AddItemToArray(arrya2 , cJSON_CreateNumber(32.45));
//			cJSON_AddItemToObject(reply, "data", arrya2 );
			break;
	    }
	    case 3:{
	    	showlnf ("Files List");
			cJSON *files = cJSON_CreateArray();
			jsonNested jn = jTool.jlistDir("/", 2);
			for (auto j : jn){
				j.erase(j.begin());
				cJSON_AddItemToArray(files, cJSON_CreateString(j sss));

				showf("Adding\t: ");
				showln(j sss);
			}
//			cJSON* filesStr = cJSON_CreateObject();
			cJSON_AddItemToObject(dataReply, "allFiles", files );
			break;
	    }
	    case 4:{

	    	cJSON* files = cJSON_CreateObject();

	    	cJSON* data = cJSON_GetObjectItem(ptrjarusJSON, "data");
	    	cJSON* getFiles = cJSON_GetObjectItem(data, "getFiles");
	    	printJSON(getFiles);


			int arraySize = cJSON_GetArraySize(getFiles);
			for (int lp1 = 0; lp1 < arraySize; lp1++){

				jStr fileName = cJSON_GetArrayItem(getFiles, lp1)->valuestring;
				jStr contain = jTool.jreadFile("/"+fileName);

				showln (fileName sss);
				showln (contain sss);
				cJSON_AddItemToObject(files, fileName sss, cJSON_Parse (contain sss) );
			}
			cJSON_AddItemToObject(dataReply, "filesContain", files );

	    	break;
	    }
	    case 6:{

	    	cJSON* files = cJSON_CreateObject();

	    	cJSON* data = cJSON_GetObjectItem(ptrjarusJSON, "data");
	    	cJSON* getFiles = cJSON_GetObjectItem(data, "getFiles");
	    	printJSON(getFiles);


			int arraySize = cJSON_GetArraySize(getFiles);
			for (int lp1 = 0; lp1 < arraySize; lp1++){

				jStr fileName = cJSON_GetArrayItem(getFiles, lp1)->valuestring;
//				jStr contain = jTool.jreadFile("/"+fileName);
				jStr contain = jTool.retValue["/"+fileName];

				showln (fileName sss);
				showln (contain sss);
				cJSON_AddItemToObject(files, fileName sss, cJSON_Parse (contain sss) );
			}
			cJSON_AddItemToObject(dataReply, "filesContain", files );

	    	break;
	    }
	    case 5:{
	    	jStr s1 = jTool.jreadFile( jTool.jScheduleStatusFile );
	    	jStr s2 = jTool.jreadFile( jTool.jFlowStatusFile );
	    	showln(s1 sss);
	    	showln(s2 sss);

	    	cJSON* json1 = cJSON_Parse(s1 sss);
	    	cJSON* json2 = cJSON_Parse(s2 sss);
	    	cJSON_AddItemToObject(dataReply, "schedule", json1 );
	    	cJSON_AddItemToObject(dataReply, "flow", json2 );

	    	break;
	    }


	    default:{
	    	showlnf ("undefine cmd");
	    }
	}
	return jexecutionOK;
}
int Jfunctionality::set(cJSON* ptrjarusJSON) {
//	jTool.debug = true;
	INFO ("msg")

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

#define timeSet 0
#define fileSave 1

		switch (cmdType)
		{

		    case timeSet:{

		    	showln( "Received\t: " +  String (printJSON(ptrjarusJSON) sss)  );
		    	cJSON *configKeys = cJSON_GetObjectItem(ptrjarusJSON, "data");

		    	for (cJSON* elements = configKeys->child; elements; elements = elements->next){
		    		jStr configKey = elements->string;
		    		bool something = true;
		    		if (configKey.find("pingTime") != std::string::npos){
		    			jTool.pingTime = jTool.getInt(configKeys, configKey sss);
		    			showln ("PING_INTERVAL\t: " + String (jTool.pingTime) + "Sec");
		    			jTool.jwriteFile(
		    					jTool.pingTimeFile sss,
								String(jTool.pingTime) sss );
		    			cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
		    			something = false;
		    		}
		    		if (configKey.find("rtcTime") != std::string::npos){

		    			long epochInt = 19800 + jTool.getInt(configKeys, configKey sss);
						jTool.i2cMutex.lock();
							RTC.set(epochInt); delay(20);	// chanege IST to GMT+5:30
							setTime(epochInt);
						jTool.i2cMutex.unlock();

						showln("TimeChanging\t: "+
								String ( printDateTime(getRtcTime()) sss ) );
						cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
						something = false;
		    		}
		    		if(something){
		    			INFO( "keyNotFound" + configKey);
						cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( -1 ) );
		    		}
		    	}

				break;
		    }
		    case fileSave:{

		    	cJSON *data = cJSON_GetObjectItem(ptrjarusJSON, "data");

		    	for (cJSON* something = data->child; something; something = something->next){
//		    		char* rendered4 = cJSON_PrintUnformatted(something);
//		    		INFO(rendered4);
			    	cJSON *scheduleFileName = something;

			    	char* rendered = cJSON_PrintUnformatted(scheduleFileName);
			    	jStr fname = scheduleFileName->string;

			    	std::vector<jStr> fileNames = jTool.SplitStringBy(',', fname);

			        for (auto vstr : fileNames){
				    	jStr fileName = '/' + vstr;
				    	jTool.jwriteFile(
				    			fileName,
								rendered);
				    	showln( "File Saving\t: " + String(fileName sss) );
				    	showln( "File Contain\t: " + String(rendered) );

			        }
			    	free (rendered);

		    	}

		    	cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
		    	int uBytes = SPIFFS.totalBytes() - SPIFFS.usedBytes();
		    	cJSON_AddItemToObject(dataReply, "spaceLeft", cJSON_CreateNumber( uBytes ) );
				break;
		    }
		    case 3:{
		    	INFO("");
		    	cJSON *data = cJSON_GetObjectItem(ptrjarusJSON, "data");
		    	for (cJSON* something = data->child; something; something = something->next){
//		    		char* rendered4 = cJSON_PrintUnformatted(something);
//		    		INFO(rendered4);
			    	cJSON *scheduleFileName = something;

			    	char* rendered = cJSON_PrintUnformatted(scheduleFileName);
			    	jStr fname = scheduleFileName->string;

			    	std::vector<jStr> fileNames = jTool.SplitStringBy(',', fname);

			        for (auto vstr : fileNames){
				    	jStr fileName = '/' + vstr;
				    	jTool.retValue[fileName] = rendered;
//				    	jTool.jwriteFile(
//				    			fileName,
//								rendered);
				    	showln( "Memory Address\t: " + String(fileName sss) );
				    	showln( "Memory Contain\t: " + String(rendered) );

			        }
			    	free (rendered);

		    	}

		    	cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
		    	int uBytes = SPIFFS.totalBytes() - SPIFFS.usedBytes();
		    	cJSON_AddItemToObject(dataReply, "spaceLeft", cJSON_CreateNumber( uBytes ) );
				break;
		    }

		    case 2:{
//		    	INFO("");
		    	cJSON* data = cJSON_GetObjectItem(ptrjarusJSON, "data");
		    	cJSON* files = cJSON_GetObjectItem(data, "deleteFiles");
		    	printJSON(files);
				int x = cJSON_GetArraySize(files);

				for (int lp1 = 0; lp1 < x; lp1++){
					jStr fileName = cJSON_GetArrayItem(files, lp1)->valuestring;
					showf ("Deleting File\t: ");
					showln (fileName sss);
					jTool.jdeleteFile( '/' + fileName );
				}

		    	break;
		    }
		    default:{
		    	showlnf ("Undefined cmd");
		    }
		}

	return jexecutionOK;

#undef timeSet
#undef fileSave
}
int Jfunctionality::stop(cJSON* ptrjarusJSON) {
	INFO ("")

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

	switch (cmdType)
	{
	    case 0:{
//	    	nfunc_stopFlow();
			break;
	    }
	    case 1:{

	    	cJSON* keys = cJSON_GetObjectItem(ptrjarusJSON, "data");
	    	showln("Flow & Schedule Stopping");

	    	for ( cJSON *key = keys->child; key; key = key->next ) { //flow & schduel
	    		showln("");
	    		jStr flowOrSc = jStr (key->string);

	    		showln(
					String (flowOrSc sss)
					+ "\t: "
					+ String (printJSON(key) sss)
				);

	    		if (flowOrSc == "flows"){

	    			cJSON* jsonFlow = readFileReturnJSON (jTool.jFlowStatusFile);
	    			int x = cJSON_GetArraySize(key);
					for (int lp1 = 0; lp1 < x; lp1++){
						jStr keyStr = cJSON_GetArrayItem(key, lp1)->valuestring;
						showln("Deleting\t: " + String (keyStr sss) );
						cJSON_DeleteItemFromObject(jsonFlow, keyStr sss);
					}
					jsonToFile(jsonFlow, jTool.jFlowStatusFile);
					showln ("jsonFlow\t: "+String ( printJSON(jsonFlow) sss));
					cJSON_Delete(jsonFlow);

					showln("Elements\t: " + String (x));
					if (x > 0){
					    RTC.alarmInterrupt(ALARM_1, false);
						jFlowHandler();
					}

	    		}

	    		if (flowOrSc == "schedules"){
	    			cJSON* jsonSchedules = readFileReturnJSON (jTool.jScheduleStatusFile);

	    			int x = cJSON_GetArraySize(key);
					for (int lp1 = 0; lp1 < x; lp1++){
						jStr keyStr = cJSON_GetArrayItem(key, lp1)->valuestring;
						showln("Deleting\t: " + String (keyStr sss) );
						cJSON_DeleteItemFromObject(jsonSchedules, keyStr sss);
					}
					jsonToFile(jsonSchedules, jTool.jScheduleStatusFile);
					showln ("jsonFlow\t: "+String ( printJSON(jsonSchedules) sss));
					cJSON_Delete(jsonSchedules);

					showln("Elements\t: " + String (x));
					if (x > 0){
						RTC.alarmInterrupt(ALARM_2, false);
						jScheduleHandler();
					}
	    		}

	    	}

	    	break;
	    }
	    default:{
	    	showlnf ("Undefined cmd");
	    }
	}
	return jexecutionOK;
}
int Jfunctionality::run(cJSON* ptrjarusJSON) {
	INFO ("")
#define RUN 1
#define ScheduleRefresh 0

	switch (cmdType)
	{
	    case ScheduleRefresh:{
	    	showlnf("RefreshSchedule");
	    	RefreshSchedule = true;
	    	cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
			break;
	    }
	    case RUN:{

	    	bool shouldRefrashFlow = false;
	    	cJSON* abc = cJSON_GetObjectItem(ptrjarusJSON, "data");
			showln (String (printJSON(abc) sss) );
			showln("key value pair");
			for (cJSON *expe = abc->child; expe; expe = expe->next) {
				showln();
				showln ( String (expe->string) +
						"\t: "+
						String (printJSON(expe) sss) );

				jStr Appliance 	= expe->string;
				if (cJSON_IsArray (expe)) {
					flowInsert(Appliance, cJSON_Duplicate(expe, true));
					shouldRefrashFlow |= true;
				}else {
					jStr str = expe->valuestring;
					showln(expe->valuestring);
					appWithAction somehting;
					somehting.appliances	= Appliance;
					somehting.action 		= str;
					actionDecoder(somehting);
				}
			}
			cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( 1 ) );
			if (shouldRefrashFlow) jFlowHandler();

	    	break;
	    }
	    case 2:{
	    	showln("Ok here lets refresh Flow");
	    	jFlowHandler();
	    	break;
	    }


	    default:{
	    	showln ("undefine cmd");
//	    	cJSON* json = objJfunctionality.readFileReturnJSON("/somefile");
//	    	objJfunctionality.printJSON(json);
//	    	cJSON* json = readFileReturnJSON("/somefile");
//	    	printJSON(json);

	    }
	}
	return jexecutionOK;

#undef buttonFire
#undef ScheduleRefresh

}

//int Jfunctionality::func_quenuTest(cJSON* ptrjarusJSON) {
//	INFO ( "in pin" );
//	showln ("waiting 3 second");
//	vTaskDelay(3000 / portTICK_RATE_MS);
//	showln ("wait over");
//	return jexecutionOK;
//}

//int Jfunctionality::nfunc_ping (cJSON* addrJson) {
//	INFO ("msg");
//	showln ("Not implemented");
//	return jexecutionOK;
//}

char* Jfunctionality::EnumPostmanFunction(jStr receChar)
{


//	INFO ("");
	reply 		= cJSON_CreateObject();
	dataReply 	= cJSON_CreateObject();

	cJSON_AddItemToObject(reply, "id", cJSON_CreateString(jTool.deviceMacIDStr sss) );

#ifdef jwebsocketSupport
	cJSON* mainServer = cJSON_Parse( receChar .data() );
	cJSON* dataServer = cJSON_GetObjectItem(mainServer, "data");
	cJSON* root = cJSON_GetObjectItem(dataServer, "message_details");
#endif

#ifdef jmqttSupport
	cJSON *root = cJSON_Parse( receChar .data() );
#endif

	if (root){
		//INFO ("");
		cJSON *cmdReply = cJSON_CreateArray();

		cJSON *cmd = cJSON_GetObjectItem(root, "cmd");
		if (cmd){
			ActionMacro = cJSON_GetArrayItem(cmd, 0)->valueint;
			cmdType		= cJSON_GetArrayItem(cmd, 1)->valueint;
			cJSON_AddItemToArray(cmdReply , cJSON_CreateNumber( ActionMacro ) );
			cJSON_AddItemToArray(cmdReply , cJSON_CreateNumber( cmdType	) );
			cJSON_AddItemToObject(reply, "cmd", cmdReply);

			funcReturnValue  =  (this->*funcPtrList[ ActionMacro ]) (root);
		}

	}else {
		showlnf ("JSON fail");
		showln(receChar sss);


		cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( -1 ) );
	}

	cJSON_AddItemToObject(reply, "data", dataReply );

	if (root)cJSON_Delete(root);

	char* rendered =
//			cJSON_Print(reply);
			cJSON_PrintUnformatted(reply);


	if(reply) cJSON_Delete(reply);
	return rendered;
}

char* Jfunctionality::EnumPostmanFunction(cJSON *tempJSON)
{
	INFO ("");
	jStr receChar = "suraj signh";


	reply 		= cJSON_CreateObject();
	dataReply 	= cJSON_CreateObject();

	cJSON_AddItemToObject(reply, "id", cJSON_CreateString(jTool.deviceMacIDStr sss) );


	cJSON *root;// = cJSON_Parse( receChar .data() );
	root = tempJSON;

	if (root){
		cJSON *cmdReply = cJSON_CreateArray();

		cJSON *cmd = cJSON_GetObjectItem(root, "cmd");
		if (cmd){
			ActionMacro = cJSON_GetArrayItem(cmd, 0)->valueint;
			cmdType		= cJSON_GetArrayItem(cmd, 1)->valueint;
			cJSON_AddItemToArray(cmdReply , cJSON_CreateNumber( ActionMacro ) );
			cJSON_AddItemToArray(cmdReply , cJSON_CreateNumber( cmdType	) );
			cJSON_AddItemToObject(reply, "cmd", cmdReply);

			funcReturnValue  =  (this->*funcPtrList[ ActionMacro ]) (root);
		}

	}else {
		showlnf ("JSON fail");
		showln(receChar sss);


		cJSON_AddItemToObject(dataReply, "ack", cJSON_CreateNumber( -1 ) );
	}

	cJSON_AddItemToObject(reply, "data", dataReply );

//	if (root)cJSON_Delete(root);

	char* rendered =
//			cJSON_Print(reply);
			cJSON_PrintUnformatted(reply);


	if(reply) cJSON_Delete(reply);
	return rendered;
}

//int Jfunctionality::nfunc_stopFlow (cJSON* ptrjarusJSON)
//{
//	flagForFLowExecution = false;
//	jTool.jwriteFile(jTool.FlowTimeStoring, "");
//	jTool.jwriteFile(jTool.JcurrentFlow, "");
//	RTC.squareWave(SQWAVE_NONE);
//	RTC.alarmInterrupt(ALARM_1, false);     //disable Alarm2
//	return jexecutionOK;
//}
//
//int Jfunctionality::nfunc_stopFlow ()
//{
//	showln ("Schedule Stopping");
//	flagForFLowExecution = false;
//	jTool.jwriteFile(jTool.FlowTimeStoring, "");
//	jTool.jwriteFile(jTool.JcurrentFlow, "");
//	setAlarmAfterSec1("00:00:10");
//	return jexecutionOK;
//}

void Jfunctionality::savoFlowStatusToFile (int count, jStr flowID)
{
	//INFO("msg");
	cJSON *tempJsonPair = cJSON_CreateObject();
	cJSON_AddItemToObject(tempJsonPair, "currentFlow", cJSON_CreateString( flowID sss ));
	cJSON_AddItemToObject(tempJsonPair, "counter", cJSON_CreateString( String (count).c_str() ));

	char* rendered = cJSON_PrintUnformatted( tempJsonPair );
	jTool.jwriteFile( jTool.JcurrentFlow, rendered );

	cJSON_Delete(tempJsonPair);
	free (rendered);

	if (!count)
	{
//		showlnf ("This is new Flow:Writing to File");
		flagForFLowExecution = true; // bad way : future task : make a funcion call here make it independented
	}

}

//void Jfunctionality::ScheduleHandler(){
//
//	INFO("");
////	return ;
////	jTool.debug = true;
//
//	jStr ScheduleStatusFile  = jTool.jreadFile( jTool.JcurrentFlow );
//	if (ScheduleStatusFile.empty()){
//		INFO("ScheduleStatusFile Problem");
//		return ;
//	}
////	INFO("");
//	cJSON *root = cJSON_Parse( ScheduleStatusFile sss );
//	if (!root) {
//		INFO ("ScheduleStatusFile Problem");
//		return;
//	}
////	INFO("");
//
////	char * accc1 = cJSON_PrintUnformatted(root);
////	showln (String (accc1) );
////	free (accc1);
//
//	jStr CurrentScheduleName = root->child->string;
//	int CurrentSchedulePostion = root->child->valueint;
//
////	INFO (CurrentScheduleName sss);
////	INFO (std::to_string (CurrentSchedulePostion));
//
//	cJSON_Delete(root);
//
//	jStr ScheduleList  = jTool.jreadFile( CurrentScheduleName );
//	if (ScheduleList.empty()) {
////		showln ("File not Found ending Schedule");
//		nfunc_stopFlow();
//		return;
//	}
//
//	cJSON* ScheduleListJson   = cJSON_Parse( ScheduleList sss );
//	if (!ScheduleListJson) {
//		INFO ("ScheduleListJson Problem");
//		return;
//	}
//
//	int count = 0;
//	cJSON *expe = ScheduleListJson->child;
//	for (; expe && (count < CurrentSchedulePostion) ; expe = expe->next, count++) {	}
//
////	Schedule last element
//	if (!expe) {
//		int count = 0;
//		cJSON *expe = ScheduleListJson->child;
//		for (; expe && (count < CurrentSchedulePostion - 1) ; expe = expe->next, count++) {}
//		cJSON* action1 = expe;
//		char * accc = cJSON_PrintUnformatted(action1);
//		showln ("ScheduleEnd, Last Action\t: " + String (accc) );
//		free (accc);
//
//		actionPerform(action1);
//
//		if(ScheduleListJson) cJSON_Delete(ScheduleListJson);
////		if(root) cJSON_Delete(root);
//		nfunc_stopFlow();
//		return;
//	}
//
//	cJSON* Action = expe;
////	char * accc = cJSON_PrintUnformatted(Action);
////	showln(accc);
////	free (accc);
//
//	jStr alarmTime = expe->string;
//	int returnTime = doesTimePassed(alarmTime);
//
//// time passed
//	if ( returnTime < 0 ) {
//
//		showln (String (returnTime) + " Sec\t: Next Schedule");
//
//		if (storedAction) cJSON_Delete( storedAction );
//		storedAction = cJSON_Duplicate(Action, true);
//
//		cJSON *tosave = cJSON_CreateObject();
//		cJSON_AddItemToObject(tosave, CurrentScheduleName sss, cJSON_CreateNumber(CurrentSchedulePostion + 1));
//		char *rendered1 = cJSON_PrintUnformatted(tosave);
//		jTool.jwriteFile( jTool.JcurrentFlow, rendered1 );
//		cJSON_Delete(tosave);
//		free (rendered1);
//		flagForFLowExecution = true; // bad way : future task : make a funcion call here make it independented
//	}
//// same to current time
//	else if ( returnTime < 10 ) {
//
//		char * accc = cJSON_PrintUnformatted(Action);
//		showln (String (returnTime) + " Sec\t: Current Action:- " + String (accc) );
//		free (accc);
//		actionPerform(Action);
//		expe = expe->next;
//		if (expe) {	// is this last schedule
//			setAlarmAfterSec1(expe->string);
//
//			cJSON *tosave = cJSON_CreateObject();
////			cJSON_AddItemToObject(tosave, root->child->string, cJSON_CreateNumber(root->child->valueint + 1));
//			cJSON_AddItemToObject(tosave, CurrentScheduleName sss, cJSON_CreateNumber(CurrentSchedulePostion + 1));
//			char *rendered1 = cJSON_PrintUnformatted(tosave);
//			jTool.jwriteFile( jTool.JcurrentFlow, rendered1 );
//			free (rendered1);
//			cJSON_Delete(tosave);
//
//		}
//		else {
//			nfunc_stopFlow();
//		}
//	}
//// future time
//	else if ( returnTime > 0 ) {
//
//		setAlarmAfterSec1(alarmTime);
////		INFO(std::to_string (sizeof(storedAction)));
//		if (storedAction && CurrentSchedulePostion > 0) {
//			INFO("");
//			char * accc = cJSON_PrintUnformatted(storedAction);
//			showln (String (returnTime) + " Sec\t: Missed Last Action : " + String ( accc) );
//			free (accc);
//			actionPerform(storedAction);
//
//		}
//	}
//
//	if(ScheduleListJson)cJSON_Delete(ScheduleListJson);
//
//}


int Jfunctionality::doesTimePassed(jStr AlarmTime) {

	String soi = AlarmTime sss;
	int hh = (jTool.getValueBySplit ( soi, ':', 0 ) ).toInt();
	int mm = (jTool.getValueBySplit ( soi, ':', 1 ) ).toInt();
	int ss = (jTool.getValueBySplit ( soi, ':', 2 ) ).toInt();

//	time_t cTime 		= RTC.get(); delay(20);
	time_t cTime 		= getSysTime();
	struct tm *stTim 	= localtime(&cTime);
	stTim->tm_hour 		= hh;
	stTim->tm_min 		= mm;
	stTim->tm_sec 		= ss;
	time_t t_of_day 	= mktime(stTim);

	Serial.print ("CurrentTime\t: ");	printDateTime(cTime);
	Serial.print ("AlarmTime\t: ");		printDateTime(t_of_day);

	return t_of_day - cTime;

}

#define setAlarmHHMSS setAlarmAfterSec1

void Jfunctionality::setAlarmHHMSS(jStr strTime) // time > hh:mm:ss
{

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

	if (strTime.empty()) strTime = "00:00:00";
	time_t s = getSysTime();
	struct tm *tm1 = localtime(&s);
	strptime(strTime sss, "%H:%M:%S", tm1);
//	struct tm tm1 = {0}; strptime(strTime sss, "%H:%M:%S", &tm1);
	showln ("NextScheduleTime\t: " + String (printDateTime( tm1) sss) );

	jTool.i2cMutex.lock();

	RTC.alarm(ALARM_2);
	RTC.alarmInterrupt(ALARM_2, false);
	RTC.squareWave(SQWAVE_NONE);

	RTC.setAlarm(
		ALM2_MATCH_HOURS,
		tm1->tm_sec,
		tm1->tm_min,
		tm1->tm_hour,
		0
	);

	RTC.alarmInterrupt(ALARM_2, true); delay(100);
	jTool.i2cMutex.unlock();

}

void Jfunctionality::setAlarmAfterSec(int seconds) // Delay
{
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

	if (seconds == INT_MAX){
		showlnf("INT_MAX : Skipping");
		return ;
	}

	if (seconds < 1){
		showlnf("Alarm second < 1");
		return ;
	}

	jTool.i2cMutex.lock();
	time_t _currentTime = getRtcTime();


	showln ("CurrentTime\t: " + String (printDateTime(_currentTime) sss) );

	time_t modifiedTime = getRtcTime() + seconds;
	struct tm *stTim 	= localtime(&modifiedTime);
	showln ("NextFlowTime\t: " + String (printDateTime(stTim ) sss) );

// Alarm 1
	// default values
	RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
	RTC.alarmInterrupt(ALARM_1, false);
	RTC.squareWave(SQWAVE_NONE);

	RTC.setAlarm(
		ALM1_MATCH_HOURS,
		stTim->tm_sec,
		stTim->tm_min,
		stTim->tm_hour,
		stTim->tm_yday
	);

	RTC.alarmInterrupt(ALARM_1, true);
	delay(100);
	jTool.i2cMutex.unlock();

}

jStr Jfunctionality::printDateTime(time_t jtime)
{
	struct tm *stTim = localtime(&jtime);
    char buf[80]; strftime( buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", stTim);
	return buf;
}

jStr Jfunctionality::printDateTime(struct tm* temp)
{
    char buf[80]; strftime( buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", temp);
	return buf;
}

cJSON* Jfunctionality::Get_Appliance_Status(int x){

//	Future task show ("applianceId will be given by BLE APP");

	cJSON *appliance = cJSON_CreateObject();

	if ( x== 0){
		cJSON_AddItemToObject(appliance, "applianceId", cJSON_CreateString( "401ab03a-a944-4edb-94b2-fda2770c4eab" ) );
	}else{
		cJSON_AddItemToObject(appliance, "applianceId", cJSON_CreateString( "6d8da24f-3e16-4fbe-9a03-ecbbb134aa3a" ) );
	}

	cJSON_AddItemToObject(appliance, "type", cJSON_CreateString( "AirCon" ) );
	cJSON_AddItemToObject(appliance, "status", cJSON_CreateNumber( 0 ) );

	//Last Action
	jStr s1;
	jStr s2;

	if ( x== 0){
		s1 = jTool.jreadFile(jTool.lastCmd1);
		s2 = jTool.jreadFile(jTool.lastCmdTime1);
	}else{
		s1 = jTool.jreadFile(jTool.lastCmd2);
		s2 = jTool.jreadFile(jTool.lastCmdTime2);
	}

	if (s1.empty()){
		cJSON_AddItemToObject(appliance, "lastCmd", cJSON_CreateString( "No lastCmd" ) );
	}else {
		cJSON_AddItemToObject(appliance, "lastCmd", cJSON_CreateString( s1 sss ) );
	}

	if (s2.empty()){
		cJSON_AddItemToObject(appliance, "lastCmdTime", cJSON_CreateString( "No lastCmdTime" ) );
	}else {
		cJSON_AddItemToObject(appliance, "lastCmdTime", cJSON_CreateString( s2 sss ) );
	}

	//operation
		cJSON *operation = cJSON_CreateObject();
		//type
		cJSON_AddItemToObject(operation, "type", cJSON_CreateNumber( 0 ) );
		//Schedule
		jStr ScheduleStatusFile  = jTool.jreadFile( jTool.JcurrentFlow );
		if (ScheduleStatusFile == "jarus" || ScheduleStatusFile == ""){
			show ("ScheduleStatusFile empty Didn't addig to ping");
			cJSON_AddItemToObject(operation, "name", cJSON_CreateString("No Schedule") );
		}else{
			cJSON* ScheduleJSON = cJSON_Parse( ScheduleStatusFile sss );
//				show ("Schedule Name");
//				show (ScheduleJSON->string);
//				show ();
			jStr schuldenamae = ScheduleJSON->child->string;
			schuldenamae.erase(schuldenamae.begin());
			cJSON_AddItemToObject(operation, "name", cJSON_CreateString(schuldenamae sss) );
			cJSON_Delete( ScheduleJSON );
		}

		cJSON_AddItemToObject(appliance, "operation", operation );

	return appliance;
}



void Jfunctionality::jFlowHandler(){

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

	INFO("");

	cJSON* jsonFlowStatusFile = readFileReturnJSON (jTool.jFlowStatusFile);
	if (!jsonFlowStatusFile){
		showlnf("json not proper");
		return ;
	}

	showln ("jsonFlowStatusFile\t: " + String ( printJSON(jsonFlowStatusFile) sss  ) );
	int nearestIntTimeInSec = INT_MAX;
	std::vector<jStr> deleteTHis;
	cJSON* nearestJSON = NULL;
	int postionIncrementer = 0;

	showln("Key, Value");
	for (cJSON *expe = jsonFlowStatusFile->child; expe; expe = expe->next) {
		showln("");

		jStr s2 = expe->string;
		showln(
			String (s2 sss)
			+ "\t: "
			+ String (printJSON(expe) sss)
		);

		jStr flow1 = cJSON_GetArrayItem(expe, 0)->valuestring;
		int currentPostion = cJSON_GetArrayItem(expe, 1)->valueint;
		int delayStore = cJSON_GetArrayItem(expe, 2)->valueint;

		cJSON* jsonFlow = readFileReturnJSON ('/' + flow1);

		showln(
			String (flow1 sss)
			+ "\t: "
			+ String (printJSON(jsonFlow) sss)
		);

		cJSON* delayObj = cJSON_GetObjectItem (jsonFlow, "delays");
		int xyz = cJSON_GetArraySize(delayObj);

		showln("TotalElmentes\t: " + String (xyz));

		if (currentPostion >= xyz) {
			showln("Flow completed deleting\t: " + String(s2 sss) );
			deleteTHis.push_back(s2);
			continue;
		}

		int currentDelay = cJSON_GetArrayItem(delayObj, currentPostion )->valueint;
		showln("flowPostions\t: " 	+	String ( currentPostion ) );
		showln("currentDelay\t: "	+ 	String (currentDelay) 	);
		showln("ConsiderDelay\t: " +	String (currentDelay - delayStore) );

		currentDelay -= delayStore;

		if ( currentDelay < nearestIntTimeInSec ){
			nearestIntTimeInSec = currentDelay;
			nearestJSON = expe;
			postionIncrementer = currentPostion;

			cJSON* actions = cJSON_GetObjectItem (jsonFlow, "IoTick_AirCon");
			Action_Temporary_Store_Flow.action = cJSON_GetArrayItem(actions, currentPostion )->valuestring;
			Action_Temporary_Store_Flow.appliances = s2;

			showln("Locally Save\t: "
					+ String (Action_Temporary_Store_Flow.appliances sss) + "|"
					+ String (Action_Temporary_Store_Flow.action sss));
		}
		cJSON_Delete (jsonFlow);
	}

	showln("");
	showln("nearestIntTimeInSec\t: " + 	String (nearestIntTimeInSec) );

	for (cJSON *expe = jsonFlowStatusFile->child; expe; expe = expe->next) {
		cJSON_ReplaceItemInArray(expe, 2,
			cJSON_CreateNumber (
					cJSON_GetArrayItem(expe, 2)->valueint + nearestIntTimeInSec
					)
			);
	}

	cJSON_ReplaceItemInArray(nearestJSON, 1, cJSON_CreateNumber(++postionIncrementer) );
	cJSON_ReplaceItemInArray(nearestJSON, 2, cJSON_CreateNumber(0) );

	for (auto ID : deleteTHis){
		showln("Deleting\t: " + String (ID sss) );
		cJSON_DeleteItemFromObject(jsonFlowStatusFile, ID sss);
	}

	jsonToFile(jsonFlowStatusFile, jTool.jFlowStatusFile);
	cJSON_Delete (jsonFlowStatusFile);

	if (nearestIntTimeInSec < 1){
		showln("flowFlag true > flowHandler calling then Refreshing");
		actionDecoder(Action_Temporary_Store_Flow);
		flowFlag = true;
	}else {
		setAlarmAfterSec(nearestIntTimeInSec);
	}
}

void Jfunctionality::jScheduleHandler(){

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

	INFO("");

	cJSON* jsonScheduleStatusFile = readFileReturnJSON (jTool.jScheduleStatusFile);
	if (!jsonScheduleStatusFile){
		showln("json not proper");
	}

	showln ("jsonScheduleStatusFile\t: " + String (printJSON(jsonScheduleStatusFile) sss) );

	int nearestIntTimeInSec = INT_MAX;
	jStr StrnearestIntTimeInSec = "";

//	jStr actionSave;
	cJSON* nearestJSON = NULL;
	std::vector<jStr> deleteTHis;
	int postionIncrementer = 0;

	showln("Key, Value");
	for (cJSON *expe = jsonScheduleStatusFile->child; expe; expe = expe->next) {

		jStr s2 = expe->string;

		showln("");
		showln(
				String (s2 sss)
				+ "\t: "
				+ String (printJSON(expe) sss)
			);

		jStr schdule1 = cJSON_GetArrayItem(expe, 0)->valuestring;
		int currentPostion = cJSON_GetArrayItem(expe, 1)->valueint;

		cJSON* jsonSchedule = readFileReturnJSON ('/' + schdule1);
		showln(
				String (schdule1 sss)
				+ "\t: "
				+ String (printJSON(jsonSchedule) sss)
			);

		cJSON* delayObj = cJSON_GetObjectItem (jsonSchedule, "times");
		int xyz = cJSON_GetArraySize(delayObj);
		showln("TotalElmentes\t: " 	+ String (xyz));

	shortcut:
		if (currentPostion >= xyz) {
			showln("Schedule completed Added to Delete List\t: " + String(s2 sss) );
			deleteTHis.push_back(s2);
			continue;
		}

		jStr currentTime = cJSON_GetArrayItem(delayObj, currentPostion )->valuestring;

		time_t t = getSysTime();
		showln ("CurrentTime \t: " + String (printDateTime(t) sss) );

		struct tm *result = localtime(&t); result->tm_sec = 0;
		if(strptime(currentTime sss, "%X", result) == NULL){
			showln ("strptime failed");
		}else {
			showln ("AlarmResult\t: " + String (printDateTime(result) sss) );
		}

		time_t t1 = mktime(result);
		int currentDelay = difftime(t1, t);
		showln("timeDiff\t: " + String (currentDelay) + " Second");

//		if (currentDelay < 60){
//		currentDelay > -20 &&
//		if (currentDelay < 60){
		if (currentDelay < -25){
			showln("Time passed Reading Next element");
			currentPostion++;
			goto shortcut;
		}

		showln("currentPostion\t: " 		+ String ( currentPostion ));
		if ( currentDelay < nearestIntTimeInSec ){
			nearestIntTimeInSec = currentDelay;

			StrnearestIntTimeInSec = currentTime;
			nearestJSON = expe;
			postionIncrementer = currentPostion;

			cJSON* actions = cJSON_GetObjectItem (jsonSchedule, "IoTick_AirCon");
			if (!actions){
				showln("Not valid-------->");
			}
			Action_Temporary_Store_Schedule.action 	= cJSON_GetArrayItem(actions, currentPostion )->valuestring;
			Action_Temporary_Store_Schedule.appliances = s2;

			showln("TempStore\t: " +
					String (Action_Temporary_Store_Schedule.appliances sss) + "|" +
					String (Action_Temporary_Store_Schedule.action sss));
		}
	}

	showln("");
	showln("nearestIntTimeInSec\t: " 	+ 	String (nearestIntTimeInSec) );
	showln("StrnearestIntTimeInSec\t: " + 	String (StrnearestIntTimeInSec sss) );

	cJSON_ReplaceItemInArray(nearestJSON, 1, cJSON_CreateNumber(++postionIncrementer) );

	for (auto ID : deleteTHis){
		showln("Deleting\t: " + String (ID sss) );
		cJSON_DeleteItemFromObject(jsonScheduleStatusFile, ID sss);
	}

	showln ("Saving JSON\t: " + String (printJSON(jsonScheduleStatusFile) sss) );
	jsonToFile(jsonScheduleStatusFile, jTool.jScheduleStatusFile);
	cJSON_Delete (jsonScheduleStatusFile);

	if (nearestIntTimeInSec < 25){
		showln("Recursive Calling > jScheduleHandler then Refreshing");
		actionDecoder (Action_Temporary_Store_Schedule);
		jScheduleHandler();
	}else {
		setAlarmHHMSS(StrnearestIntTimeInSec);
	}

}



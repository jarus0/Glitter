void SearchAndAttachSchedule() {

	INFO("")

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


	jStr CurrentSchedule = "";

	time_t currentTime = talk.coreFunc.getSysTime();
	int TodayDate = day(currentTime);
    int TodayMonth = month(currentTime);

	show("CurrentDateTime\t: " + String (talk.coreFunc.printDateTime(currentTime) sss) );
	showln ("\nSearching Today's Routine");

	bool didItAttached = false;
    for (int lop = 0; lop < 2; lop++) {

		jStr fileToRead = "";
		if (lop == 0) {
			fileToRead = "/IgnoreDays";
		}
		if (lop == 1) {
			fileToRead = "/SpecialDays";
		}


			//showln ("Searching Today's Date["+ String(TodayDate) + "] in " + String(fileToRead sss) + " List");
			showln("Reading list file\t: " + String(fileToRead sss));

		jStr contain = talk.coreFunc.jTool.jreadFile( fileToRead );
		if (contain.empty()){
			continue;
		}else{

				showln (contain sss);

		}
		cJSON *root = cJSON_Parse(contain sss);
		for ( cJSON *month = root->child; month; month = month->next ) {
//			show (month->string);
			if (String (monthShortStr(TodayMonth)) == String (month->string) ) {
					showln ("\t> " + String (month->string) + " : Months matched");
				int SizeOfDays = cJSON_GetArraySize(month);

				for (int loop = 0; loop < SizeOfDays; loop++) {
					cJSON *subitem = cJSON_GetArrayItem(month, loop);
					int day = subitem->valueint;
					if (day == TodayDate) {
							showln ("\t" + String (day)  + " : Today's day is in " + String (fileToRead sss));
//						jStr abc;
						if (lop == 0){
							CurrentSchedule = "/onNone";
							didItAttached = true;
						}
						else {
							char something[20];	sprintf(something, "/SpecialDays_%02d%02d%02d", 20, TodayMonth, TodayDate);
							CurrentSchedule = jStr (something);
							didItAttached = true;
						}

					}
				}
			}

		}
		if(root)cJSON_Delete(root);
	}

    if (!didItAttached) {
    	CurrentSchedule = "/" + jStr (dayShortStr(weekday(currentTime)));
//    		Serial.println("CurrentSchedule\t: "); showln(CurrentSchedule sss);

    }

    Serial.println ("Today Schedule\t: " + String (CurrentSchedule sss) );

    jStr someContain = talk.coreFunc.jTool.jreadFile(CurrentSchedule);
    showln(someContain sss);

    talk.coreFunc.jTool.jwriteFile(talk.coreFunc.jTool.jScheduleStatusFile, someContain);
	flagForFLowExecution = true;

}



inline void loadingCredentialsFromFile() {

	INFO("")

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

//appliancesConfigFile
	cJSON* jsonScheduleStatusFile = talk.coreFunc.readFileReturnJSON ("/appliancesConfigFile");
	talk.coreFunc.printJSON(jsonScheduleStatusFile);

	if (jsonScheduleStatusFile){

		for (cJSON *appliances = jsonScheduleStatusFile->child; appliances; appliances = appliances->next){
			showln(appliances ->string);
			jStr appType = talk.coreFunc.jTool.getStr(appliances, "type");

			showln(appType sss);
			showln("Key,Value printing");

			for (cJSON *expe = appliances->child; expe; expe = expe->next){
				jStr appliace = expe->string;

				if (appliace == "type"){
					showln("skiiping");
					continue;
				}
				showln(appliace sss);

	//			jStr configParameter = expe->valuestring;
	//			showln(configParameter sss);
				if (appliace == "GPIO"){
					int pinNumber = expe->valueint;
					if (pinNumber != -1 ) {
						showln ("Making pinNumber Output\t: " + String (pinNumber) );
						pinMode(pinNumber, OUTPUT);
					}
				}
			}
		}
	}

	cJSON_Delete( jsonScheduleStatusFile );

// Ping time interval time
	jStr pingTimeStr = talk.coreFunc.jTool.jreadFile(talk.coreFunc.jTool.pingTimeFile sss);
	talk.coreFunc.jTool.pingTime = pingTimeStr.empty() ? 120 : std::stoi ( pingTimeStr );
	printf("PingTime\t: %d\n", talk.coreFunc.jTool.pingTime);

//	Temperature config loading
	jStr temperatureContain = talk.coreFunc.jTool.jreadFile("/temperature");
	if (temperatureContain.empty()){
		talk.coreFunc.jTool.minTemperature 		= 10;
		talk.coreFunc.jTool.maxTemperature 		= 60;
		talk.coreFunc.jTool.pivotTemperature 	= 23;
	}else{
		cJSON* temperatureContainJSON = cJSON_Parse( temperatureContain .data() );
		talk.coreFunc.jTool.minTemperature 		= talk.coreFunc.jTool.getInt(temperatureContainJSON, "min");
		talk.coreFunc.jTool.maxTemperature 		= talk.coreFunc.jTool.getInt(temperatureContainJSON, "max");
		talk.coreFunc.jTool.pivotTemperature 	= talk.coreFunc.jTool.getInt(temperatureContainJSON, "pivot");
		cJSON_Delete(temperatureContainJSON);
	}

	showln ("min, max, pivot\t: "+
			String (talk.coreFunc.jTool.minTemperature) +", "+
			String (talk.coreFunc.jTool.maxTemperature) +", "+
			String (talk.coreFunc.jTool.pivotTemperature)
		);

	jStr meshConfigStr = talk.coreFunc.jTool.jreadFile("/meshConfig");
	if (meshConfigStr.empty()){
		meshConfigStr = "{\"wanSSID\":\"IoTick\",\"wanPwd\":\"Enterprise\",\"meshID\":\"77:77:77:77:77:77\",\"meshPwd\":\"MAP_PASSWD\",\"groupID\":\"ff:ff:ff:ff:ff:ff\"}";
	}
	cJSON* meshConfigJSON = cJSON_Parse( meshConfigStr .data() );
	if (meshConfigJSON){
		jStr wanSSID 	= talk.coreFunc.jTool.getStr(meshConfigJSON, "wanSSID");
		jStr wanPwd 	= talk.coreFunc.jTool.getStr(meshConfigJSON, "wanPwd");
		jStr meshID 	= talk.coreFunc.jTool.getStr(meshConfigJSON, "meshID");
		jStr meshPwd 	= talk.coreFunc.jTool.getStr(meshConfigJSON, "meshPwd");
		jStr groupID 	= talk.coreFunc.jTool.getStr(meshConfigJSON, "groupID");

		showln ("Router SSID, Pass\t: " + String(wanSSID sss) + "|" + String(wanPwd  sss));
		showln ("Mesh & Group ID, Pass\t: " +
				String(meshID sss) 	+"|"+
				String(groupID sss)	+ "|" +
				String(meshPwd sss));

		mesh_addr_t MeshID;		talk.coreFunc.jTool.strTomac(meshID, MeshID.addr);
		mesh_addr_t GroupID;	talk.coreFunc.jTool.strTomac(groupID, GroupID.addr);

//		Future Task; > show ("Warning\t: GroupID not Using in Config");

		//wanSSID
			memcpy(&talk.cfg.router.ssid, wanSSID.c_str(), wanSSID.length());
			talk.cfg.router.ssid_len = wanSSID.length();
		//wanPwd
			memcpy(&talk.cfg.router.password, wanPwd.c_str(), wanPwd.length());
		//Mesh ID
			memcpy(&talk.cfg.mesh_id, MeshID.addr, 6);
		//Mesh Password
			memcpy(&talk.cfg.mesh_ap.password, meshPwd.c_str(), meshPwd.length() );
		//GroupID Future task
//				show("Warning\t: GroupID Setting code remain");

		// Default value
			talk.cfg.channel 		= 0;	//channel (must match the router's channel)
			talk.cfg.mesh_ap.max_connection = 6;

	}else {
		showlnf ("Mesh Config Problem");
	}
	cJSON_Delete(meshConfigJSON);

}




void SendPing(cJSON* AppliancesModulesJSON[], int countAppliancesModulesJSON){

	INFO("");

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


	cJSON *ping = cJSON_CreateObject();

	//id
		cJSON_AddItemToObject(ping, "id", cJSON_CreateString(talk.coreFunc.jTool.deviceMacIDStr sss) );

	//organizationId
		cJSON_AddItemToObject(ping, "organizationId", cJSON_CreateString(talk.coreFunc.jTool.orgId sss) );

	//cmd
		cJSON *cmd = cJSON_CreateArray();
		cJSON_AddItemToArray(cmd , cJSON_CreateNumber( 5 ) );
		cJSON_AddItemToArray(cmd , cJSON_CreateNumber( 1 ) );
		cJSON_AddItemToObject(ping, "cmd", cmd);

	//data
	cJSON *data = cJSON_CreateObject();

	//rtcTime
		time_t currentTime = talk.coreFunc.getRtcTime();
//		talk.coreFunc.printDateTime(currentTime);
		cJSON_AddItemToObject(data, "rtcTime", cJSON_CreateNumber( currentTime - 19800 ) );

	//heapSize
		int heapSize = ESP.getFreeHeap();
		cJSON_AddItemToObject(data, "heapSize", cJSON_CreateNumber( heapSize ) );

////appliances Array
		cJSON *appliancesArray = cJSON_CreateArray();
		for (int lp1 = 0; lp1 < countAppliancesModulesJSON; lp1++){
			cJSON_AddItemToArray(appliancesArray, AppliancesModulesJSON[lp1]);
		}
		cJSON_AddItemToObject(data, "appliances",  appliancesArray);

	//date
	cJSON_AddItemToObject(ping, "data", data);

	char* rendered =
			cJSON_PrintUnformatted( ping );

	showln (rendered);
	talk.sendToRpi ( rendered );
//	if (mode){
//		talk.sendToRpi ( rendered );
//	}else{
////		jBLE_Send(&paramStoreing, rendered);
//	}
//	talk.sendToRpi ( rendered );
	cJSON_Delete( ping );
	free( rendered );

}



#ifdef jAIR_CONDITIONER
//https://github.com/crankyoldgit/IRremoteESP8266
#define DECODE_AC true
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>

#define kIrLed 4
IRac OnDemandACtype(kIrLed);

int sendButtonFromFile(jStr fileName){

	jStr fileContain = talk.coreFunc.jTool.jreadFile(fileName);
	if (fileContain.empty()){
//		show ("Button File not Found");
//		showlnf ("");
		return -1;
	}
//	showln (fileContain sss);

	cJSON *root = cJSON_Parse( fileContain.data() );
	if (!root){
		showlnf ("Button JSON Problem");
		return -1;
	}


	if (OnDemandACtype.isProtocolSupported( decode_type_t(talk.coreFunc.jTool.prot) )){

		OnDemandACtype.next.protocol 	= decode_type_t(talk.coreFunc.jTool.prot);
		OnDemandACtype.next.model 		= talk.coreFunc.jTool.model;
//		showln ("AC Protocol & Model Supported\t: " + String (OnDemandACtype.next.protocol) + ", " + String(OnDemandACtype.next.model) );
		showln ("Supported AC");


	}else {
		showln ("No its supported");
		return 1;
	}

	OnDemandACtype.next.mode 		= stdAc::opmode_t(talk.coreFunc.jTool.getInt(root, "mode"));
	OnDemandACtype.next.celsius 	= talk.coreFunc.jTool.getInt(root, "celsius");
	OnDemandACtype.next.degrees		= talk.coreFunc.jTool.getInt(root, "temprature");
	OnDemandACtype.next.fanspeed 	= stdAc::fanspeed_t(talk.coreFunc.jTool.getInt(root, "fan"));
	OnDemandACtype.next.swingv 		= stdAc::swingv_t(talk.coreFunc.jTool.getInt(root, "swingv"));
	OnDemandACtype.next.swingh 		= stdAc::swingh_t(talk.coreFunc.jTool.getInt(root, "swingh"));
	OnDemandACtype.next.light 		= talk.coreFunc.jTool.getInt(root, "light");                         // Turn off any LED/Lights/Display that we can.
	OnDemandACtype.next.beep 		= talk.coreFunc.jTool.getInt(root, "beep");                        // Turn off any beep from the A/C if we can.
	OnDemandACtype.next.econo	 	= talk.coreFunc.jTool.getInt(root, "econo");                        // Turn off any economy modes if we can.
	OnDemandACtype.next.filter 		= talk.coreFunc.jTool.getInt(root, "filter");                       // Turn off any Ion/Mold/Health filters if we can.
	OnDemandACtype.next.turbo 		= talk.coreFunc.jTool.getInt(root, "turbo"); // Don't use any turbo/powerful/etc modes.
	OnDemandACtype.next.quiet 		= talk.coreFunc.jTool.getInt(root, "quiet"); // Don't use any quiet/silent/etc modes.
	OnDemandACtype.next.sleep		= talk.coreFunc.jTool.getInt(root, "sleep");    // Don't set any sleep time or modes.
	OnDemandACtype.next.clean 		= talk.coreFunc.jTool.getInt(root, "clean");; // Turn off any Cleaning options if we can.
	OnDemandACtype.next.clock 		= talk.coreFunc.jTool.getInt(root, "clock");    // Don't set any current time if we can avoid it.
	OnDemandACtype.next.power 		= talk.coreFunc.jTool.getInt(root, "power"); // Initially start with the unit off.

	cJSON_Delete(root);
	vTaskSuspendAll();
	OnDemandACtype.sendAc();           // Have the IRac class create and send a message.
	xTaskResumeAll();
	return 1;

}

#endif



#ifdef jAIR_CONDITIONER
    void sendButton(jStr buttonName){

        INFO("butonExc");
        if (buttonName[0] != '/') {
            buttonName = '/' + buttonName;      //              Future Task : Why to add Slash
        }
//      int response =
                sendButtonFromFile(buttonName);
    }

#endif

void actionDecoder(appWithAction jtempAA){

    #undef showLOG
    #define showLOG 1

    showln (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " + String (jtempAA.appliances sss) + "|" + String (jtempAA.action sss));

    jStr firstTwoChar = jtempAA.action.substr(0, 2);

    #if showLOG==1
        showln("firstTwoChar\t: " + String (firstTwoChar sss) );
    #endif

    if (firstTwoChar == "F-"){
        showln("This is Flow : Inserting to jFlowStatusFile");

        cJSON* newArray = cJSON_CreateArray();
        cJSON_AddItemToArray(newArray, cJSON_CreateString(jtempAA.action sss));
        cJSON_AddItemToArray(newArray, cJSON_CreateNumber(0));
        cJSON_AddItemToArray(newArray, cJSON_CreateNumber(0));
        talk.coreFunc.flowInsert(jtempAA.appliances, newArray );

    } else if (firstTwoChar == "B-"){
        showln("This is Button");

        cJSON* json = talk.coreFunc.readFileReturnJSON ("/appliancesConfigFile");
        if (json){
            cJSON* json1 = cJSON_GetObjectItem(json, jtempAA.appliances sss);
            if (json1){
                talk.coreFunc.jTool.prot = talk.coreFunc.jTool.getInt(json1, "Protocol");
                talk.coreFunc.jTool.model = talk.coreFunc.jTool.getInt(json1, "Model");
                show("Protocol, Model\t: " + String (talk.coreFunc.jTool.prot) + "|" + String (talk.coreFunc.jTool.model));
                sendButton(jtempAA.action);

            }else {
                showln("Problem appliancesConfigFile");
            }
        }
        cJSON_Delete( json );
    } else if (firstTwoChar == "D-"){
        cJSON* json = talk.coreFunc.readFileReturnJSON ("/appliancesConfigFile");
        if (json){
            cJSON* json1 = cJSON_GetObjectItem(json, jtempAA.appliances sss);
            if (json1){
                int pinGPIO = talk.coreFunc.jTool.getInt(json1, "GPIO");
                showln("GPIO\t: " + String (pinGPIO) + "|" + String (jtempAA.action sss));
                digitalWrite(pinGPIO, jtempAA.action == "D-0" ? LOW : HIGH);
            }else {
                showln("Problem");
            }
        }
        cJSON_Delete( json );
    }

//Creating Response
    cJSON *root1 = cJSON_CreateObject();
    cJSON_AddItemToObject(root1, "id", cJSON_CreateString(talk.coreFunc.jTool.deviceMacIDStr sss) );
//cmd
    cJSON *cmd = cJSON_CreateArray();
    cJSON_AddItemToArray(cmd, cJSON_CreateNumber( 5 ) );
    cJSON_AddItemToArray(cmd, cJSON_CreateNumber( 2 ) );
    cJSON_AddItemToObject(root1, "cmd", cmd);

//applianceId-actionID adding
    cJSON *applianceId_actionID  = cJSON_CreateObject();
    cJSON_AddItemToObject(applianceId_actionID, jtempAA.appliances sss, cJSON_CreateString( jtempAA.action sss ) );
    cJSON_AddItemToObject(root1, "data", applianceId_actionID );

    char* rendered = cJSON_PrintUnformatted(root1);
    showln ("Response\t: " + String (rendered) );
    talk.sendToRpi ( rendered );
    cJSON_Delete(root1); free (rendered);

}

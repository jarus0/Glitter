/*
 * CommunicationModule.cpp
 *
 *  Created on: 17-Sep-2020
 *      Author: suraj
 */

#include "CommunicationModule.h"

CommunicationModule::CommunicationModule() {
	// TODO Auto-generated constructor stub

}

CommunicationModule::~CommunicationModule() {
	// TODO Auto-generated destructor stub
}



void CommunicationModule::broadcastToNodes(jStr msg){

//	unsigned char jMac[6]; coreFunc.jTool.strTomac("3c:71:bf:fb:ee:24", jMac);
/*	mesh_addr_t broadcast_group_id = {
			.addr = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00}
	};

	mesh_data_t data = {};
	data.data 	= (uint8_t *) msg.c_str();
	data.size 	= msg.length();
	data.proto 	= MESH_PROTO_BIN;
	data.tos 	= MESH_TOS_P2P;

	esp_err_t errReturn = esp_mesh_send( &broadcast_group_id, &data, MESH_DATA_GROUP, NULL, 0);

	if (errReturn)
	{
		INFO("meshSendFail");
		Serial.println ( esp_err_to_name (errReturn) );
	}else{
		INFO("BroadcastSuccess");
	}*/
}

//void CommunicationModule::rootToNodeMsg (mesh_addr_t senderAddress, jStr collectedData)
//{
//	meshDataStreamer(collectedData, senderAddress);
//}

void CommunicationModule::websocketToSay( jStr myMsg )
{
/*#undef showLOG
#define showLOG 0

	#ifdef jmqttSupport
		//	Future task : Serial.println("Check first that in which mode device is, BLE or MQTT");

	if (coreFunc.jTool.mqttStatus){
//		if (coreFunc.jTool.mqttStatus){
			int jdata = esp_mqtt_client_publish(client, MQTTpublishChannel sss, myMsg.c_str(), 0, 0, 0);
			if(jdata){}
			#if showLOG==1
				showln ("MQTT publish response\t: " + String(jdata) );
			#endif
		}else {
			#if showLOG==1
				showlnf("jMQTT_NOT_CONNETED");
			#endif
		}
	#endif

#ifdef jwebsocketSupport


	//INFO("MSG");
	if (esp_websocket_client_is_connected(websocketClient))
	{
//		std::string myMsg = "some messsage";
//		int len = sprintf(data, "hello %04d", i++);
//		ESP_LOGI(TAG, "Sending %s", myMsg);
//		esp_websocket_client_send_text(
//				client,
//				"someConst",
//				10,	//myMsg.length,
//				portMAX_DELAY);	//esp_websocket_client_send(client, data, len, portMAX_DELAY);


//		showln("some message");
//		showln(myMsg sss);

		cJSON* old = cJSON_Parse(myMsg sss);

		cJSON* message = cJSON_CreateObject();
		cJSON* message_details = cJSON_CreateObject();

		cJSON_AddItemToObject(message_details, "message_details", old );
		cJSON_AddItemToObject(message, "message", message_details );


		char* c= cJSON_PrintUnformatted(message);
		jStr abc = jStr (c);

//		cJSON_Delete(old);
		cJSON_Delete(message);
		free(c);

		show("Sending to server\t: ");
		showln (abc sss);

		esp_websocket_client_send_text(
				websocketClient,
				abc.c_str(),
				abc.length(),
				portMAX_DELAY);	//esp_websocket_client_send(client, data, len, portMAX_DELAY);

	}

#endif
*/
}

void CommunicationModule::sendToRpi( jStr msgToSend )
{
/*	switch ( valWhoAmi )
	{
	    case None:
	    {
	    	//showln ("None : Error whom to send" + String (msgToSend sss));
	    }break;
	    case Root:
	    {
	    	//showln ("I AM ROOT" + String (msgToSend .c_str()) );
	    	// you can also check that is this root
	    	websocketToSay(msgToSend);
	    	responeseStr = msgToSend;
	    }break;
	    case Node:
	    {
	    	//showln ("Node : Msg to RPI");
//	    	nodeToRootMsg(msgToSend);
	    	meshDataStreamer(msgToSend);
	    }break;
	    default: {}
	}*/

}

void CommunicationModule::websocketCollect (jStr collectedData) {
/*
	jStr toSend = "xxx";
	int pos = collectedData.find("\"id\":");

	if ( pos > 0 ) {

	#ifdef jwebsocketSupport
			toSend = collectedData.substr(pos + 7, 17);
	#endif
	#ifdef jmqttSupport
		toSend = collectedData.substr(pos + 6, 17);
	#endif

	}else {
		showlnf ("ID not Found in message");
		showln (collectedData sss);
	}

	if ( toSend == coreFunc.jTool.deviceMacIDStr )
	{
//		showln ("Root ID match found : Action Performing : data is " + String (collectedData sss) );
//		char *some = objJfunctionality.npostmanFunction( collectedData );

		char *some = coreFunc.EnumPostmanFunction( collectedData );
		sendToRpi( some );
		//showln ( "root sending reply to rpi" + String (sstr.c_str()) );
		free (some);

	}else if (toSend.size() > 0)
	{
		mesh_addr_t senderAddress; coreFunc.jTool.strTomac(toSend, senderAddress.addr);
		meshDataStreamer(collectedData, senderAddress);
	}
	else
	{
		showln ("ID doesn't match");
	}*/
}



void CommunicationModule::setHandlers(esp_event_handler_t jevent_handler11)
{


	showln("MESH_HANDLER_SETTING");
	return ;

    /*  mesh initialization */
//    ESP_ERROR_CHECK(esp_mesh_init());
    //ESP_ERROR_CHECK(esp_event_handler_register(MESH_EVENT, ESP_EVENT_ANY_ID, &mesh_event_handler, NULL));
/*    ESP_ERROR_CHECK(esp_event_handler_register(MESH_EVENT, ESP_EVENT_ANY_ID, jevent_handler11, NULL));

    ESP_ERROR_CHECK(esp_mesh_set_max_layer(CONFIG_MESH_MAX_LAYER));
    ESP_ERROR_CHECK(esp_mesh_set_vote_percentage(1));
    ESP_ERROR_CHECK(esp_mesh_set_ap_assoc_expire(10));
    ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(WIFI_AUTH_WPA_PSK));	
//    ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(WIFI_AUTH_WPA2_PSK));	
//    ESP_ERROR_CHECK(esp_mesh_set_ap_authmode(CONFIG_MESH_AP_AUTHMODE));


//    ESP_ERROR_CHECK(esp_mesh_set_config(&cfg));	// cfg set from Config file


//    esp_mesh_allow_root_conflicts(true);
//
//  	uint8_t NodeAddress1[6] = {0x3c, 0x71, 0xbf, 0xfd, 0x0f, 0x3c};
//  	uint8_t NodeAddress2[6] = {0x3c, 0x71, 0xbf, 0xfd, 0x23, 0x84};
//  	uint8_t NodeAddress3[6] = {0x3c, 0x71, 0xbf, 0xfd, 0x00, 0xf8};
//
//    mesh_addr_t addr1[3];
//
//    for (int lp1 = 0; lp1 < 6; lp1++){
//    	addr1[0].addr[lp1] = NodeAddress1[lp1];
//    }
//    for (int lp1 = 0; lp1 < 6; lp1++){
//    	addr1[1].addr[lp1] = NodeAddress2[lp1];
//    }
//    for (int lp1 = 0; lp1 < 6; lp1++){
//    	addr1[2].addr[lp1] = NodeAddress3[lp1];
//    }

//    showln (esp_mesh_is_my_group(&addr1));
//    addr1.addr[5] = 0xff;
//    showln (esp_mesh_is_my_group(&addr1));

//    esp_mesh_get_group_list(&addr1, 0);

//	mesh_addr_t addr2;
//	esp_mesh_get_group_list(&addr2, 1);
//	jStr deviceMacIDStr1 = coreFunc.jTool.arrayToStrMac (addr2.addr);
//	showln ( "MAC id is :- " + String (deviceMacIDStr1 .c_str()) );
//	showln (esp_mesh_get_group_num());

//    mesh_addr_t GroupID = {
//    		.addr = {0xff, 0xff, 0xff, 0xff, 0xff, 0x00}
//    };
//
//    esp_err_t errReturn = esp_mesh_set_group_id(&GroupID, 1);
//    if (errReturn){
//    	Serial.print ( "Group ID can't set" );
//    	Serial.println ( esp_err_to_name (errReturn) );
//    }else {
//    	Serial.print ( "Group ID set successfully" );
//    }

//    ESP_ERROR_CHECK(esp_mesh_start());	

    //    esp_mesh_fix_root(true);
*/

}

//void CommunicationModule::websocket_app_start(void){}

void CommunicationModule::sendInitinfo()
{
/*
		time_t currentTime = coreFunc.getRtcTime() - 19800;
		Serial.print("Init Time\t: "); coreFunc.printDateTime(currentTime + 19800);
	//	Serial.print("Epoch Time\t: ");
	//	Serial.println (currentTime);

		cJSON* initimessage = cJSON_CreateObject();
		cJSON_AddItemToObject(initimessage, "id", cJSON_CreateString( coreFunc.jTool.deviceMacIDStr sss ) );

	//cmd
		cJSON *arrya1 = cJSON_CreateArray();
		cJSON_AddItemToArray(arrya1 , cJSON_CreateNumber(5) );
		cJSON_AddItemToArray(arrya1 , cJSON_CreateNumber(0) );
		cJSON_AddItemToObject(initimessage, "cmd", arrya1 );

	//data
		cJSON* data = cJSON_CreateObject();
		cJSON_AddItemToObject(data, "hwVersion", cJSON_CreateString(coreFunc.jTool.fhardwareVer sss) );
		cJSON_AddItemToObject(data, "swVersion", cJSON_CreateString(coreFunc.jTool.ffirmwareVer sss) );
		cJSON_AddItemToObject(data, "rtcTime", cJSON_CreateNumber( currentTime ) );

		cJSON_AddItemToObject(data, "orgId", cJSON_CreateString( coreFunc.jTool.orgId sss ) );
		cJSON_AddItemToObject(data, "zoneId", cJSON_CreateString( coreFunc.jTool.zoneId sss) );
		cJSON_AddItemToObject(data, "Layer", cJSON_CreateNumber(-1) );

	//hashCode
		jStr allhashCode = coreFunc.jTool.jreadFile("/hashCode");
		if (allhashCode.empty()){
			cJSON_AddItemToObject(data, "hashCode", cJSON_CreateNumber(0) );
		}else {
			cJSON* allhashCodeJOSN = cJSON_Parse( allhashCode sss );
			cJSON_AddItemToObject(data, "hashCode", allhashCodeJOSN);
		}

		cJSON_AddItemToObject(initimessage, "data", data );

		char* rendered = cJSON_PrintUnformatted( initimessage  );
		showln (rendered);
		sendToRpi ( rendered  );

		cJSON_Delete( initimessage  );
		free( rendered );*/
}

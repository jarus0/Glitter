/*
 * CommunicationModule.h
 *
 *  Created on: 17-Sep-2020
 *      Author: suraj
 */

#ifndef MAIN_COMMUNICATIONMODULE_H_
#define MAIN_COMMUNICATIONMODULE_H_

#include "FirmwareType.h"
#include "Jfunctionality.h"

#undef INFO
#define INFO(MSG) coreFunc.jTool.info(__FILE__, (String (__LINE__)).c_str(), __func__, MSG );

#include "cJSON.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_mesh_internal.h"
#include "nvs_flash.h"

#ifdef jwebsocketSupport
#include <stdio.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_websocket_client.h"

#include "esp_event.h"
#include "esp_event_loop.h"
#endif

//static const char *TAG = "WEBSOCKET";
#define RX_SIZE          (1024 * 5)
static uint8_t rx_buf[RX_SIZE] = { 0, };

static bool is_running = true;
static bool is_mesh_connected = false;
//mesh_addr_t mesh_parent_addr;


#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "tcpip_adapter.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"


class CommunicationModule {
public:
	CommunicationModule();
	virtual ~CommunicationModule();

	Jfunctionality coreFunc;

	//	Config config;

	int mesh_layer = -1;
		// Default Parameter Given Below
		jStr MQTTpublishChannel = "default/data";		// Default Parameter
		jStr MQTTsubscriChannel = "default/config";		// Default Parameter
		jStr routerName = "Mark";						// Default Parameter
		jStr routerPass = "1234567890";				// Default Parameter

	    jStr meshApPass = "meshPass";
	    int meshAPConnections = 6;
	    int maxLayer = 6;

		mesh_cfg_t cfg = {}; 	//= MESH_INIT_CONFIG_DEFAULT();
	//	esp_mqtt_client_config_t mqtt_cfg = {0};
	//	esp_mqtt_client_config_t mqtt_cfg;

	//	esp_websocket_client_handle_t client;			// make a class check that does this not a null
		esp_mqtt_client_handle_t client;

	#ifdef jwebsocketSupport
		esp_websocket_client_handle_t websocketClient;
	#endif

		int valWhoAmi = None;

		std::string responeseStr;
		int websocketDisConnectCounter = 0;

		void sendInitinfo();
	//	void nodeToRootMsg (jStr msg);
	//	void rootToNodeMsg (mesh_addr_t senderAddress, jStr collectedData);
		void websocketToSay( jStr msg );
		void sendToRpi( jStr msgToSend );
		void websocketCollect (jStr collectedData);
	//	void websocket_app_start(void);
		void broadcastToNodes(jStr msg);
		//static void esp_mesh_p2p_rx_main(void *arg);
		esp_err_t esp_mesh_comm_p2p_start(void);

	//	void setHandlers(esp_event_handler_t jevent_handler1, esp_event_handler_t jevent_handler2);
		void setHandlers(esp_event_handler_t jevent_handler1);
		esp_event_handler_t jevent_handler2;

	//	inline void meshMessaging(jStr collectedData, mesh_addr_t senderAddress = {0}){

	//		sendBigStr(senderAddress, collectedData);

	//		mesh_data_t data;
	//		data.data = (uint8_t *) collectedData.c_str();
	//		data.size = collectedData.length();
	//		data.proto = MESH_PROTO_BIN;
	//		data.tos = MESH_TOS_P2P;
	////		esp_err_t errReturn = esp_mesh_send( NULL, &data, MESH_DATA_P2P, NULL, 0);
	//		esp_err_t errReturn = esp_mesh_send(&senderAddress ,&data, MESH_DATA_P2P, NULL, 0);
	//
	//		int send_count = 0;
	//		if (errReturn)
	//		{
	//			Serial.println ("esp_mesh_sending fail");
	//			Serial.println ( esp_err_to_name (errReturn) );
	//		}else{}
	//	}

	//	template <typename T>
	//	void jlog(T logMsg)
	//	{
	//		std::ostringstream oss;
	//		oss << logMsg;
	//		jStr logMsg1 = objJarusAccessories.deviceMacIDStr + ":- " + oss .str();
	//		sendToRpi(logMsg1);
	//	}

		int maxBufferTransferLimit = 20;


		inline bool meshDataStreamer(jStr dataStr, mesh_addr_t senderAddress = {0}){
	//	inline bool sendBigStr(jStr dataStr, mesh_addr_t senderAddress = {0}){
		/*
			#define showLOG 0

			#if showLOG == 1
				INFO("");
			#endif

			int totalLength = dataStr.length();

			int totalPartNeededToSend = dataStr.length() / maxBufferTransferLimit;
			int remainBits =	dataStr.length() % maxBufferTransferLimit;
			if (remainBits > 0) {totalPartNeededToSend++;} totalPartNeededToSend--;

			#if showLOG == 1
				printf("totalPartNeededToSend\t: %d\n", totalPartNeededToSend);
			#endif

	//		printf("remainBits\t\t: %d\n", remainBits);


			int coounter = 0;
			while (totalLength > 0){

	//			totalLength = dataStr.length();
				int chunkSize = std::min (maxBufferTransferLimit, totalLength);
				#if showLOG == 1
					showln("totalLength\t: " 	+ String(totalLength) );
					showln("chunkSize\t: " 		+ String(chunkSize+2) );
				#endif

				uint8_t buffer[chunkSize+2]; memcpy ( &buffer[2], dataStr sss, chunkSize );
				buffer[0] = coounter++;
				buffer[1] = totalPartNeededToSend;
				#if showLOG == 1
					showln("CurrentPart\t: " + String(coounter) +"/"+ String(totalPartNeededToSend));
				#endif

				mesh_data_t data;
				data.data 	= buffer;
				data.size 	= chunkSize+2;
				data.proto 	= MESH_PROTO_BIN;
				data.tos 	= MESH_TOS_P2P;

				esp_err_t err = esp_mesh_send(&senderAddress ,&data, MESH_DATA_P2P, NULL, 0);
				if (err){
					#if showLOG == 1
						showln ("SendNodeError\t: " + String (esp_err_to_name (err) ) );
					#endif
					return false;
				}
				else{
					totalLength	-= chunkSize;
					dataStr.erase(dataStr.begin(), dataStr.begin() + chunkSize);
				}

			}
			*/
			return true;
		}


	    inline int StrToHex(const char str[])
		{
		  return (int) strtol(str, 0, 16);
		}

	#ifdef jwebsocketSupport
	    esp_event_handler_t jevent_handler_Websocket;
	    void setWebsocketHandlers(esp_event_handler_t jevent_handler11){
	    	jevent_handler_Websocket = jevent_handler11;
	    }

	    void websocket_app_start(void)
	    {

	        esp_websocket_client_config_t websocket_cfg = {}; 	//	websocket_cfg.buffer_size default buffer size can be change
	        showln ("Warning : when root lose its IP then Websocket must delete");

	#ifdef jarus
	        websocket_cfg.uri = "ws://192.168.43.224";
	        websocket_cfg.port = 12345;
	#endif
	//        websocket_cfg.uri = "ws://192.168.43.224";
	//        websocket_cfg.uri = "ws://192.168.43.224";
	#ifdef otherperson
	        websocket_cfg.uri  = "ws://192.168.2.8:8000/ws/v1/bulk-device-socket";
	#endif

	//

	        websocketClient = esp_websocket_client_init(&websocket_cfg);	//esp_websocket_client_handle_t client = esp_websocket_client_init(&websocket_cfg);
	        //esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
	        esp_websocket_register_events(websocketClient, WEBSOCKET_EVENT_ANY, jevent_handler_Websocket, (void *)websocketClient);
	        esp_websocket_client_start(websocketClient);

	        show ("websocket starting sendInitinfo Remain >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	//        sendInitinfo();
	//        char data[32];
	//        int i = 0;
	//
	//        show ("After 5 second Initialize Message Request");
	//        vTaskDelay(3000 / portTICK_RATE_MS);
	//
	//        mesh_addr_t route_table[CONFIG_MESH_ROUTE_TABLE_SIZE];
	//        int route_table_size = 0;
	//
	//    	esp_mesh_get_routing_table(
	//    			(mesh_addr_t *) &route_table,
	//    			CONFIG_MESH_ROUTE_TABLE_SIZE * 6,
	//    			&route_table_size);
	//    	show (route_table_size);
	//    	show ("show (route_table_size);");
	//        for (int looop = 0; looop < route_table_size; looop++) {
	//            if (esp_websocket_client_is_connected(websocketClient)){
	//
	//            	vTaskDelay( 100 / portTICK_RATE_MS );
	//            	show ("Websocket Connected : " + String(looop) );
	//            	show (objJarusAccessories.arrayToStrMac(route_table[looop].addr) sss);
	//
	//            	//{"id":"24:6f:28:15:ab:fc","cmd":0,"type":4}
	//    			cJSON *root = cJSON_CreateObject();
	//    			cJSON_AddItemToObject(root, "id", cJSON_CreateString(objJarusAccessories.arrayToStrMac(route_table[looop].addr) sss));
	//    			cJSON_AddItemToObject(root, "cmd", cJSON_CreateNumber(0));
	//    			cJSON_AddItemToObject(root, "type", cJSON_CreateNumber(4));
	//    			char* rendered = cJSON_Print(root);
	//    			//sendToRpi(rendered);
	//    			rootToNodeMsg(route_table[looop], rendered);
	//            	free(rendered);
	//            	cJSON_Delete(root);
	//            }
	//        }

	        show ("Websocket infinity waiting loop [valWhoAmi]:- " + String (valWhoAmi) );
	    	while ( valWhoAmi == Root ) {

	    //        if (esp_websocket_client_is_connected(client))
	    //        {
	    //            int len = sprintf(data, "hello %04d", i++);
	    //            ESP_LOGI(TAG, "Sending %s", data);
	    //            esp_websocket_client_send_text(client, data, len, portMAX_DELAY);	//esp_websocket_client_send(client, data, len, portMAX_DELAY);
	    //        }
	            esp_task_wdt_reset();
	            vTaskDelay(1000 / portTICK_RATE_MS);
	        }

	        Serial.println ("IP lost Websocket Closing>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	        vTaskDelay(1000 / portTICK_RATE_MS);	// Give server some time to respond before closing
	        esp_websocket_client_stop(websocketClient);
	        ESP_LOGI(TAG, "Websocket Stopped");


	        esp_websocket_client_destroy(websocketClient);

	    }


	#endif

};

#endif /* MAIN_COMMUNICATIONMODULE_H_ */

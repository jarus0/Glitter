/*
 * main.h
 *
 *  Created on: 24-Mar-2020
 *      Author: suraj
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_


#define newFeature

#include "FirmwareType.h"

#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"


#include "esp32/rom/rtc.h"
//#include "rtc_time.h"
#include "esp_sleep.h"

// Future task why multiple includtion make it once

#include "time.h"
#include "esp_log.h"

#include <Arduino.h>
#include "esp_task_wdt.h"
#include "CommunicationModule.h"
#include <Wire.h>
#include <cstring>
//static const char *TAG = "mesh_main";
static const char *MESH_TAG = "mesh_main";


#ifdef jACS712
//--------------POWER METER-----------------------------
struct powerData {
  float iRMS;
  float realPower;
  float kWattHour;
};
//powerData data;

#endif

CommunicationModule talk;
esp_err_t gErr;


#undef INFO
#define INFO(MSG) talk.coreFunc.jTool.info(__FILE__, (String (__LINE__)).c_str(), __func__, MSG );




// //Start Alarm points
// #include <stdio.h>
// #include "esp_types.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "driver/periph_ctrl.h"
// #include "driver/timer.h"

// #define TIMER_DIVIDER         16  //  Hardware timer clock divider
// #define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds
// #define TIMER_INTERVAL0_SEC   (3.4179) // sample test interval for the first timer
// #define TIMER_INTERVAL1_SEC   (5.78)   // sample test interval for the second timer
// #define TEST_WITHOUT_RELOAD   0        // testing will be done without auto reload
// #define TEST_WITH_RELOAD      1        // testing will be done with auto reload

// /*
//  * A sample structure to pass events
//  * from the timer interrupt handler to the main program.
//  */
// typedef struct {
//     int type;  // the type of timer's event
//     timer_group_t timer_group;
//     timer_idx_t timer_idx;
//     uint64_t timer_counter_value;
// } timer_event_t;

// xQueueHandle timer_queue;

// /*
//  * A simple helper function to print the raw timer counter value
//  * and the counter value converted to seconds
//  */
// static void inline print_timer_counter(uint64_t counter_value)
// {
//     printf("Counter: 0x%08x%08x\n", (uint32_t) (counter_value >> 32),
//                                     (uint32_t) (counter_value));
//     printf("Time   : %.8f s\n", (double) counter_value / TIMER_SCALE);
// }

// /*
//  * Timer group0 ISR handler
//  *
//  * Note:
//  * We don't call the timer API here because they are not declared with IRAM_ATTR.
//  * If we're okay with the timer irq not being serviced while SPI flash cache is disabled,
//  * we can allocate this interrupt without the ESP_INTR_FLAG_IRAM flag and use the normal API.
//  */
// void IRAM_ATTR timer_group0_isr(void *para)
// {
//     int timer_idx = (int) para;

//     /* Retrieve the interrupt status and the counter value
//        from the timer that reported the interrupt */
//     uint32_t intr_status = TIMERG0.int_st_timers.val;
//     TIMERG0.hw_timer[timer_idx].update = 1;
//     uint64_t timer_counter_value =
//         ((uint64_t) TIMERG0.hw_timer[timer_idx].cnt_high) << 32
//         | TIMERG0.hw_timer[timer_idx].cnt_low;

//     /* Prepare basic event data
//        that will be then sent back to the main program task */
//     timer_event_t evt;
//     evt.timer_group = TIMER_GROUP_0;
//     evt.timer_idx = (timer_idx_t)timer_idx;
//     evt.timer_counter_value = timer_counter_value;

//     /* Clear the interrupt
//        and update the alarm time for the timer with without reload */
//     if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
//         evt.type = TEST_WITHOUT_RELOAD;
//         TIMERG0.int_clr_timers.t0 = 1;
//         timer_counter_value += (uint64_t) (TIMER_INTERVAL0_SEC * TIMER_SCALE);
//         TIMERG0.hw_timer[timer_idx].alarm_high = (uint32_t) (timer_counter_value >> 32);
//         TIMERG0.hw_timer[timer_idx].alarm_low = (uint32_t) timer_counter_value;
//     } else if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_1) {
//         evt.type = TEST_WITH_RELOAD;
//         TIMERG0.int_clr_timers.t1 = 1;
//     } else {
//         evt.type = -1; // not supported even type
//     }

//     /* After the alarm has been triggered
//       we need enable it again, so it is triggered the next time */
//     TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

//     /* Now just send the event data back to the main program task */
//     xQueueSendFromISR(timer_queue, &evt, NULL);
// }

// /*
//  * Initialize selected timer of the timer group 0
//  *
//  * timer_idx - the timer number to initialize
//  * auto_reload - should the timer auto reload on alarm?
//  * timer_interval_sec - the interval of alarm to set
//  */
// static void example_tg0_timer_init(timer_idx_t timer_idx,
//     bool auto_reload, double timer_interval_sec)
// {
//     /* Select and initialize basic parameters of the timer */
//     timer_config_t config;
//     config.divider = TIMER_DIVIDER;
//     config.counter_dir = TIMER_COUNT_UP;
//     config.counter_en = TIMER_PAUSE;
//     config.alarm_en = TIMER_ALARM_EN;
//     config.intr_type = TIMER_INTR_LEVEL;
//     config.auto_reload = auto_reload;
//     timer_init(TIMER_GROUP_0, timer_idx, &config);

//     /* Timer's counter will initially start from value below.
//        Also, if auto_reload is set, this value will be automatically reload on alarm */
//     timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

//     /* Configure the alarm value and the interrupt on alarm. */
//     timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
//     timer_enable_intr(TIMER_GROUP_0, timer_idx);
// 	timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr,
//         (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

//     timer_start(TIMER_GROUP_0, timer_idx);
// }

// /*
//  * The main task of this example program
//  */
// static void timer_example_evt_task(void *arg)
// {
//     while (1) {
//         timer_event_t evt;
//         xQueueReceive(timer_queue, &evt, portMAX_DELAY);

//         /* Print information that the timer reported an event */
//         if (evt.type == TEST_WITHOUT_RELOAD) {
//             printf("\n    Example timer without reload\n");
//         } else if (evt.type == TEST_WITH_RELOAD) {
//             printf("\n    Example timer with auto reload\n");
//         } else {
//             printf("\n    UNKNOWN EVENT TYPE\n");
//         }
//         printf("Group[%d], timer[%d] alarm event\n", evt.timer_group, evt.timer_idx);

//         /* Print the timer values passed by event */
//         printf("------- EVENT TIME --------\n");
//         print_timer_counter(evt.timer_counter_value);

//         /* Print the timer values as visible by this task */
//         printf("-------- TASK TIME --------\n");
//         uint64_t task_counter_value;
//         timer_get_counter_value((timer_group_t)evt.timer_group, evt.timer_idx, &task_counter_value);
//         print_timer_counter(task_counter_value);
//     }
// }
// //end alamr points




bool flagForFLowExecution = false; // bad way to do it

// future modification needed
#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>      // http://arduiniana.org/libraries/streaming/
//extern DS3232RTC RTC;
DS3232RTC RTC;

//bool somethingMehual = false;
bool mode = false;
float tempTemperature = 0;
int AC1Protocol;
int AC1Model;
int AC2Protocol;
int AC2Model;

#include "routine.h"
void actionDecoder(appWithAction jtempAA);
void SearchAndAttachSchedule();
inline void loadingCredentialsFromFile();
void SendPing(cJSON* AppliancesModulesJSON[], int countAppliancesModulesJSON);


//OTA
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
//#include "protocol_examples_common.h"
#include "string.h"

#include "nvs.h"
#include "nvs_flash.h"

//static const char *TAG = "simple_ota_example";

/*
make -j4
cp build/internal_communication.bin firmware.bin
sudo cp firmware.bin /var/www/html
make monitor

http://file.avinashi.com
suraj.singh@avinashi.com
6FrI7n7CS9wRBoZx

*/
uint8_t last_layer = 0;

#define OTA_URL_SIZE 256
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    return ESP_OK;
}


BaseType_t somethinf111;
BaseType_t somethinf112;
BaseType_t somethinf113;
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;


void simple_ota_example_task(void *pvParameter) {}

//All main Function Definitions
//void actionDecoder(appWithAction jtempAA);



#ifdef jAIR_CONDITIONER
#endif

void saveScheduleFile(jStr tempstr){
	tempstr = '/' + tempstr;
	cJSON *newSchedule = cJSON_CreateObject();
	cJSON_AddItemToObject(newSchedule, tempstr sss, cJSON_CreateNumber( 0 ));
	char *rendered = cJSON_PrintUnformatted(newSchedule);
	talk.coreFunc.jTool.jwriteFile( talk.coreFunc.jTool.JcurrentFlow, rendered );
	cJSON_Delete(newSchedule);
	free (rendered);
	flagForFLowExecution = true;
}

#ifdef jIRReceiver

//#define DECODE_AC

#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

const uint16_t kRecvPin = 19;
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;

#if DECODE_AC
const uint8_t kTimeout = 50;
#else   // DECODE_AC
const uint8_t kTimeout = 15;
#endif  // DECODE_AC
const uint16_t kMinUnknownSize = 12;

#define LEGACY_TIMING_INFO false
// ==================== end of TUNEABLE PARAMETERS ====================

// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results




#endif


#ifdef jACS712
cJSON* getACPowerStatusJSON(int address){

	powerData pData = {};
	talk.coreFunc.jTool.i2cMutex.lock();
		delay(1);	//https://arduino.stackexchange.com/questions/43007/why-is-a-delay1-necessary-before-wire-requestfrom
		Wire.requestFrom(address, sizeof(pData));
		Wire.readBytes( (uint8_t*)&pData, sizeof(pData) );
		delay(1200);
		//vTaskDelay(1200 / portTICK_RATE_MS);
	talk.coreFunc.jTool.i2cMutex.unlock();

	showf ("Current, Watts, kWH\t: ");
	show ( pData.iRMS );				show (", ");
	show ( String (pData.realPower) );		show (", ");
	showln ( String (pData.kWattHour, 4) );

//AC Power Meter
	cJSON* acPowerMeter = cJSON_CreateObject();
	if (address == 8)cJSON_AddItemToObject(acPowerMeter, "applianceId", cJSON_CreateString( "pwr1" ) );
	if (address == 9)cJSON_AddItemToObject(acPowerMeter, "applianceId", cJSON_CreateString( "pwr2" ) );
	cJSON_AddItemToObject(acPowerMeter, "type", cJSON_CreateString( "IoTick_PWR" ) );

	cJSON_AddItemToObject(acPowerMeter, "current", cJSON_CreateString( String(pData.iRMS) sss ) );
	cJSON_AddItemToObject(acPowerMeter, "Watts", cJSON_CreateString( String(pData.realPower, 2)  sss ) );
	cJSON_AddItemToObject(acPowerMeter, "kWH", cJSON_CreateString( String(pData.kWattHour, 4)  sss ) );

	return acPowerMeter;
}
#endif

#ifdef __cplusplus
extern "C"
{
	#endif

	#ifdef jDS18B20
		#include "ds18b20.h"
	#endif

	#ifdef __cplusplus
}
#endif


#ifdef jDS18B20
cJSON* getDS18B20StatusJSON(){

	float tempC = 0;
	for (int lp1 = 0; lp1 < 4; lp1++){
		tempC = ds18b20_get_temp();
		if (tempC < 100) break;
//		else {
//			showln ("somehogn for thtesshdsa dahwegrehrj");
//		}
		vTaskDelay(500 / portTICK_RATE_MS);
	}

	showln (tempC);
	tempTemperature = tempC;

	cJSON* DS18B20json = cJSON_CreateObject();
	cJSON_AddItemToObject(DS18B20json, "applianceId", cJSON_CreateString( "appliance5" ) );
	cJSON_AddItemToObject(DS18B20json, "type", cJSON_CreateString( "IoTick_Temp" ) );
	cJSON_AddItemToObject(DS18B20json, "temperature", cJSON_CreateString( String(tempC) sss ) );
	return DS18B20json;
}

#endif

#ifdef jDHT22

//#ifdef jDHT22

//DH22

// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 18     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

//#endif

cJSON* getDH22StatusJSON(){

	bdh22 abc = {};
	int lp1;
	for(lp1 = 0; lp1 < 5; lp1++){

		Serial.println( "Requesting DHT22 getData " + String(lp1) );
		esp_task_wdt_reset();
		vTaskDelay(2000 / portTICK_RATE_MS);

		bool letsExit1 = false;
		bool letsExit2 = false;

        sensors_event_t event;
        dht.temperature().getEvent(&event);
        if (isnan(event.temperature)) {
//          Serial.println(F("Error reading temperature!"));
        }
        else {
          Serial.print(F("Temperature: "));
          Serial.print(event.temperature);
          Serial.println(F("°C"));

          abc.tempe = event.temperature;
          letsExit1 = true;
        }

// Get humidity event and print its value.
        dht.humidity().getEvent(&event);
        if (isnan(event.relative_humidity)) {
//          Serial.println(F("Error reading humidity!"));
        }
        else {
          Serial.print(F("Humidity: "));
          Serial.print(event.relative_humidity);
          Serial.println(F("%"));
         abc.humu = event.relative_humidity;
          letsExit2 = true;
        }

        if (
        		(abc.humu > 0) &&
				(abc.tempe > 0) &&
				letsExit1 && letsExit2
				)
        {
        	Serial.println ("Exiting from loop " + String(lp1));
        	break;
        }
	}

	Serial.println("DH22 get tried\t: " +  String(lp1));
// temporaruy added
	tempTemperature = abc.tempe;

	cJSON* dht22json = cJSON_CreateObject();
	cJSON_AddItemToObject(dht22json, "applianceId", cJSON_CreateString( "appliance1" ) );
	cJSON_AddItemToObject(dht22json, "type", cJSON_CreateString( "IoTick_TH" ) );

	cJSON_AddItemToObject(dht22json, "temperature", cJSON_CreateString( String(abc.tempe, 2) sss ) );
	cJSON_AddItemToObject(dht22json, "humidity", cJSON_CreateString( String(abc.humu, 2)  sss ) );
	return dht22json;

}
#endif



#define FORMAT_SPIFFS_IF_FAILED true

#define auxiliaryButton 36
#define BuiltInLED 2
#define interruptPin 23
volatile int interruptCounter = 0;
int numberOfInterrupts = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR handleInterrupt()
{
  portENTER_CRITICAL_ISR(&mux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&mux);
}

void jEventHandler(void *pvParameter) {
    INFO("");

    #undef showlog
    #define showlog 1

    #undef showln
    #undef show
    #if showlog==1
        #define showln(x)   Serial.println(x);
        #define show(x)     Serial.print(x);
    #else
        #define showln(x)   ;
        #define show(x)     ;
    #endif

    INFO("Temporary Stopped");
//  time_t xx = millis();
    while (true){

//      if (( xx + 1 * 1000 ) < millis()){
//          xx = millis();
//
//          showln("hi suraj how are you");
//          for (auto x : talk.coreFunc.jTask){
//              showln (x.interval);
//              showln (x.conditionsToEvaluate sss);
//          }
//
//      }

        esp_task_wdt_reset();
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void myTestThread (void *pvParameter) {

    int applianceCounter = 0;
    cJSON* jsonAppliancesModules[maximumApplianceTypesHandle];

#ifdef jDHT22
    jsonAppliancesModules[applianceCounter++] = getDH22StatusJSON();
#endif

#ifdef jDS18B20
    jsonAppliancesModules[applianceCounter++] = getDS18B20StatusJSON();
#endif

#ifdef jACS712
    jsonAppliancesModules[applianceCounter++] = getACPowerStatusJSON(8);
    jsonAppliancesModules[applianceCounter++] = getACPowerStatusJSON(9);
#endif

#ifdef jAIR_CONDITIONER
    jsonAppliancesModules[applianceCounter++] = talk.coreFunc.Get_Appliance_Status(0);
    jsonAppliancesModules[applianceCounter++] = talk.coreFunc.Get_Appliance_Status(1);
#endif

    SendPing(jsonAppliancesModules, applianceCounter);

    if(TaskHandle_1) vTaskDelete(TaskHandle_1);    // Delete the task using the TaskHandle_1

    while (true){
        esp_task_wdt_reset();
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}


void jpause (){
    while (true)
    {
        esp_task_wdt_reset();
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}


struct test{
    //std::vector<int> some;
    int some[10]={};
    //jStr som;
    char som[30]={};
    int x;
    float ash;
};

void myCodeStartFromHere(){

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
    }

    Serial.begin (115200, SERIAL_8N1);   while(!Serial);
    showln ("MAC ID\t: " + String (talk.coreFunc.jTool.deviceMacIDStr sss));

	printf (SPIFFS.begin(true) ? "SPIFFS Mount successful\n" : "SPIFFS Mount Failed\n");

    

    RESET_REASON reset_reason = rtc_get_reset_reason(0);
    jp(reset_reason)


//  sensors.begin();    //DS18B20 ? //dont know blokcing it then working otherwise not working
//----------------------POWER METER---------------------
//#ifdef jACS712
    Wire.begin();       //Begins I2C communication at pin (A4,A5)   //Power mesuremnt
//#endif
    RTC.begin();

#ifdef jDS18B20
    ds18b20_init(18);
#endif

//    https://github.com/JChristensen/DS3232RTC/blob/master/examples/SetSerial/SetSerial.ino
    setSyncProvider(RTC.get);
    setSyncInterval(10);

    if (timeStatus() != timeSet) {
        showln("RTC Sync Failed");
//      Serial << F(" FAIL!");
    }
    else {
        showln("RTC Sync Done");
    }
#ifdef jDHT22
////DH22
    dht.begin();
    Serial.println(F("DHTxx Unified Sensor Example"));
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
    // Set delay between sensor readings based on sensor details.
//    delayMS = sensor.min_delay / 1000;

#endif

#ifdef jIRReceiver
#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(kBaudRate, SERIAL_8N1);
#endif  // ESP8266
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.enableIRIn();  // Start the receiver
#endif

    printf (RTC.alarm(ALARM_1) ? "Alarm 1 resetting\n" : "Alarm 1 not set\n");
    printf (RTC.alarm(ALARM_2) ? "Alarm 2 resetting\n" : "Alarm 2 not set\n");


    pinMode (interruptPin   , INPUT_PULLUP);    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
    pinMode (auxiliaryButton, INPUT);
    pinMode (BuiltInLED     , OUTPUT);


    talk.coreFunc.jTool.jdeleteFile(talk.coreFunc.jTool.jFlowStatusFile);
    SearchAndAttachSchedule();
    showln ("Schedule File Contain\t: " + String (talk.coreFunc.jTool.jreadFile(talk.coreFunc.jTool.jScheduleStatusFile)sss));


    unsigned long int someTime = millis();
    unsigned long int TempTime = 0;

    #define specialUpperSc  1
    #define specialLowerSc -1
    #define normalSc        0
    int someVarible         = normalSc;
    int pressDelayCounter   = 0;
//  long temporarySomthi    = 0;

//    xTaskCreate(&jEventHandler, "jEventHandler", 8*1024, NULL, 5, 0);



    while (true)
    {
        esp_task_wdt_reset();
        vTaskDelay(100 / portTICK_RATE_MS);

/*        if (!digitalRead (auxiliaryButton)){
            pressDelayCounter++;
            showln(pressDelayCounter);
            if (pressDelayCounter > 20 && pressDelayCounter < 30){
                digitalRead (BuiltInLED) ? digitalWrite (BuiltInLED, LOW) : digitalWrite (BuiltInLED, HIGH);
            }else if (pressDelayCounter > 30){
                showln("Formatting Device");
                talk.coreFunc.jTool.jformat();
                showln("Formatting Device complete");
                delay(5000); 
            }
        }else{
            pressDelayCounter = 0;
            digitalWrite (BuiltInLED, LOW);
        }
*/
#ifdef jIRReceiver

        // Check if the IR code has been received.
          if (irrecv.decode(&results)) {
            // Display a crude timestamp.
            uint32_t now = millis();
            Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
            // Check if we got an IR message that was to big for our capture buffer.
            if (results.overflow)
              Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
            // Display the library version the message was captured with.
            Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
            // Display the basic output of what we found.
            Serial.print(resultToHumanReadableBasic(&results));
            // Display any extra A/C info if we have it.
            String description = IRAcUtils::resultAcToString(&results);
            if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
            yield();  // Feed the WDT as the text output can take a while to print.
        #if LEGACY_TIMING_INFO
            // Output legacy RAW timing info of the result.
            Serial.println(resultToTimingInfo(&results));
            yield();  // Feed the WDT (again)
        #endif  // LEGACY_TIMING_INFO
            // Output the results as source code
            Serial.println(resultToSourceCode(&results));
            Serial.println();    // Blank line between entries
            yield();             // Feed the WDT (again)
          }

#endif

//Restart By API
        if (talk.coreFunc.jTool.restartBit){
            talk.coreFunc.jTool.restartBit = false;
            ESP.restart();
        }

//Schedule Refresh By API
        if ( talk.coreFunc.RefreshSchedule ){
            talk.coreFunc.RefreshSchedule = false;
            showln ("RefreshSchedule");
            SearchAndAttachSchedule();
        }

//Flow Refresh
        if ( talk.coreFunc.flowFlag ){
            talk.coreFunc.flowFlag = false;
            showln ("RefreshFlow");
            talk.coreFunc.jFlowHandler();
        }




//Upgrade By API
        if ( talk.coreFunc.UpgradeDevice ){ // reset by task it self
            talk.coreFunc.UpgradeDevice = false;
            //somethinf113 = xTaskCreate(&simple_ota_example_task, "ota_example_task", 8*1024, NULL, 5, &TaskHandle_3);
        }

// Time display
        if (( TempTime + 1 * 1000 ) < millis()){
            TempTime = millis();
//          showln ("CurrentTime\t: " +
//                  String (
//                          talk.coreFunc.printDateTime(
//                                  talk.coreFunc.getSysTime()) sss) );
//          int currentTemperature = map(analogRead(33), 1, 4*1024, 10, 30);
            int currentTemperature = tempTemperature;
            if (currentTemperature == 0){
//              show ("Sensor problem");
            }else {

//              Serial.println (currentTemperature);

                if (someVarible == normalSc){
                    if ( currentTemperature > talk.coreFunc.jTool.maxTemperature ){
                        showln ("Need to attach SpecialUpper");
                        someVarible = specialUpperSc;
                        saveScheduleFile("Emergency_Upper");
                        talk.coreFunc.jScheduleHandler();   //talk.coreFunc.ScheduleHandler();
                    }
                    if ( currentTemperature < talk.coreFunc.jTool.minTemperature ){
                        showln ("Need to attach SpecialLower");
                        someVarible = specialLowerSc;
                        saveScheduleFile("Emergency_Lower");
                        talk.coreFunc.jScheduleHandler();   //talk.coreFunc.ScheduleHandler();
                    }
                } else if (someVarible != normalSc) {
                    if (someVarible == specialUpperSc){
                        if (talk.coreFunc.jTool.pivotTemperature > currentTemperature){
                            showln ("Temperature back to LOW, Let's attach daily Schedule");
                            someVarible = normalSc;
                            SearchAndAttachSchedule();
                            showln ("Schedule File Contain: -" + String (talk.coreFunc.jTool.jreadFile(talk.coreFunc.jTool.JcurrentFlow)sss));
                            talk.coreFunc.jScheduleHandler();   //talk.coreFunc.ScheduleHandler();
                        }
                    }
                    if (someVarible == specialLowerSc){
                        if (talk.coreFunc.jTool.pivotTemperature < currentTemperature){
                            showln ("Temperature back to HIGH, Let's attach daily Schedule");
                            someVarible = normalSc;
                            SearchAndAttachSchedule();
                            showln ("Schedule File Contain: -" + String (talk.coreFunc.jTool.jreadFile(talk.coreFunc.jTool.JcurrentFlow)sss));
                            talk.coreFunc.jScheduleHandler();   //talk.coreFunc.ScheduleHandler();
                        }
                    }
                }
            }

        }

//Ping send Interval
/*        if (( someTime + talk.coreFunc.jTool.pingTime * 10000 ) < millis()){
            showln ("PING_EVENT_SEND");

//          time_t cTime = talk.coreFunc.getSysTime();
//          showln ( String (talk.coreFunc.printDateTime(cTime) sss) );
            somethinf111 = xTaskCreate(&myTestThread, "something", 10*1024, NULL, 1, &TaskHandle_1);
            someTime = millis();
        }
*/

         if (flagForFLowExecution) {
            flagForFLowExecution = false;
            talk.coreFunc.jScheduleHandler();
         }

         if(interruptCounter>0) {
            portENTER_CRITICAL(&mux);
            interruptCounter--;
            portEXIT_CRITICAL(&mux);
            numberOfInterrupts++;
            showln ( "[---------------- ** Alarm Triggered ** ----------------]" );

            if ( RTC.alarm(ALARM_1) )   //Flow_ALARM_1
            {
                time_t currentTime = talk.coreFunc.getSysTime();
                showln ("ALARM_1_Flow\t: "
                        + String (talk.coreFunc.printDateTime( currentTime ) sss) );

                actionDecoder(talk.coreFunc.Action_Temporary_Store_Flow);
                showln("jFlowHandler Refreshing");
                talk.coreFunc.jFlowHandler();

            }

            if ( RTC.alarm(ALARM_2) )    //Schedule_ALARM_2
            {
                time_t currentTime = talk.coreFunc.getSysTime();
                showln ("ALARM_2_Schedule\t: "+
                        String (talk.coreFunc.printDateTime( currentTime ) sss));

                struct tm *stTim    = localtime(&currentTime);
                if (
                        stTim->tm_hour  == 0 &&
                        stTim->tm_min   == 0 &&
                        stTim->tm_sec   < 30
                    )
                {
                    INFO ("Mid night Let's attach Todays Schedule");
                    SearchAndAttachSchedule();
                }
                //this could be loop if multiple appliaces are their
                actionDecoder(talk.coreFunc.Action_Temporary_Store_Schedule);

                //F- only insert to file need to refrash here
                jStr firstTwoChar = talk.coreFunc.Action_Temporary_Store_Schedule.action.substr(0, 2);
                if (firstTwoChar == "F-") {
                    showln("jFlowHandler Refreshing");
                    talk.coreFunc.jFlowHandler();
                }

                flagForFLowExecution = true;
            }

         }

    }


}

#endif /* MAIN_MAIN_H_ */

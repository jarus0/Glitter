/*
 * FirmwareType.h
 *
 *  Created on: 06-Aug-2020
 *      Author: suraj
 */

#ifndef MAIN_FIRMWARETYPE_H_
#define MAIN_FIRMWARETYPE_H_

//Supported Devices Enable here
#define maximumApplianceTypesHandle 10
//#define	jDHT22			//working
//#define	jDS18B20		//working
//#define jACS712			// not working
#define jAIR_CONDITIONER	// not working
//#define jIRReceiver 		// not working

//Communication Mode
//#define jwebsocketSupport
#define jmqttSupport
//#define jBLESupport

//#define office
#define jarus
//#define otherperson

#endif /* MAIN_FIRMWARETYPE_H_ */

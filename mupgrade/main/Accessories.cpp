/*
 * Accessories.cpp
 *
 *  Created on: 17-Sep-2020
 *      Author: suraj
 */

#include "Accessories.h"

Accessories::Accessories() {
	// TODO Auto-generated constructor stub
	uint8_t staMac[6];	esp_read_mac( staMac, ESP_MAC_WIFI_STA);
	char macChar[18];	sprintf(macChar,MACSTR,MAC2STR(staMac));
	deviceMacIDStr = macChar;

	Serial.println( "MAC id :- " + String (deviceMacIDStr .c_str()) );

	funList["iEqual"] = &Accessories::iEqual;
	funList["isLess"] = &Accessories::isLess;
	funList["isMore"] = &Accessories::isMore;
	funList["OrOrOr"] = &Accessories::OrOrOr;
	funList["isjAnd"] = &Accessories::isjAnd;
	funList["invert"] = &Accessories::invert;
	funList["adding"] = &Accessories::adding;
}

Accessories::~Accessories() {
	// TODO Auto-generated destructor stub
}


template<typename T>
std::string Accessories::toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}


String Accessories::getValueBySplit(String data, char separator, int index)
{
//	Future task, optimised way to do > https://www.geeksforgeeks.org/how-to-split-a-string-in-cc-python-and-java/
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String Accessories::getCurrentFileName(String FullFileAddress)
{
  for (int myloop = 0; myloop < 20; myloop++ )
  {
    if (getValueBySplit(FullFileAddress, (char)47, myloop).equals("") && (myloop != 0) )
    {
      return getValueBySplit(FullFileAddress, (char)47, myloop-1);
    }
  }
  return "-1";
}

void Accessories::info(jStr StrFile, jStr Strline, jStr Strfunc, jStr StrMsg)
//void Accessories::info(jStr StrDate, jStr StrTime, jStr StrFile, jStr Strline, jStr Strfunc, jStr StrMsg)
{

//	heap_caps_dump_all();
	showlnf("");
//	static uint32_t totalSize = ESP.getHeapSize();
//	float inPer = (float) 100 * esp_get_free_heap_size() / totalSize;
	//float inPer = esp_get_free_heap_size() / 1024;
	float inPer = (float)ESP.getFreeHeap() / 1024;
//	float inPer = ESP.getFreeHeap();

/*
	jsonPair fileInfo;

	if ( StrMsg.compare("msg") != 0)
	{
		fileInfo["msg"] = StrMsg;
	}

	fileInfo["date"] = StrDate;
	fileInfo["time"] = StrTime;
	fileInfo["file"] = getCurrentFileName(StrFile.c_str()).c_str();
	fileInfo["line"] = Strline;
	fileInfo["func"] = Strfunc;
	fileInfo["func"] = Strfunc;


	fileInfo["line"] = StrArg;


	100 * esp_get_free_heap_size() / totalSize

	float heapSize = (float) totalSize / (float) esp_get_free_heap_size() / 1000;

	fileInfo["heap"] = String (inPer).c_str() + jStr ("%");
	fileInfo["total heap"] = String ( ).c_str();
	Serial.println(ESP.getFreeHeap());
	JJLOG.println(String (esp_get_free_heap_size()).c_str());
	JJLOG.print("Jlog\t: ");
	JJLOG.println (getJsonFromJP(&fileInfo).c_str());
*/
//	Serial.println("abc");

	showf ("File|Func|Line|Heap|Msg\t: ");
	show ( getCurrentFileName(StrFile.c_str()).c_str());			showf ( "|" );
	show ( Strfunc 	.c_str());				showf ( "|" );
	show ( Strline 	.c_str() );				showf ( "|" );
//	JJLOG.print ( String (inPer) + "%" );			JJLOG.print ( "|" );
	show ( String (inPer, 3) );			showf ( "KB |" );
	show ( StrMsg 	.c_str() ); 			showlnf ( "" );


//
//	JJLOG.print ( "Heap:" ); JJLOG.print ( String (inPer) + "%" );			JJLOG.print ( "\t\t" );
//	JJLOG.print ( "Line:" ); JJLOG.print ( Strline 	.c_str() );				JJLOG.print ( "\t\t" );
//	JJLOG.print ( "Func:" ); JJLOG.print ( Strfunc 	.c_str());				JJLOG.print ( "\t\t" );
//	JJLOG.print ( "File:" ); JJLOG.print ( getCurrentFileName(StrFile.c_str()).c_str());			JJLOG.print ( "\t" );
//	JJLOG.print ( "MSG :" ); JJLOG.print ( StrMsg 	.c_str() ); JJLOG.println ( "" );

}


inline void Accessories::block1()
{
	GlobalMutexLU11.lock();
	//	irrecv.disableIRIn();
}
inline void Accessories::unblock1()
{
	GlobalMutexLU11.unlock();
	//    irrecv.enableIRIn();
}

//void Accessories::getIRinstance(IRrecv *tempIRrecv)
//{
//	ptrIRrecv1 = tempIRrecv;
//}

void Accessories::jwriteFile(jStr temp_path, jStr temp_message){

	//GlobalMutexLU.lock();irrecv.disableIRIn();
	block1();
	const char *path = temp_path.c_str();
	const char *message = temp_message.c_str();

	//PJJLOG.print("Writing file\t: " + String (temp_path.c_str()));

    File file = SPIFFS.open(path, FILE_WRITE);
    if(file){
        if( file.print(message) )
        {
        	//PJJLOG.println(" |File written :- " + String (temp_message.c_str()) );
        }
        else
        {
        	//PJJLOG.println(" |File write failed");
        }
    }
    else
    {
    	//PJJLOG.println("- failed to open file for writing");
    }
    file.close();
    //irrecv.enableIRIn();GlobalMutexLU.unlock();
    unblock1();

}

void Accessories::jappendFile(jStr temp_path, jStr temp_message){

//	irrecv.disableIRIn();
//	GlobalMutexLU.lock();
	block1();
	const char *path = temp_path.c_str();
	const char *message = temp_message.c_str();

	//PJJLOG.printf("Appending to file: %s\r\n", path);

    File file = SPIFFS.open(path, FILE_APPEND);
    if(!file){
        //PJJLOG.println("- failed to open file for appending");
        file.close();
        unblock1();
        //GlobalMutexLU.unlock();
        return;
    }
    if(file.print(message))
    {
    	//PJJLOG.println("- message appended");
    }
    else
    {
    	//PJJLOG.println("- append failed");
    }
    file.close();
//    GlobalMutexLU.unlock();
//    irrecv.enableIRIn();
    unblock1();
}



void Accessories::jrenameFile(jStr temp_path1, jStr temp_path2){
	block1();
//	irrecv.disableIRIn();
//	GlobalMutexLU.lock();
	const char *path1 = temp_path1.c_str();
	const char *path2 = temp_path2.c_str();
    //PJJLOG.printf("Renaming file %s to %s\r\n", path1, path2);

    if (SPIFFS.rename(path1, path2))
	{
		//PJJLOG.println("- file renamed");
	}
    else
    {
    	//PJJLOG.println("- rename failed");
    }
    unblock1();
//    GlobalMutexLU.unlock();
//    irrecv.enableIRIn();
}

void Accessories::jdeleteFile(jStr temp_path){
	block1();
//	irrecv.disableIRIn();
//	GlobalMutexLU.lock();
    const char *path = temp_path.c_str();

    //PJJLOG.print("Deleting "+ String (path));

	if( SPIFFS.remove(path) )
	{
		//PJJLOG.println(" deleted");
	}
	else
	{
		//PJJLOG.println(" can't delete");
	}
	unblock1();
//    GlobalMutexLU.unlock();
//    irrecv.enableIRIn();
}
void Accessories::jdeleteFolder(jStr temp_path){
//	irrecv.disableIRIn();
//	GlobalMutexLU.lock();
	block1();

    File root = SPIFFS.open(temp_path.c_str());
    if(root && root.isDirectory())
    {
    	//PJJLOG.println("deleting folder\t: " + String (root.name()) );
    	for (	File file = root.openNextFile();
    			file;
    			file = root.openNextFile())
    	{

			if(!file.isDirectory())
			{
				//PJJLOG.print("deleting file\t: "+ String (file.name()));
				if( SPIFFS.remove(file.name()) ) {
					//PJJLOG.println(" deleted");
				}
				else {
					//PJJLOG.println(" can't delete");
				}
			}
			else
			{
				//PJJLOG.print("deleting folder\t: "+ String (file.name()));
				if( SPIFFS.remove(file.name()) )
				{
					//PJJLOG.println(" deleted");
				}
				else {
					//PJJLOG.println(" can't delete");
				}
			}
			file.close();
    	}
    }
    root.close();
    unblock1();
//    GlobalMutexLU.unlock();
//    irrecv.enableIRIn();
}

jsonNested Accessories::jlistDir(jStr temp_dirname, uint8_t levels)
{
	block1();
//	irrecv.disableIRIn();
//	GlobalMutexLU.lock();
	// future some patiya is here solve in future
	jsonNested temp;

//	const char *dirname = temp_dirname.c_str();

    //PJJLOG.printf("Listing directory: %s\r\n", dirname);

    File root = SPIFFS.open(temp_dirname sss);
    if(!root){
        //PJJLOG.println("- failed to open directory");
        unblock1();
        //GlobalMutexLU.unlock();
        return temp;
    }
    if(!root.isDirectory()){
        //PJJLOG.println(" - not a directory");
        unblock1();//GlobalMutexLU.unlock();
        return temp;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            //PJJLOG.print("  DIR : ");
            //PJJLOG.println(file.name());
            if(levels){
            	jlistDir(file.name(), levels -1);
            }
        } else {
        	temp.push_back(file.name());
            //PJJLOG.print("  FILE: ");
            //PJJLOG.print(file.name());
            //PJJLOG.print("\tSIZE: ");
            //PJJLOG.println(file.size());
        }
        file = root.openNextFile();
    }
    unblock1();
//    GlobalMutexLU.unlock();
//    irrecv.enableIRIn();
    return temp;
}


jStr Accessories::jreadFile(jStr temp_path)
{
	block1();
	jStr returnStr = "";

    File file = SPIFFS.open(temp_path.c_str());
    if(file && !file.isDirectory()){
    	//returnStr = file.readBytesUntil('\n');
        while(file.available())
        	returnStr += (char) file.read();
        //PJJLOG.println("]Contain:- " + String (returnStr.c_str()) );

        if (returnStr.empty()){
        	Serial.println(String(temp_path sss) + " Empty");
//        	printf("%s", temp_path sss);
        }

    }
    else{
//    	showln("hi this is something");
//    	showln(temp_path sss);
    	Serial.println(String(temp_path sss) + " File not Found");
    }

    file.close();

    unblock1();

//    GlobalMutexLU.unlock();
    return returnStr;
}

void Accessories::jtestFileIO(jStr temp_path)
{
	block1();
//	irrecv.disableIRIn();
//	GlobalMutexLU.lock();
	const char *path = temp_path.c_str();
    JJLOG.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = SPIFFS.open(path, FILE_WRITE);
    if(!file){
        JJLOG.println("- failed to open file for writing");
        unblock1(); //GlobalMutexLU.unlock();
        return;
    }

    size_t i;
    JJLOG.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          JJLOG.print(".");
        }
        file.write(buf, 512);
    }
    JJLOG.println("");
    uint32_t end = millis() - start;
    JJLOG.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = SPIFFS.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        JJLOG.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              JJLOG.print(".");
            }
            len -= toRead;
        }
        JJLOG.println("");
        end = millis() - start;
        JJLOG.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        JJLOG.println("- failed to open file for reading");
    }

    unblock1();
//    GlobalMutexLU.unlock();
//    irrecv.enableIRIn();
}

void Accessories::jformat()
{
	block1();
//	irrecv.disableIRIn();
//	GlobalMutexLU.lock();
	if (SPIFFS.format()) {
		Serial.println("\n\nSuccess format");
		//talk.jlog("\n\nSuccess format");
	}
	else
	{
		Serial.println("\n\nError format");
	}
	unblock1();
//	GlobalMutexLU.unlock();
//	irrecv.enableIRIn();
}

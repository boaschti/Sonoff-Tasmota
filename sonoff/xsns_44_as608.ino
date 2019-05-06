// Conditional compilation of driver
#ifdef USE_AS608

// Define driver ID
#define XSNS_44  44

#include <TasmotaSerial.h>
#include <Adafruit_Fingerprint.h>


//SoftwareSerial mySerial(2, 3);

//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//TasmotaSerial *mySerial;

/*
TasmotaSerial mySerial(pin[GPIO_AS608_RX], pin[GPIO_AS608_TX]);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
*/


//TasmotaSerial *mySerial = NULL;

bool as608selected = false;


void as608init(){

    if ((pin[GPIO_AS608_RX] < 99) && (pin[GPIO_AS608_TX] < 99)){
      as608selected = true;
    }

    Adafruit_Fingerprint finger = Adafruit_Fingerprint(new TasmotaSerial(pin[GPIO_AS608_RX], pin[GPIO_AS608_TX], 0));

    finger.begin(57600);

    if (finger.verifyPassword()){
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS608x found"));
    }else{
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS608x not found! Got data: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"), finger.mydata[0], finger.mydata[1], finger.mydata[2], finger.mydata[3], finger.mydata[4], finger.mydata[5], finger.mydata[6], finger.mydata[7], finger.mydata[8], finger.mydata[9], finger.mydata[10], finger.mydata[11]);
    }
    AddLog(LOG_LEVEL_INFO);

}

void as608everySecond(){
    //mySerial->print("Hallo");
    //finger.print("Hallo");
}



/**
 * The callback function Xsns44() interfaces Tasmota with the sensor driver.
 *
 * It provides the Tasmota callback IDs.
 *
 * @param   byte    callback_id  Tasmota function ID.
 * @return  boolean              Return value.
 * @pre     None.
 * @post    None.
 *
 */

 /*********************************************************************************************\
  * Interface
 \*********************************************************************************************/

 bool Xsns44(uint8_t function)
 {
   bool result = false;


     switch (function) {
       case FUNC_INIT:
          as608init();
         break;
       case FUNC_EVERY_250_MSECOND:
         break;
      case FUNC_EVERY_SECOND:
         as608everySecond();
       case FUNC_JSON_APPEND:
         break;
       case FUNC_SERIAL:
         break;
   }
   return result;
 }

 #endif

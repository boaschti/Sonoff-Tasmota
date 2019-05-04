// Conditional compilation of driver
#ifdef USE_AS608

// Define driver ID
#define XSNS_43  43

#include <TasmotaSerial.h>
#include <Adafruit_Fingerprint.h>


//SoftwareSerial mySerial(2, 3);

//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//TasmotaSerial *mySerial;

/*
TasmotaSerial mySerial(pin[GPIO_AS608_RX], pin[GPIO_AS608_TX]);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
*/

TasmotaSerial *mySerial = NULL;

bool as608selected = false;


void as608init(){

    if ((pin[GPIO_AS608_RX] < 99) && (pin[GPIO_AS608_TX] < 99)){
      as608selected = true;
    }

    Adafruit_Fingerprint finger = Adafruit_Fingerprint(new TasmotaSerial(pin[GPIO_AS608_RX], pin[GPIO_AS608_TX], 1));


    finger.begin(57600);
    finger.verifyPassword();

}

void as608everySecond(){
    //mySerial->print("Hallo");
    //finger.print("Hallo");
}



/**
 * The callback function Xsns43() interfaces Tasmota with the sensor driver.
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

 bool Xsns43(uint8_t function)
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

 #ifdef USE_WEBSERVER
       case FUNC_WEB_APPEND:
         break;
 #endif  // USE_WEBSERVER
   }
   return result;
 }

 #endif

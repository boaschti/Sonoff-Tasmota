// Conditional compilation of driver
#ifdef USE_AS608

// Define driver ID
#define XSNS_44  44

#include <TasmotaSerial.h>
#include <Adafruit_Fingerprint.h>


bool as608selected = false;
static uint8_t enrollstep = 0;
static uint8_t ModellNumber = 0;



void as608init(){

    if ((pin[GPIO_AS608_RX] < 99) && (pin[GPIO_AS608_TX] < 99)){

        as608selected = true;

        Adafruit_Fingerprint finger = Adafruit_Fingerprint(new TasmotaSerial(pin[GPIO_AS608_RX], pin[GPIO_AS608_TX], 0));

        finger.begin(57600);

        if (finger.verifyPassword()){
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS60x found"));
        }else{
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS60x not found! Got data: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"), finger.mydata[0], finger.mydata[1], finger.mydata[2], finger.mydata[3], finger.mydata[4], finger.mydata[5], finger.mydata[6], finger.mydata[7], finger.mydata[8], finger.mydata[9], finger.mydata[10], finger.mydata[11]);
        }
        AddLog(LOG_LEVEL_INFO);
    }

}

uint8_t as608Enroll(uint8_t nr){
     // set Variable to 1rst step
    if (!enrollstep){
        enrollstep = 1;
    }


    return true;

}

int getFingerImage(){
  int p;
  //p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Image taken"));
      AddLog(LOG_LEVEL_INFO);
      break;
    case FINGERPRINT_NOFINGER:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "waiting"));
      AddLog(LOG_LEVEL_INFO);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Comm error"));
      AddLog(LOG_LEVEL_INFO);
      break;
    case FINGERPRINT_IMAGEFAIL:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Imaging error"));
      AddLog(LOG_LEVEL_INFO);
      break;
    default:
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "any error"));
      AddLog(LOG_LEVEL_INFO);
      break;
    }

  return p;
}

int convertFingerImage(uint8_t slot){
    int p;
    //p = finger.image2Tz(slot);
    switch (p) {
      case FINGERPRINT_OK:
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Image converted"));
        AddLog(LOG_LEVEL_INFO);
        break;
      case FINGERPRINT_IMAGEMESS:
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Image too messy"));
        AddLog(LOG_LEVEL_INFO);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Comm error"));
        AddLog(LOG_LEVEL_INFO);
        break;
      case FINGERPRINT_FEATUREFAIL:
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Could not find fingerprint features"));
        AddLog(LOG_LEVEL_INFO);
        break;
      case FINGERPRINT_INVALIDIMAGE:
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Could not find fingerprint features"));
        AddLog(LOG_LEVEL_INFO);
        break;
      default:
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "any error"));
        AddLog(LOG_LEVEL_INFO);
        break;
    }

    return p;
}

void as608Main(){
    if(!as608selected){
      return;
    }

    int p;

    if (!enrollstep){
      // search for Finger

    }else{
        // enroll is active
        switch (enrollstep){
        case 1:
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "place finger and wait"));
            AddLog(LOG_LEVEL_INFO);
            enrollstep++;
        case 2:
        // get first image
            if (getFingerImage() == FINGERPRINT_OK){
              enrollstep++;
            }
            break;
        case 3:
        // convert image
            if(convertFingerImage(1) == FINGERPRINT_OK){
                enrollstep++;
                break;
            }else{
                enrollstep--;
                break;
            }
            break;
        case 4:
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "remove finger and wait"));
            AddLog(LOG_LEVEL_INFO);
            enrollstep++;
        case 5:
        // Remove finger
            //p = finger.getImage();
            if (p == FINGERPRINT_OK){
                enrollstep++;
            }
        case 6:
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "place same finger again"));
            AddLog(LOG_LEVEL_INFO);
            enrollstep++;
        case 7:
        // get second image of finger
            if (getFingerImage() == FINGERPRINT_OK){
              enrollstep++;
            }
            break;
        case 8:
        // convert second image
            if(convertFingerImage(2) == FINGERPRINT_OK){
                enrollstep++;
                break;
            }else{
                enrollstep--;
                break;
            }
            break;

        // create modell

            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "creating model"));
            AddLog(LOG_LEVEL_INFO);

            //p = finger.createModel();
            if (p == FINGERPRINT_OK) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Prints matched"));
              AddLog(LOG_LEVEL_INFO);
            } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Communication error"));
              AddLog(LOG_LEVEL_INFO);
              enrollstep = 99;
            } else if (p == FINGERPRINT_ENROLLMISMATCH) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Fingerprints did not match"));
              AddLog(LOG_LEVEL_INFO);
              enrollstep = 99;
            } else {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "any error"));
              AddLog(LOG_LEVEL_INFO);
              enrollstep = 99;
            }

        // store modell

            //p = finger.storeModel(ModellNumber);
            if (p == FINGERPRINT_OK) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Stored!"));
              AddLog(LOG_LEVEL_INFO);
              enrollstep = 0;
              ModellNumber = 0;
            } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Communication error"));
              AddLog(LOG_LEVEL_INFO);
              enrollstep = 99;
            } else if (p == FINGERPRINT_BADLOCATION) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Could not store in that location"));
              AddLog(LOG_LEVEL_INFO);
              enrollstep = 99;
            } else if (p == FINGERPRINT_FLASHERR) {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Error writing to flash"));
              AddLog(LOG_LEVEL_INFO);
              enrollstep = 99;
            } else {
              snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "any error"));
              AddLog(LOG_LEVEL_INFO);
              enrollstep = 99;
            }
        case 99:
            enrollstep = 1;
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "enroll starts again!"));
            AddLog(LOG_LEVEL_INFO);
        default:
            enrollstep = 0;
            ModellNumber = 0;
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "oops sth went wrong"));
            AddLog(LOG_LEVEL_INFO);
        }
    }
}


bool as608Command(void){
    if (XSNS_44 == XdrvMailbox.index) {

        // convert space or = to , to get mor compatibility
        for (uint8_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
          if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
        }

        char sub_string[XdrvMailbox.data_len];
        // Test if enroll is active
        if (!enrollstep){
          // look for command enroll and get number
          if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"enroll")) { // Note 1 used for param number
                uint8_t ModellNumber = atoi(subStr(sub_string, XdrvMailbox.data, ",", 2));  // Note 2 used for param number
                snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS60x Enroll called #%i"), ModellNumber);
                AddLog(LOG_LEVEL_INFO);
                as608Enroll(ModellNumber);
                return true;
          }else{
                snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS60x Enroll active! Cancel with: enroll reset"));
                AddLog(LOG_LEVEL_INFO);
                return false;
          }
        }





        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"test")) { // Note 1 used for param number
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS60x test"));
          AddLog(LOG_LEVEL_INFO);
          return true;
        }
    }

}

void as608everySecond(){
    //mySerial->print("Hallo");
    //finger.print("Hallo");
}

/*
mqtt published
snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"MPR121%c\":{\"Button%i\":%i}}"), pS->id[i], j, BITC(i,j));
MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);

mqtt received

*/





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
          as608Main();
          break;
      case FUNC_EVERY_SECOND:
          as608everySecond();
      case FUNC_JSON_APPEND:
          break;
      case FUNC_SERIAL:
          break;
      case FUNC_COMMAND_SENSOR:
          result = as608Command();
          break;
   }
   //return result;
   return result;
 }

 #endif

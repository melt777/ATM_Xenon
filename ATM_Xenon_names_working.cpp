/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "e:/aacrs/02_12/markvisser/git_clone/ATM_XENON/ATM_Xenon/ATM_Xenon_names_working.ino"

#include "Particle.h"
#include "Ubidots.h"
void Name_of_Device(const char *topic, const char *data);
inline void softDelay(uint32_t msDelay);
void DetonateAction(const char *event, const char *data);
int cloudResetFunction(String command);
void setup();
void loop();
void timerUbidotInformation(void);
#line 4 "e:/aacrs/02_12/markvisser/git_clone/ATM_XENON/ATM_Xenon/ATM_Xenon_names_working.ino"
#define UBIDOTS_TOKEN "BBFF-h7usBVZlFXpogFgoPzdCbAwC6pxy31"

// Argon e00fce68e65d8787c17b5bdc Name: CRSgateway
// Xenon e00fce6813a4279e4cd79ca8 Name: crs can 1

// Argon e00fce688527f71c9ee9b0ef Name: 74zen
// Xenon e00fce689e3f887405d2cc19 Name: 74zen can 3
// burned xenon : e00fce68914fed94fc726d72  can 1 
// burned xenon : e00fce68a755ca49156869fd can 2

// Argon e00fce6883727dbb74be0c13 Name: Metal tech gateway
// Xenon e00fce6803061a4e7f19572a Name: Metal tech can 1


// Ubidots *******************************************************************
Ubidots ubidots(UBIDOTS_TOKEN, UBI_INDUSTRIAL, UBI_MESH);
const char* WEBHOOK_NAME = "Ubidots";
//Ubidots ubidots("webhook", UBI_PARTICLE);   =================check this
//ubidots wigets variables
float value = analogRead(A0); // variable
float EM_BEACON1_BATT = 0 ;
float EM_BEACON2_BATT = 0;

Timer timer(200000, timerUbidotInformation);// 5min timer to send info to ubidots
//*******************************************************************
//BLE 
#define SCAN_RESULT_COUNT 30
BleScanResult results[SCAN_RESULT_COUNT];
const size_t SCAN_RESULT_MAX = 30;
BleScanResult scanResults[SCAN_RESULT_MAX];

//*******************************************************************
//setup remote cloud reset
#define DELAY_BEFORE_REBOOT 2000
unsigned int rebootDelayMillis = DELAY_BEFORE_REBOOT;
unsigned long rebootSync = millis();
bool resetFlag = false;

//BLYNK *******************************************************************
char auth[] = "Jlx9iNCE2RGTdxNbScoY2adIgwEKpWif"; // authorisation token for blynk
char Device_Name_message [64];

char dev_name[32] = ""; // keeps the device name
char VARIABLE_LABEL[32] = ""; // keeps the device name
bool publishName = false; // to publish the name


// used for getting the device name ***********************************************
void Name_of_Device(const char *topic, const char *data) {
  strncpy(dev_name, data, sizeof(dev_name)-1);
  Serial.printlnf("received in supscribe/publish request%s: %s", topic, dev_name);
  publishName = true;
  strncpy(VARIABLE_LABEL, dev_name, sizeof(dev_name));
  Serial.printlnf("variable label  that is copied from dev_name : %s", VARIABLE_LABEL);
  float voltage = analogRead(BATT) * 0.0011224;
  Particle.publish("voltage", String::format("%.2f",voltage), PRIVATE);
    ubidots.add(VARIABLE_LABEL, voltage); //pair Vbatt with name of Xenon
  Serial.printlnf("variable label that is uploaded to ubidots :  %s", VARIABLE_LABEL);
  ubidots.meshPublishToUbidots("Ubidots"); // webhook its publishing to on particle consol


}

/********************************************************************************************
*********************************************************************************************
*********************************************************************************************/
int led2 = D7;
bool DetonateSatus = 0; // holds the status for detonation.
SYSTEM_THREAD(ENABLED);
const char *EVENT_NAME = "Beacon MAC"; // here we need to change it to name of node if possible

inline void softDelay(uint32_t msDelay){
      for (uint32_t ms = millis(); 
            millis() - ms < msDelay;  
            Particle.process());
}

//XENON
void DetonateAction(const char *event, const char *data){
    
   if (strcmp (data, "Boom")==0){ 
      softDelay(3000);// wait 3 seconds 
      digitalWrite (led2, HIGH);
      DetonateSatus=1; // shows it has detonated
     
      Particle.publish("BOOM!",PRIVATE);
      Mesh.publish ("Detonate_ack","Boom_ack"); // publish to all subscriber to detonate
      softDelay(3000);
    }
   //Serial.println (data);
    if (strcmp (data, "Boom_reset")==0){  // turn led off as well as detonator voltage
      digitalWrite (led2, LOW);
      DetonateSatus=1; // shows it has detonated
      Particle.publish("BOOM_reset",PRIVATE);
      softDelay(2000);
    }
} 

//  Remote Reset Function
int cloudResetFunction(String command){
    resetFlag = true;
    rebootSync = millis();
    return 0;
}

void setup() {
  SYSTEM_MODE(AUTOMATIC);
  Particle.keepAlive(20);
  timer.start(); // start timer to send info to ubidots once per hour or whatever its set to
  Serial.begin(115200);
  pinMode(led2, OUTPUT); // output for triggered led
  BLE.setTxPower(-8); // Use lower power // Use lower power -20, -16, -12, -8, -4, 0, 4, 8.
  //Remote Reset Particle Function, to reset device from the cloud
  Particle.function("reset", cloudResetFunction);//Remote Reset Particle Function, to reset device from the cloud
  Mesh.subscribe("Detonate", DetonateAction);
  pinMode(BATT, INPUT);
  ubidots.setDebug(true);  // Uncomment this line for printing debug messages
}
        
void loop() {
  
} // end of main

void timerUbidotInformation(void){
  
  
 //
  float voltage = analogRead(BATT) * 0.0011224;
 // Particle.publish("voltage", String::format("%.2f",voltage), PRIVATE);
  // get the device name    
  
   waitUntil(Particle.connected);
  Particle.subscribe("particle/device/name", Name_of_Device);
  softDelay(3000);
  Particle.publish("particle/device/name");  // ask the cloud for the name to be sent to you
 //  softDelay(10000);
 // ubidots.add(VARIABLE_LABEL, voltage); //pair Vbatt with name of Xenon
 // Serial.printlnf("variable label that is uploaded to ubidots :  %s", VARIABLE_LABEL);
  //ubidots.meshPublishToUbidots("Ubidots"); // webhook its publishing to on particle consol
  

}
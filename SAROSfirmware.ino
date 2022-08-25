
/*
Sketch: SAROS Irrigation Scheduler

Developer: Kennedy Nganga

Date: April 2017

Seloto, Babati, Tanzania
*/


///////////////////////////////COMMUNICATION CODE//////////////////////////////////////


include <UbidotsFONA.h>
include <SoftwareSerial.h>


define APN  "Vodacom Internet"  // The APN of your operator
define USER ""  // if your apn doesnt have username just leave it ""
define PASS ""  // if your apn doesnt have password just leave it ""
define TOKEN "X8AhGZui1hq8m4xJSkhQJwQkB5xXwP7"  // Replace it with your Ubidots token

Ubidots client(TOKEN);  


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



//////////////////////PAYLOAD/////////////////////////////

//Declaring irrigation counter variables
int ic1 = 0;  //Irrigation counter 1

int ic2 = 0; //Irrigation counter 2

////////////////////////DHT22/////////////////////
#include "DHT.h"
#define DHTPIN 53     // Temperature and Humidity input pin
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);  //Initialize DHT sensor

/////////////////////////

///////////4 Channel  Relay to control irrigation ////////////////////////////
// Declaring and initializing variables

 int Rly1 = 32;
 int Rly2 = 34;
 int Rly3 = 36;
 int Rly4 = 38;
////////////////////////////

void setup() {


  
  //////////////////////////////////// COMMS CODE //////////////////////

  Serial.begin(115200);
  delay(2000);
  while(!client.setApn(APN, USER, PASS));


  /////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////

  

///////////////////////////////////////PAYLOAD//////////////////////
  
  Serial.begin(9600);

  ////////////Relay///////////////

  pinMode(Rly1, OUTPUT);
  pinMode(Rly2, OUTPUT);
  pinMode(Rly3, OUTPUT);
  pinMode(Rly4, OUTPUT);


  /////////////////////////////////////////

  ////////////////Soil moisture/////
pinMode(A14, INPUT);
pinMode(A11, INPUT);

////////////////DHT22 sensor
Serial.begin(9600);
  dht.begin();

}

void loop() {
 

    //Turn off all the relays first
  digitalWrite(Rly1, HIGH);
  digitalWrite(Rly2, HIGH);
  digitalWrite(Rly3, HIGH);
  digitalWrite(Rly4, HIGH);

  /////////////////////////////////////////////


/////////////////////////Temperature and Humidity///////////////////

float hum = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temp = dht.readTemperature();

// Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(temp, hum, false);

  
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");

  
  Serial.println();
  

  ////////////////////////// Soil moisture //////////////////

//float readVH400(int A0) {
  // This function returns Volumetric Water Content by converting the analogPin value to voltage
  // and then converting voltage to VWC using the piecewise regressions provided by the manufacturer  
  
  // Read value and convert to voltage  

  /////////SoilMoisture1
  int sensor1DN = analogRead(A11);
  float sensorVoltage1 = sensor1DN*(3.0 / 1023.0);
  float soilmosV1 = sensorVoltage1;
  float VWC1;

   ///////SoilMoisture2
  int sensor2DN = analogRead(A14);
  float sensorVoltage2 = sensor2DN*(3.0 / 1023.0);
  float soilmosV2 = sensorVoltage2;
  float VWC2;

  ///////////Get Volumetric Water Content from Voltage
  
  // Calculate VWC1
  if(sensorVoltage1 <= 1.1) {
    VWC1 = 10*sensorVoltage1 -1;
  } else if(sensorVoltage1 > 1.1 && sensorVoltage1 <= 1.3) {
    VWC1 = 25*sensorVoltage1 -17.5;
  } else if(sensorVoltage1 > 1.3 && sensorVoltage1 <= 1.82) {
    VWC1 = 48.08*sensorVoltage1 -47.5;
  } else if(sensorVoltage1 > 1.82) {
    VWC1 = 26.32*sensorVoltage1 -7.89;
  }
  


   // Calculate VWC2
  if(sensorVoltage2 <= 1.1) {
    VWC2 = 10*sensorVoltage2 -1;
  } else if(sensorVoltage2 > 1.1 && sensorVoltage2 <= 1.3) {
    VWC2 = 25*sensorVoltage2 -17.5;
  } else if(sensorVoltage2 > 1.3 && sensorVoltage2 <= 1.82) {
    VWC2 = 48.08*sensorVoltage2 -47.5;
  } else if(sensorVoltage2 > 1.82) {
    VWC2 = 26.32*sensorVoltage2 -7.89;
  }
  


//Printing out the outputs

  
  Serial.print("SoilMoisture P1: ");
  Serial.print(VWC1);
  Serial.print(" ");

  
  Serial.print("SoilMoisture P2: ");
  Serial.print(VWC2);
  Serial.print(" ");


  Serial.println();


//Irrigation control section

 if ( VWC1 > 5 && VWC1 < 20 ){
  
   int n1 = 0;

    while (n1 < 120) {               //120 pulses totaling 600 seconds = 36 liters

  digitalWrite(Rly1, LOW);
  delay(5000);                       //5 sec pulse
  digitalWrite(Rly1, HIGH);
  delay(5000);                       //5 second rest

  
  Serial.print("n1 = ");
  Serial.print(n1);
  Serial.println( );

  n1 ++;

  }

  ic1 ++; //increment irrigation counter

  Serial.print("ic1 = ");
  Serial.print(ic1);
  Serial.print(" ");
  Serial.print("Irrigator 1 executed");
  Serial.println();
  
 }

 

 if ( VWC2 > 5 && VWC2 < 20 ){
  
   int n2 = 0;

    while (n2 < 120) {                //120 pulses per irrigation cycle. Approx. 36 liters

  digitalWrite(Rly2, LOW);
  delay(5000);                       //5 second pulse
  digitalWrite(Rly2, HIGH);
  delay(5000);                       //5 second rest

  
  Serial.print("n2 = ");
  Serial.print(n2);
  Serial.println( );

  n2 ++;

  }

  ic2 ++; //increment irrigation counter

  Serial.print("ic2 = ");
  Serial.print(ic2);
  Serial.print(" ");
  Serial.print("Irrigator 2 executed");
  Serial.println();
  
 }



 

  //////////////////////////////// COMMS CODE////////////////////////////

  
  //client.add("Soil_Temperature *C", temperature);
  //client.add("Sun_Brightness_Voltage", lum);
  client.add("Relative_Humidity %\t", hum);
  client.add("Ambient_Temperature *C", temp);
  client.add("Soil_Moisture_P1_VWC", VWC1);
  client.add("Soil_Moisture_P2_VWC", VWC2);
  


 // client.add("Soil_Moisture_P1_Voltage", soilmosV1);
 // client.add("Soil_Moisture_P2_Voltage", soilmosV2);
 // client.add("Wind_Speed_Voltage", wind);
  client.add("Heat_Index *C", hic); 

  
  client.sendAll();

///////////////////////////////////////////////////////////////


 
 
 delay(3600000);  // 60 minute delay before cycling code





}




#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "xplane.h"
#include "websock.h"

WiFiUDP Udp;

const int analog_ip = A0; //Naming analog input pin
const int rotary1A = D0;
const int rotary1B = D1;
const int button1 = D2;

const int rotary2A = D3;
const int rotary2B = D4;
const int button2 = D5;

int inputVal  = 0;        //Variable to store analog input values
int aLastState = 0;
int aState = 0;
int counter = 0;
int aLastState2 = 0;
int aState2 = 0;
int counter2 = 0;

const char* ssid = "";
const char* password = "";

//Refer https://github.com/nasa/XPlaneConnect/blob/81a1447d2a3d8de1c30fde4e471a7b40cbf361d6/C/src/xplaneConnect.c
//Refer http://simvim.com/b58/xdata.html
//Refer https://developer.x-plane.com/datarefs/


void sendData(char *name, int counter) {
  // float throttle =  ((float)counter/30.0f);
  // Serial.print("Throttle: ");
  // Serial.println(throttle);
  // sendDREF("sim/flightmodel/engine/ENGN_thro_override", throttle);
  // sendCMND("sim/flight_controls/flaps_up");
  sendThrottleToWebSockets(name, counter);
}

void setup() {

  pinMode (rotary1A,INPUT);
  pinMode (rotary1B,INPUT);
  pinMode (button1,INPUT);

  pinMode (rotary2A,INPUT);
  pinMode (rotary2B,INPUT);
  pinMode (button2,INPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Initialising");

  WiFi.begin(ssid, password);  

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

   //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP  

  aLastState = digitalRead(rotary1A);
  startWebServer();
}

void checkRotartor1() {
   if(digitalRead(button1) == 0 && counter != 1) {
      counter = 0;
      Serial.print("Reset Position: ");
      Serial.println(counter);
      sendData("rotator1", counter);
   }
   aState = digitalRead(rotary1A); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(rotary1B) != aState) { 
       counter ++;
     } else {
       counter --;
     }
     Serial.print("Position: ");
     Serial.println(counter);     
     sendData("rotator1" ,counter);
     aLastState = aState; // Updates the previous state of the outputA with the current state
   } 
}

void checkRotartor2() {
   if(digitalRead(button2) == 0 && counter2 != 1) {
      counter2 = 0;
      Serial.print("Reset Position2: ");
      Serial.println(counter2);
      sendData("rotator2", counter2);
   }
   aState2 = digitalRead(rotary2A); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState2 != aLastState2){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(rotary2B) != aState2) { 
       counter2 ++;
     } else {
       counter2 --;
     }
     Serial.print("Position2: ");
     Serial.println(counter2);     
     sendData("rotator2", counter2);
     aLastState2 = aState2; // Updates the previous state of the outputA with the current state
   } 
}

void loop() {
   loopWebSockServer();
   checkRotartor1();
   checkRotartor2();
}

// VW CAN BUS Steering wheel to HID
// Receive CAN BUS Messages under a certain ID and spit out Keyboard Media commands
// By Gavin Tranter 2020


// Using Consumer HID API from NicoHood
// https://github.com/NicoHood/HID/wiki/Consumer-API
#include "CANBUS_Steering_Controls_to_HID.h"
#include "Arduino.h"
#include <HID-Project.h>

// CANBUS Library from Coryjflowler
// https://github.com/coryjfowler/MCP_CAN_lib
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string
long unsigned int T=0;

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10


//********************************Setup Loop*********************************//
void setup()
{
  Serial.begin(115200);
 
  // Initialize MCP2515 running at 16MHz with a baudrate of 100kb/s and the masks and filters disabled.
  // VAG Infotainment Baudrate is 100KBPS
  if(CAN0.begin(MCP_ANY, CAN_100KBPS, MCP_16MHZ) == CAN_OK)
 
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input


  // Use the library to communicate with Windows
  Consumer.begin();
  delay(1000);

}

//********************************Main Loop*********************************//
void loop()
{
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

    // CANBUS ID 0x5C1 is the ID for VW Steering Wheel
    if(rxId  == 0x5C1){   
       for(byte i = 0; i<len; i++){
      //  sprintf(msgString, " %.2X", rxBuf[i]);
        T=(rxBuf[i]);
         //Serial.println(T);
              //delay(500);


              //CANBUS Data 6 for Volume Up
              if (T == 6) {
                Serial.println("UP");
                Consumer.write(MEDIA_VOL_UP);
                delay(250);
              }

              //CANBUS Data 7 for Volume Down
              if (T == 7) {
                Serial.println("Down");
                Consumer.write(MEDIA_VOL_DOWN);
                delay(250);
              }

              //CANBUS Data 26 for Mute
              if (T == 26) {
                Serial.println("Mute");
                Consumer.write(MEDIA_VOLUME_MUTE);
                delay(250);
              }

              //CANBUS Data 43 for Phone
              if (T == 43) {
                Serial.println("Play/Pause");
                Consumer.write(MEDIA_PLAY_PAUSE);
                delay(250);
              }

              //CANBUS Data 34 for Media Next
              if (T == 34) {
                Serial.println("Media Next");
                Consumer.write(MEDIA_NEXT);
                delay(250);
              }

              //CANBUS Data 35 for Media Previous
              if (T == 35) {
                Serial.println("Media Previous");
                Consumer.write(MEDIA_PREVIOUS);
                delay(250);
              }


              
      }
    }
  }
}

//********************************End File*********************************//

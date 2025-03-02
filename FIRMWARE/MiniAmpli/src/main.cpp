#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
#define REMOTEXY_MODE__ESP32CORE_BLUETOOTH
#include <BluetoothSerial.h>
// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "Mini Karibous Ampli"
#include <RemoteXY.h>

SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
DFRobotDFPlayerMini myDFPlayer;

int lastVolume = -1; // Stocke la dernière valeur du volume pour éviter des mises à jour inutiles
bool lastLoopMusic = false;
bool lastLoopAll = false;

void updateVolume();
void updateLoopSettings();

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = // 755 bytes
{ 255,8,0,0,0,49,3,19,0,0,0,77,105,110,105,32,75,97,114,105,
  98,111,117,115,32,65,109,112,108,105,0,24,1,106,200,1,1,10,0,12,
  9,24,84,12,255,30,26,111,112,116,105,111,110,32,48,0,111,112,116,105,
  111,110,32,49,0,111,112,116,105,111,110,32,50,0,111,112,116,105,111,110,
  32,51,0,111,112,116,105,111,110,32,52,0,111,112,116,105,111,110,32,53,
  0,111,112,116,105,111,110,32,54,0,111,112,116,105,111,110,32,55,0,111,
  112,116,105,111,110,32,56,0,111,112,116,105,111,110,32,57,0,111,112,116,
  105,111,110,32,49,48,0,111,112,116,105,111,110,32,49,49,0,111,112,116,
  105,111,110,32,49,50,0,111,112,116,105,111,110,32,49,51,0,111,112,116,
  105,111,110,32,49,52,0,111,112,116,105,111,110,32,49,53,0,111,112,116,
  105,111,110,32,49,54,0,111,112,116,105,111,110,32,49,55,0,111,112,116,
  105,111,110,32,49,56,0,111,112,116,105,111,110,32,49,57,0,111,112,116,
  105,111,110,32,50,48,0,111,112,116,105,111,110,32,50,49,0,111,112,116,
  105,111,110,32,50,50,0,111,112,116,105,111,110,32,50,51,0,111,112,116,
  105,111,110,32,50,52,0,111,112,116,105,111,110,32,50,53,0,111,112,116,
  105,111,110,32,50,54,0,111,112,116,105,111,110,32,50,55,0,111,112,116,
  105,111,110,32,50,56,0,111,112,116,105,111,110,32,50,57,0,111,112,116,
  105,111,110,32,51,48,0,111,112,116,105,111,110,32,51,49,0,111,112,116,
  105,111,110,32,51,50,0,111,112,116,105,111,110,32,51,51,0,111,112,116,
  105,111,110,32,51,52,0,111,112,116,105,111,110,32,51,53,0,111,112,116,
  105,111,110,32,51,54,0,111,112,116,105,111,110,32,51,55,0,111,112,116,
  105,111,110,32,51,56,0,111,112,116,105,111,110,32,51,57,0,111,112,116,
  105,111,110,32,52,48,0,111,112,116,105,111,110,32,52,49,0,111,112,116,
  105,111,110,32,52,50,0,111,112,116,105,111,110,32,52,51,0,111,112,116,
  105,111,110,32,52,52,0,111,112,116,105,111,110,32,52,53,0,111,112,116,
  105,111,110,32,52,54,0,111,112,116,105,111,110,32,52,55,0,111,112,116,
  105,111,110,32,52,56,0,111,112,116,105,111,110,32,52,57,0,111,112,116,
  105,111,110,32,53,48,0,111,112,116,105,111,110,32,53,49,0,111,112,116,
  105,111,110,32,53,50,0,111,112,116,105,111,110,32,53,51,0,111,112,116,
  105,111,110,32,53,52,0,111,112,116,105,111,110,32,53,53,0,111,112,116,
  105,111,110,32,53,54,0,111,112,116,105,111,110,32,53,55,0,111,112,116,
  105,111,110,32,53,56,0,111,112,116,105,111,110,32,53,57,0,111,112,116,
  105,111,110,32,54,48,0,111,112,116,105,111,110,32,54,49,0,111,112,116,
  105,111,110,32,54,50,0,111,112,116,105,111,110,32,54,51,0,111,112,116,
  105,111,110,32,54,52,0,4,23,73,71,11,128,178,26,1,12,42,24,24,
  7,24,31,226,150,182,239,184,143,0,1,66,42,24,24,7,24,31,226,143,
  185,239,184,143,0,1,39,42,24,24,7,24,31,226,143,184,239,184,143,0,
  129,7,73,18,13,64,16,240,159,148,136,0,129,9,15,59,7,64,16,83,
  101,108,101,99,116,32,121,111,117,114,32,109,117,115,105,99,32,58,0,10,
  13,91,24,24,117,25,24,31,240,159,148,128,0,31,10,39,91,24,24,117,
  25,24,31,240,159,148,129,0,31,10,65,91,24,24,117,25,24,31,240,159,
  148,130,0,31 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t MusicSelector; // from 0 to 65
  int8_t volume; // from 0 to 100
  uint8_t Play; // =1 if button pressed, else =0
  uint8_t Stop; // =1 if button pressed, else =0
  uint8_t Pause; // =1 if button pressed, else =0
  uint8_t Random; // =1 if state is ON, else =0
  uint8_t LoopAll; // =1 if state is ON, else =0
  uint8_t LoopMusic; // =1 if state is ON, else =0

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)

void setup()
{
  RemoteXY_Init();
  softSerial.begin(9600, SWSERIAL_8N1, /*rx =*/22, /*tx =*/23, false);

  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(softSerial, /*isACK = */ true, /*doReset = */ true))
  { // Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
    {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(30); // Set volume value. From 0 to 30
  myDFPlayer.play(1);    // Play the first mp3
}

void loop()
{
  RemoteXY_Handler();

  if (RemoteXY.Play)
    myDFPlayer.play(RemoteXY.MusicSelector);
  if (RemoteXY.Pause)
    myDFPlayer.pause();
  if (RemoteXY.Stop)
    myDFPlayer.stop();

  updateVolume();
  updateLoopSettings();
}

void updateVolume()
{
  int newVolume = map(RemoteXY.volume, 0, 100, 0, 30);
  if (newVolume != lastVolume)
  { // Vérifie si la valeur a changé
    myDFPlayer.volume(newVolume);
    lastVolume = newVolume; // Met à jour la dernière valeur connue
  }
}

void updateLoopSettings() {
  if (RemoteXY.LoopMusic != lastLoopMusic) {
      if (RemoteXY.LoopMusic) myDFPlayer.enableLoop();
      else myDFPlayer.disableLoop();
      lastLoopMusic = RemoteXY.LoopMusic;
  }
  
  if (RemoteXY.LoopAll != lastLoopAll) {
      if (RemoteXY.LoopAll) myDFPlayer.enableLoopAll();
      else myDFPlayer.disableLoopAll();
      lastLoopAll = RemoteXY.LoopAll;
  }
}
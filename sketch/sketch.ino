#include <SD.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>   // Available at https://github.com/adafruit/Adafruit_VS1053_Library
//#include <Adafruit_BLE_UART.h> // Available at https://github.com/adafruit/Adafruit_nRF8001

/* --- MP3 Shield Settings --- */

#define SHIELD_RESET -1
#define SHIELD_CS 7
#define SHIELD_DCS 6
#define CARDCS 4
#define DREQ 3

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

/* --- Bluetooth Settings --- */

//#define ADAFRUITBLE_REQ 10
//#define ADAFRUITBLE_RDY 2
//#define ADAFRUITBLE_RST 9
//
//Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

/* --- General Settings --- */

int volume = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Nobody tells Boxy Brown."));
  randomSeed(analogRead(0));
  setupPlayer();
  //setupBluetooth();
}

void loop() {
  checkForSerial();
  //uart.pollACI();
}

// Custom function to read serial

/* --- MP3 Shield Functions --- */

void setupPlayer(){
  if (! musicPlayer.begin()) {
    Serial.println(F("Unable to detect MP3 Player.  Check wiring."));
    while (1);
  }

  SD.begin(CARDCS);

  musicPlayer.setVolume(volume, volume);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  
  char fileName[12];    //an array to hold the filename
  int fileToPlay;

  fileToPlay = random(1,6); //a random number
  sprintf(fileName, "boxy_00%d.mp3", fileToPlay);

  playMP3(fileName);
}

// Change volume and play test blip. Volumes are a little weird... 0 is loudest, 100 is quietest
void changeVolume(int change){
  musicPlayer.stopPlaying();
  int newVolume = volume + change;
  volume = constrain(newVolume, 0, 100);
  musicPlayer.setVolume(volume, volume);
  playMP3("testblip.mp3");
}

// Play MP3 via passed File Name from SD Card
void playMP3(char* fileName){
  musicPlayer.stopPlaying();
  musicPlayer.startPlayingFile(fileName);
  Serial.println(fileName);
}

/* --- Bluetooth Functions --- */

//void aciCallback(aci_evt_opcode_t event)
//{
//  switch(event)
//  {
//    case ACI_EVT_DEVICE_STARTED:
//      Serial.println(F("Advertising started"));
//      break;
//    case ACI_EVT_CONNECTED:
//      Serial.println(F("Connected!"));
//      break;
//    case ACI_EVT_DISCONNECTED:
//      Serial.println(F("Disconnected or advertising timed out"));
//      break;
//    default:
//      break;
//  }
//}
//
//void rxCallback(uint8_t *buffer, uint8_t len)
//{
//  Serial.print(F("Received "));
//  Serial.print(len);
//  Serial.print(F(" bytes: "));
//  for(int i=0; i<len; i++)
//   Serial.print((char)buffer[i]); 
//
//  Serial.print(F(" ["));
//
//  for(int i=0; i<len; i++)
//  {
//    Serial.print(" 0x"); Serial.print((char)buffer[i], HEX); 
//  }
//  Serial.println(F(" ]"));
//
//  /* Echo the same data back! */
//  uart.write(buffer, len);
//}

//void setupBluetooth(){
//  uart.setDeviceName("BoxyBrown");
//  uart.setRXcallback(rxCallback);
//  uart.setACIcallback(aciCallback);
//  uart.begin();
//}

/* --- Custom Functions --- */

String readSerial() {
  while ( !Serial.available());

  String str = "";
  while (Serial.available()) {
      str += (char) Serial.read();
      delay(1);
  }
  return str;
}

void checkForSerial(){
  String action = readSerial();
  receivedMessage(action);
}

void receivedMessage(String action)
{
  if(action){
    Serial.print("action: ");
    Serial.println(action);
  }
  
  // Increase Volume
  if (action == "+") {
    changeVolume(-5);
  }

  // Decrease Volumne
  if (action == "-") {
    changeVolume(5);
  }

  // Nobody Tells Boxy Brown
  if (action == "1") {
    playMP3("boxy_001.mp3");
  }

  // That was a Grocery Bag
  if (action == "2") {
    playMP3("boxy_002.mp3");
  }

  // I just a What B&@#$ ?!?!
  if (action == "3") {
    playMP3("boxy_003.mp3");
  }

  // Diss me to my face
  if (action == "4") {
    playMP3("boxy_004.mp3");
  }

  // I want me some pie
  if (action == "5") {
    playMP3("boxy_005.mp3");
  }
}


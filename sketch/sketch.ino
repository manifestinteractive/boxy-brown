// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1 // VS1053 reset pin (unused!)
#define SHIELD_CS     7  // VS1053 chip select pin (output)
#define SHIELD_DCS    6  // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4 // Card chip select pin
#define DREQ 3   // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// Default Settings
int volume = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Nobody tells Boxy Brown");

  if (! musicPlayer.begin()) {
    Serial.println(F("Unable to detect MP3 Player.  Check wiring."));
    while (1);
  }

  SD.begin(CARDCS);

  musicPlayer.setVolume(volume, volume);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
}

void loop() {

  String action = read();
  Serial.println(action);

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

  delay(100);
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
}

// Custom function to read serial
String read() {
  while ( !Serial.available());

  String str = "";
  while (Serial.available()) {
      str += (char) Serial.read();
      delay(1);
  }
  return str;
}

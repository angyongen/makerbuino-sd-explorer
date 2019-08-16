#define screen gb.display
#define debug Serial

#include <Gamebuino.h>
#include "gbTools.h"
#include <SdFat.h>///#include "Gamebuino-SD-Explorer.h"
SdFat sd;
Gamebuino gb;


  FatFile dir;
  FatFile file;


void setup() {
  Serial.begin(115200);
  gb.begin();
  
  gb.battery.thresholds[0] = 0; //disable battery monitoring
  gb.setFrameRate(25);
  
  screen.println(F("reading SD card..."));
  if (!sd.begin(SD_CS, SPI_HALF_SPEED)) {
    screen.println(F("SD card not found"));
    debug.println(F("SD failed"));
    pause(gb);
  } else {
    debug.println(F("SD ok"));
  }
  waitForUpdate(gb);
  state_explore();
}



void loop() {
  // put your main code here, to run repeatedly:

}

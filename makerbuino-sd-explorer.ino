//Gamebuino
#define DISPLAYDIRECT
#include <gamebuino_main_alt.h>
#include "gbTools.h" //pause and waitForUpdate
#include "FreeStack.h"
Gamebuino gb;
#define repeatTime 3

//SdFat
#include <SdFat.h>///#include "Gamebuino-SD-Explorer.h"
SdFat sd;
//SdFat sd2;

#define screen gb.display
#define debug Serial
void setup() {
  Serial.begin(115200);
  gb.begin();

  gb.display.persistence = true;
  //gb.battery.thresholds[0] = 0; //disable battery monitoring
  //      gb.display.fontSize = 1;
  //      gb.display.textWrap = false;
  gb.setFrameRate(25);

  screen.clear();
  screen.println(F("reading SD card..."));
  if (!sd.begin(SD_CS, SPI_HALF_SPEED)) {
    //screen.println(F("SD card not found"));
    debug.println(F("SD failed"));
    pause(gb);
  } else {
    debug.println(F("SD ok"));
  }
  waitForUpdate(gb);
  //Serial.println(FreeStack());
}


void loop() {
  explorer_loop(sd);
}

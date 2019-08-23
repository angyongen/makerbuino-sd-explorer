//Gamebuino
#include <Gamebuino.h>
#include "gbTools.h" //pause and waitForUpdate
Gamebuino gb;
#define repeatTime 3

//SdFat
#include <SdFat.h>///#include "Gamebuino-SD-Explorer.h"
SdFat sd;

#define screen gb.display
#define debug Serial
void setup() {
  Serial.begin(115200);
  gb.begin();

  gb.display.persistence = true;
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
  explorer_loop();
}


void loop() {
  if (gb.update())
  {
    gb.display.clear();
    gb.display.print(F("Please reset the device"));
  }
}

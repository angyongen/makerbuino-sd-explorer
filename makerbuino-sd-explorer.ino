//Gamebuino
#include <Gamebuino.h>
#include "gbTools.h" //pause and waitForUpdate
Gamebuino gb;
#define repeatTime 3

//SdFat
#include <SdFat.h>///#include "Gamebuino-SD-Explorer.h"
SdFat sd;

#define explorer_state 0
#define file_actions_state 1
#define LFN_viewer_state 2
uint8_t state;

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
  state = 0;
}


void loop() {
  switch (state)
  {
    case explorer_state:
      state = explorer_loop(state);
      break;
    case LFN_viewer_state:
      state = LFN_viewer_loop(state);
      break;
  }
}

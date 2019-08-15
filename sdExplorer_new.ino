#define screen gb.display
#define debug Serial

#include <Gamebuino.h>
//#include "gbTools.cpp"
//#include "explorer.cpp"
Gamebuino gb;
sdExplorer sdExplorer;



void setup() {
  Serial.begin(115200);
  gb.begin();
  
  gb.battery.thresholds[0] = 0; //disable battery monitoring
  gb.setFrameRate(25);
  
  screen.println(F("reading SD card..."));
  if (!sd.begin(SD_CS, SPI_HALF_SPEED)) {
    screen.println(F("SD card not found"));
    debug.println(F("SD failed"));
    pause();
  } else {
    debug.println(F("SD ok"));
  }
  gb_waitForUpdate();
  
  sdExplorer.showMenu();
}

void loop() {
  // put your main code here, to run repeatedly:

}

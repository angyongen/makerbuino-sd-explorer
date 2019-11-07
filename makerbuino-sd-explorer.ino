//Gamebuino
#define DISPLAYDIRECT
#include <gamebuino_main_alt.h>
#include "gbTools.h" //pause and waitForUpdate
#include "FreeStack.h"
Gamebuino gb;
#define repeatTime 5

//SdFat
#include <SdFat.h>///#include "Gamebuino-SD-Explorer.h"

enum  sd_manager_state : uint8_t { zero = 0, initialised, reading, failed};
struct sd_manager
{
  SdFat sdReader;
  int8_t csPin;
  sd_manager_state state;
  SPISettings spiSettings;
};

sd_manager sd0;
sd_manager sd1;

void print_sd_info(sd_manager & sd, bool selected) {
  gb.display.cursorX = 0;
  if (selected) gb.display.print(F("\20")); else gb.display.print(F(" "));
  switch (sd.state) {
    case sd_manager_state::zero:
      gb.display.print(F("InitSD"));
      break;
    case sd_manager_state::initialised:
      gb.display.print(F("OpenSD"));
      break;
    case sd_manager_state::reading:
      gb.display.print(F("READ.."));
      break;
    case sd_manager_state::failed:
      gb.display.print(F("FAILED"));
      break;
  }
  gb.display.print(F(":CS"));
  gb.display.print(sd.csPin);
  gb.display.print(F("="));
  print_pinInfo(sd.csPin);
  waitForUpdate(gb);
}

void printAll_sdInfo()
{
  gb.display.clear();
  print_sd_info(sd0, false);
  gb.display.println();
  print_sd_info(sd1, false);
  gb.display.println();
}

void setup() {
  Serial.begin(115200);
  gb.begin();

  gb.display.persistence = true;
  gb.setFrameRate(25);

  gb.display.textWrap = false;

  //gb.battery.thresholds[0] = 0; //disable battery monitoring
  //gb.display.fontSize = 1;
  //Serial.println(FreeStack());
  
  sd0.csPin = SD_CS;
  printAll_sdInfo();
}

void initialiseSD(sd_manager & sd)
{
  Serial.println(F("Reading SD card"));
  sd.state = sd_manager_state::reading;
  print_sd_info(sd, true);
  if (sd.sdReader.begin(sd.csPin, sd.spiSettings)) {
    Serial.println(F("SD ok"));
    waitForUpdate(gb);
    sd.state = sd_manager_state::initialised;
  } else {
    Serial.println(F("SD failed"));
    sd.state = sd_manager_state::failed;
    print_sd_info(sd, true);
    pause_ABC_UDLR(gb);
    sd0.state = sd_manager_state::zero;
    sd1.state = sd_manager_state::zero;
  }
}

int8_t sd_loop(sd_manager & sd, uint8_t cur) {
  gb.display.cursorY = cur;
  print_sd_info(sd, true);
  while (true)
  {
    if (gb.update())
    {
      if (gb.buttons.pressed(BTN_A)) {
        if (sd.csPin >= 0) {
          if (sd.state == sd_manager_state::initialised) explorer_loop(sd.sdReader); else initialiseSD(sd);
        }
        printAll_sdInfo();
        gb.display.cursorY = cur;
        print_sd_info(sd, true);
      }
      if (gb.buttons.repeat(BTN_LEFT, repeatTime)) {
        if (!sd.state == sd_manager_state::initialised) if (sd.csPin > -1) --sd.csPin;
        print_sd_info(sd, true);
      }
      if (gb.buttons.repeat(BTN_RIGHT, repeatTime)) {
        if (!sd.state == sd_manager_state::initialised) if (sd.csPin < 19) ++sd.csPin;
        print_sd_info(sd, true);
      }
      if (gb.buttons.repeat(BTN_UP, repeatTime)) {
        print_sd_info(sd, false);
        return -1;
      }
      if (gb.buttons.repeat(BTN_DOWN, repeatTime)) {
        print_sd_info(sd, false);
        return 1;
      }
    }
  }
}

int8_t cur = 0;
void loop() {
  switch (cur)
  {
    case 0:
      cur += sd_loop(sd0, 0);
      break;
    case 1:
      cur += sd_loop(sd1, gb.display.fontHeight);
      break;
  }
  if (cur < 0) cur = 1;
  if (cur > 1) cur = 0;
}

//Gamebuino
#define DISPLAYDIRECT
#include <gamebuino_main_alt.h>
#include "gbTools.h" //pause and waitForUpdate
#include "FreeStack.h"
Gamebuino gb;
#define repeatTime 3

//SdFat
#include <SdFat.h>///#include "Gamebuino-SD-Explorer.h"

uint8_t sd_cur;

bool sd1_initialised;
int8_t SD1_CS = SD_CS; //10
SdFat sd1;

bool sd2_initialised;
int8_t SD2_CS = -1;
SdFat sd2;

SdFat & sd = sd1;

bool initialiseSD(uint8_t csPin, SPISettings spiSettings)
{
  gb.display.clear();
  gb.display.print(F("reading SD card @ "));
  gb.display.print(csPin);
  gb.display.println(F("..."));
  waitForUpdate(gb);
  if (sd.begin(csPin, spiSettings)) {
    Serial.println(F("SD ok"));
    waitForUpdate(gb);
    return true;
  } else {
    Serial.println(F("SD failed"));
    gb.display.println(F("SD card not found"));
    waitForUpdate(gb);
    pause(gb);
    return false;
  }
}

void redraw_mainmenu(uint8_t sd_cur)
{
  gb.display.clear();
  if (sd_cur == 0) gb.display.print(F("\20"));
  if (sd1_initialised) {
    gb.display.print(F("Open"));
  } else {
    gb.display.print(F("Initialise"));
  }
  gb.display.print(F(" SD 1 @ "));
  gb.display.println(SD1_CS);
  if (sd_cur == 1) gb.display.print(F("\20"));
  if (sd2_initialised) {
    gb.display.print(F("Open"));
  } else {
    gb.display.print(F("Initialise"));
  }
  gb.display.print(F(" SD 2 @ "));
  gb.display.println(SD2_CS);
}

void setup() {
  Serial.begin(115200);
  gb.begin();

  gb.display.persistence = true;
  gb.setFrameRate(25);

  //gb.battery.thresholds[0] = 0; //disable battery monitoring
  //gb.display.fontSize = 1;
  //gb.display.textWrap = false;

  //Serial.println(FreeStack());
  redraw_mainmenu(sd_cur);
}

void loop() {
  if (gb.update())
  {
    if (gb.buttons.pressed(BTN_A))
    {
      if (sd_cur == 0)
      {
        sd = sd1;
        if (sd1_initialised) {
          explorer_loop();
        } else {
          if (SD2_CS > 0) {
            pinMode(SD2_CS, OUTPUT);
            digitalWrite(SD2_CS, HIGH);
          }
          sd1_initialised = initialiseSD(SD1_CS, SPI_HALF_SPEED);
        }
      } else if (sd_cur == 1) {
        sd = sd2;
        if (sd2_initialised) {
          explorer_loop();
        } else {
          if (SD1_CS > 0) {
            pinMode(SD1_CS, OUTPUT);
            digitalWrite(SD1_CS, HIGH);
          }
          sd2_initialised = initialiseSD(SD2_CS, SPI_HALF_SPEED);
        }
      }
      redraw_mainmenu(sd_cur);
    }
  }
  if (gb.buttons.repeat(BTN_UP, repeatTime)) if (sd_cur != 0) redraw_mainmenu(--sd_cur);
  if (gb.buttons.repeat(BTN_DOWN, repeatTime)) if (sd_cur != 1) redraw_mainmenu(++sd_cur);
}

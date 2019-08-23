typedef int8_t displayedFiles_t; // file # on screen

const displayedFiles_t displayedFiles = 6;

//bool explorer_option_showBlankEntries = true;
const bool explorer_option_showBlankEntries = true;

inline void explorer_redraw(FatFile & dir, uint32_t & startPosition)
{
  gb.display.clear();
  dir_t entry;
  dir.seekSet(startPosition);
  gb.display.print(F("position: "));
  gb.display.println(startPosition);
  //Serial.println();
  displayedFiles_t f = 0;
  while (f < displayedFiles)
  {
    if (readNextDirRaw(dir, entry))
    {
      switch (checkDirEntry(entry)) {
        case 0: //last entry
          //Serial.print(0); Serial.println(entry.name[0], HEX);
          return;
        case 1: //invalid or deleted
          if (explorer_option_showBlankEntries) gb.display.println();
          //Serial.print(1); Serial.println(entry.name[0], HEX);
          break;
        case 2:
          //Serial.print(2); Serial.println(entry.name[0], HEX);
          ++f;
          for (uint8_t i = 0; i < 11; i++) gb.display.print((char)entry.name[i]);
          gb.display.println();
          break;
      }
    } else {
      return;
    }
  }
}

uint8_t explorer_loop()
{
  FatFile dir;
  uint32_t startPosition; // position in dir file of first displayed file
  uint16_t file_index;

  if (dir.open("/")) {
    displayedFiles_t fileNo = 0;
    dir.rewind();
    startPosition = dir.curPosition();
    explorer_redraw(dir, startPosition);
    while (true) {
      if (gb.update()) {
        if (gb.buttons.pressed(BTN_A))
        {
          dir_t entry;
          dir.seekSet(startPosition);
          uint16_t n = readNextDir(dir, entry);
          if (n > 0) {
            --n;
            startPosition += 32 * n;
            dir.seekSet(startPosition);
            file_actions_loop(dir, entry, startPosition);
          } else {
            dir.open("/");
            dir.rewind();
          }
          explorer_redraw(dir, startPosition);
        }
        if (gb.buttons.repeat(BTN_UP, repeatTime))
        {
          if (startPosition != 0)
          {
            startPosition -= 32;
            explorer_redraw(dir, startPosition);
          }
        }
        if (gb.buttons.repeat(BTN_DOWN, repeatTime))
        {
          startPosition += 32;
          explorer_redraw(dir, startPosition);
        }
        if (gb.buttons.pressed(BTN_B))
        {
          //enterDirectory();
        }
        if (gb.buttons.pressed(BTN_C))
        {
        }
      }
    }
  } else {

  }
}

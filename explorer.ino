typedef int8_t displayedFiles_t; // file # on screen


#ifdef DISPLAYDIRECT
const displayedFiles_t displayedFiles = 5;
#else
const displayedFiles_t displayedFiles = 7;
#endif


class explorer_options
{
  public:
  uint8_t data;
  
  bool showBlankEntries() {return (data & 1);};
  void showBlankEntries(bool val) {if (val) data = data | (1); else data = data & ~(1); };
  
  bool showLFNEntries() {return (data & 2);};
  void showLFNEntries(bool val) {if (val) data = data | (2); else data = data & ~(2); };
  
  bool showDeletedEntries() {return (data & 4);};
  void showDeletedEntries(bool val) {if (val) data = data | (4); else data = data & ~(4); };
  
  bool showVolumeNameEntry() {return (data & 8);};
  void showVolumeNameEntry(bool val) {if (val) data = data | (8); else data = data & ~(8); };
};

explorer_options explorer_option;
inline void explorer_printAttributes(dir_t & entry)
{
  uint8_t attr = entry.attributes;
  attr <<= 2;
  gb.display.print((attr & 0x80) ? F("A") : F("-")); attr <<= 1;
  attr <<= 1;
  gb.display.print((attr & 0x80) ? F("V") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("S") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("H") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("R") : F("-")); attr <<= 1;
}
inline bool explorer_redraw(FatFile & dir, uint32_t & startPosition)
{
  gb.display.clear();
  dir_t entry;
  dir.seekSet(startPosition);
  gb.display.print(F("position: "));
  gb.display.println(startPosition / 32);
  //Serial.println(FreeStack());
  displayedFiles_t f = 0;
  //Serial.println();
  while (f < displayedFiles)
  {
    if (readNextDirRaw(dir, entry))
    {
      switch (checkDirEntry(entry)) {
        case 0: //last entry
          //Serial.print(0); Serial.println(entry.name[0], HEX);
          return (f > 1); // stop when f == 1
        case 1: //invalid or deleted
          if (explorer_option.showBlankEntries()) {
            ++f;
            if ((entry.attributes == DIR_ATT_LONG_NAME) && explorer_option.showLFNEntries()) {
              printLFNEntry(reinterpret_cast<ldir_t &>(entry));
              gb.display.cursorX = gb.display.fontWidth * 14;
              gb.display.println(F(" [LFN]"));
            } else if ((entry.name[0] == DIR_NAME_DELETED) && explorer_option.showDeletedEntries()) {
              gb.display.print(F(" "));
              for (uint8_t i = 1; i < 11; i++) gb.display.print((char)entry.name[i]);
              gb.display.cursorX = gb.display.fontWidth * 12;
              gb.display.print(F("[X] "));
              explorer_printAttributes(entry);
              gb.display.println();
            } else if ((entry.attributes == DIR_ATT_VOLUME_ID) && explorer_option.showVolumeNameEntry()) {
              for (uint8_t i = 0; i < 11; i++) gb.display.print((char)entry.name[i]);
              gb.display.cursorX = gb.display.fontWidth * 12;
              gb.display.println(F(" [LABEL]"));
            } else {
              gb.display.println();
            }
          } else {
            if (f == 0)  startPosition += 32;
          }
          //Serial.print(1); Serial.println(entry.name[0], HEX);
          break;
        case 2:
          //Serial.print(2); Serial.println(entry.name[0], HEX);
          ++f;
          for (uint8_t i = 0; i < 11; i++) gb.display.print((char)entry.name[i]);
          gb.display.cursorX = gb.display.fontWidth * 11;
          if (DIR_IS_SUBDIR(&entry)) gb.display.print(F("<DIR>")); else gb.display.print(F("     "));
          explorer_printAttributes(entry);
          gb.display.println();
          break;
      }
    } else {
      return false;
    }
  }
  return true;
}

inline void explorer_gotoPreviousFile(FatFile & dir, uint32_t & startPosition)
{
  if (startPosition == 0) return;
  dir_t entry;
  uint32_t newStartPosition = startPosition - 32;
  dir.seekSet(newStartPosition);
  while (readNextDirRaw(dir, entry))
  {
    switch (checkDirEntry(entry)) {
      case 0:
        return;
      case 1:
        if (explorer_option.showBlankEntries()) {
          startPosition = newStartPosition;
          return;
        } else {
          if (newStartPosition == 0) return;
          newStartPosition -= 32;
          dir.seekSet(newStartPosition);
          break;
        }
      case 2:
        startPosition = newStartPosition;
        return;
    }
    dir.seekSet(newStartPosition);
  }
}

inline void explorer_gotoNextFile(FatFile & dir, uint32_t & startPosition)
{
  startPosition += 32;
  /*
    dir_t entry;
    dir.seekSet(startPosition);
    while (readNextDirRaw(dir, entry))
    {
    switch (checkDirEntry(entry)) {
      case 0:
        startPosition -= 32;
        return;
      case 1:
        if (!explorer_option_showBlankEntries) startPosition += 32;
        break;
      case 2:
        return;
    }
    }
  */
}
uint8_t explorer_loop()
{
  FatFile dir;
  uint32_t startPosition; // position in dir file of first displayed file
  uint16_t file_index;
  bool endNotReached = false;
  if (dir.openRoot(&sd)) {
    displayedFiles_t fileNo = 0;
    //dir.rewind();
    startPosition = 0;
    endNotReached = explorer_redraw(dir, startPosition);
    while (true) {
      if (gb.update()) {
        if (gb.buttons.pressed(BTN_A))
        {
          dir_t entry;
          dir.seekSet(startPosition);
          readNextDirRaw(dir, entry);
            file_actions_loop(dir, entry, startPosition);
          /*
          uint16_t n = readNextDir(dir, entry);
          if (n > 0) {
            --n;
            startPosition += 32 * n;
            dir.seekSet(startPosition);
            file_actions_loop(dir, entry, startPosition);
          } else {
            dir.close();
            dir.openRoot(&sd);
            //dir.rewind();
          }
          */
          endNotReached = explorer_redraw(dir, startPosition);
        }
        if (gb.buttons.repeat(BTN_UP, repeatTime))
        {
          explorer_gotoPreviousFile(dir, startPosition);
          endNotReached = explorer_redraw(dir, startPosition);
        }
        if (gb.buttons.repeat(BTN_DOWN, repeatTime))
        {
          if (endNotReached) {
            explorer_gotoNextFile(dir, startPosition);
          }
          endNotReached = explorer_redraw(dir, startPosition);
        }
        if (gb.buttons.pressed(BTN_B))
        {
          //enterDirectory();
          //return;
        }
        if (gb.buttons.pressed(BTN_C))
        {
          explorer_options_loop();
          endNotReached = explorer_redraw(dir, startPosition);
        }
      }
    }
  } else {

  }
}

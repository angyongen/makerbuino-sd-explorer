typedef int8_t displayedFiles_t; // file # on screen

const displayedFiles_t displayedFiles = 7;

bool explorer_option_showBlankEntries = false;

inline bool explorer_redraw(FatFile & dir, uint32_t & startPosition)
{
  gb.display.clear();
  dir_t entry;
  dir.seekSet(startPosition);
  gb.display.print(F("position: "));
  gb.display.println(startPosition / 32);
  //Serial.println();
  displayedFiles_t f = 0;
  while (f < displayedFiles)
  {
    if (readNextDirRaw(dir, entry))
    {
      switch (checkDirEntry(entry)) {
        case 0: //last entry
          //Serial.print(0); Serial.println(entry.name[0], HEX);
          return (f > 1); // stop when f == 1
        case 1: //invalid or deleted
          if (explorer_option_showBlankEntries) {
            ++f;
            if (entry.name[0] == DIR_NAME_DELETED) {
              gb.display.print(F(" "));
              for (uint8_t i = 1; i < 11; i++) gb.display.print((char)entry.name[i]);
              gb.display.println(F(" [X]"));
            } else {
              gb.display.println(F(" "));
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
          if (DIR_IS_SUBDIR(&entry) )gb.display.print(F(" <DIR>"));
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
        if (explorer_option_showBlankEntries) {
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
  if (dir.open("/")) {
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
        }
        if (gb.buttons.pressed(BTN_C))
        {
          explorer_option_showBlankEntries = !explorer_option_showBlankEntries;
          endNotReached = explorer_redraw(dir, startPosition);
        }
      }
    }
  } else {

  }
}

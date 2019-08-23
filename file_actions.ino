

#define file_actions_count 3

inline void file_action_0_printText(FatFile & dir, dir_t & entry)
{
  if (DIR_IS_SUBDIR(&entry)) {
    gb.display.print(F("Open "));
  } else {
    gb.display.print(F("Flash "));
  }
  gb.display.print((char*)entry.name);
}

inline void file_action_0(FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  if (DIR_IS_SUBDIR(&entry)) {
    if (!dir.open(&dir, fileposition / 32, O_READ)) {
      Serial.println(F("open fail"));
    }
    dir.seekSet(0);
    fileposition = dir.curPosition();
    return;
  } else {
    load_game_menu(dir, (char*)entry.name);
  }
}

inline void file_action_1_printText(FatFile & dir, dir_t & entry)
{
  gb.display.print(F("Properties"));
}
inline void file_action_1(FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  gb.display.clear();

  gb.display.print(F("attributes: "));
  //bitmask: 1=read only, 2=hidden, 4=system, 8=volumelabel, 16=dir, 32=archive
  uint8_t attr = entry.attributes;
  gb.display.print((attr & 0x80) ? F("x") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("x") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("A") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("D") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("V") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("S") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("H") : F("-")); attr <<= 1;
  gb.display.print((attr & 0x80) ? F("R") : F("-")); attr <<= 1;


  while (true) {
    if (gb.update()) {
      if (gb.buttons.pressed(BTN_B)) return;
    }
  }
}

inline void file_action_2_printText(FatFile & dir, dir_t & entry)
{
  gb.display.print(F("Get LFN"));
}

inline bool redrawLFNPage(FatFile & dir, uint32_t filePosition, uint8_t indexStart, uint8_t indexEnd)
{ //true for next page available, false for unavailable
  dir_t lfnEntry;
  for (uint8_t index = indexStart; index < indexEnd; ++index) // 7 lines x 13
  {
    filePosition -= 32;
    dir.seekSet(filePosition);
    if (readNextDirRaw(dir, lfnEntry))
    {
      ldir_t & ldir = reinterpret_cast<ldir_t&>(lfnEntry);
      //if (!ldir) {DBG_FAIL_MACRO;goto fail;}
      if (index != (ldir.ord & 0X1F)) {
        DBG_FAIL_MACRO;
        return false;
      }
      if (ldir.attr != DIR_ATT_LONG_NAME) return false;
      if (index < 10) gb.display.print(F(" "));
      gb.display.print(index); gb.display.print(F(" "));
      for (uint8_t i = 0; i < 5; ++i) printLFNChar(ldir.name1[i]);
      for (uint8_t i = 0; i < 6; ++i) printLFNChar(ldir.name2[i]);
      for (uint8_t i = 0; i < 2; ++i) printLFNChar(ldir.name3[i]);
      if (ldir.ord & LDIR_ORD_LAST_LONG_ENTRY) return false;
      gb.display.println();
    }
  }
  return true;
}
inline void LFNViewer_draw_pageNo(uint8_t & page, uint8_t & pages)
{
  gb.display.cursorX = 0;
  gb.display.cursorY = 0;
  if (page == 1) {
    gb.display.print(F(" "));
  } else {
    gb.display.print(F("\21"));
  }
  gb.display.print(F(" Page "));
  gb.display.print(page);
  gb.display.print(F(" of "));
  if (pages == 0) {
    gb.display.print(F("? \20"));
  } else {
    gb.display.print(pages);
  }
  if (page < pages) gb.display.print(F(" \20"));
}
inline void file_action_2(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  uint8_t indexStart = 1;
  uint8_t page = 1;
  uint8_t pages = 0;

  gb.display.clear();
  gb.display.println();
  if (!redrawLFNPage(dir, filePosition, indexStart, indexStart + 7)) pages = page;
  LFNViewer_draw_pageNo(page, pages);
  while (true)
  {
    if (gb.update())
    {
      if (gb.buttons.pressed(BTN_B)) return;
      if (gb.buttons.pressed(BTN_LEFT)) {
        if (page != 1) --page;
        gb.display.clear();
        gb.display.println();
        if (!redrawLFNPage(dir, filePosition, indexStart, indexStart + 7)) pages = page;
        LFNViewer_draw_pageNo(page, pages);
      }
      if (gb.buttons.pressed(BTN_RIGHT)) {
        if (page != pages) ++page;
        gb.display.clear();
        gb.display.println();
        if (!redrawLFNPage(dir, filePosition, indexStart, indexStart + 7)) pages = page;
        LFNViewer_draw_pageNo(page, pages);
      }
    }
  }
}

void file_actions_loop(FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  int8_t currentY = LCDHEIGHT / 2;
  int8_t targetY = 0;
  int8_t option = 0;
  entry.name[8] = 0; //end string before extension; only 9 bytes copied during load_game subroutine.
  while (true) {
    if (gb.update()) {
      if (gb.buttons.pressed(BTN_B)) return;
      if (gb.buttons.pressed(BTN_UP)) --option;
      if (gb.buttons.pressed(BTN_DOWN)) ++option;

      if (option < 0) option = 0;
      if (option >= file_actions_count) option = file_actions_count - 1;

      targetY = (gb.display.fontHeight + 4) - gb.display.fontHeight * option; //center the menu on the active item
      currentY = (currentY + targetY) / 2; // slowly approach targetY by taking average
      //explorer_menu_redraw(currentY, option);

      gb.display.clear();
      gb.display.cursorX = 0;
      gb.display.cursorY = currentY;
      gb.display.fontSize = 1;
      gb.display.textWrap = false;
      for (byte i = 0; i < file_actions_count; i++) {
        if (gb.display.cursorY < -gb.display.fontHeight) {
          gb.display.cursorY += gb.display.fontHeight;
          continue;
        }
        if (i == option) {
          gb.display.cursorX = 3;
          gb.display.cursorY = currentY + option * gb.display.fontHeight;
        }
        switch (i)
        {
          case 0:
            file_action_0_printText(dir, entry);
            gb.display.println();
            break;
          case 1:
            file_action_1_printText(dir, entry);
            gb.display.println();
            break;
          case 2:
            file_action_2_printText(dir, entry);
            gb.display.println();
            break;
          case 3:
            gb.display.println(F("Edit"));
            break;
          case 4:
            gb.display.println(F("Open with..."));
            break;
        }
      }
      gb.display.setColor(WHITE);
      gb.display.drawFastHLine(0, currentY + gb.display.fontHeight * option - 1, LCDWIDTH);
      gb.display.setColor(BLACK);
      gb.display.drawRoundRect(0, currentY + gb.display.fontHeight * option - 2, LCDWIDTH, (gb.display.fontHeight + 3), 3);

      if (gb.buttons.pressed(BTN_A))
      {
        switch (option)
        {
          case 0:
            file_action_0(dir, entry, fileposition);
            return;
          case 1:
            file_action_1(dir, entry, fileposition);
            break;
          case 2:
            file_action_2(dir, entry, fileposition);
            break;
        }
      }
    }
  }
}

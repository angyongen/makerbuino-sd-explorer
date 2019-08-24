
#ifdef DISPLAYDIRECT
const uint8_t displayedEntries = 5;
#else
const uint8_t displayedEntries = 7;
#endif

inline bool file_action_3_available(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
    // Check for LFN.
    if (!dir.seekSet(filePosition - 32)) return false;
    ldir_t ldir;
    readNextDirRaw(dir, reinterpret_cast<dir_t&>(ldir));
    return (ldir.attr == DIR_ATT_LONG_NAME) && (1 == (ldir.ord & 0X1F));
}
inline void file_action_3_printText(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  gb.display.print(F("Get LFN"));
}
inline bool redrawLFNPage(FatFile & dir, uint32_t filePosition, uint8_t indexStart, uint8_t indexEnd)
{ //true for next page available, false for unavailable
  dir_t lfnEntry;
  filePosition -= (indexStart - 1) * 32;
  for (uint8_t index = indexStart; index < indexEnd; ++index)
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
inline void file_action_3(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  uint8_t page = 1;
  uint8_t pages = 0;

  gb.display.clear();
  gb.display.println();
  if (!redrawLFNPage(dir, filePosition, page * displayedEntries - displayedEntries + 1, page * displayedEntries + 1)) pages = page;
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
        if (!redrawLFNPage(dir, filePosition, page * displayedEntries - displayedEntries + 1, page * displayedEntries + 1)) pages = page;
        LFNViewer_draw_pageNo(page, pages);
      }
      if (gb.buttons.pressed(BTN_RIGHT)) {
        if (page != pages) ++page;
        gb.display.clear();
        gb.display.println();
        if (!redrawLFNPage(dir, filePosition, page * displayedEntries - displayedEntries + 1, page * displayedEntries + 1)) pages = page;
        LFNViewer_draw_pageNo(page, pages);
      }
    }
  }
}

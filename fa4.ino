inline bool file_action_4_available(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  return true;
}
inline void file_action_4_printText(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  gb.display.print(F("Properties"));
}
inline void printAttributes(dir_t & entry)
{
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
}

inline void file_properties_print_page_1(FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  gb.display.clear();
  gb.display.println(F("  Properties 1 of 2 \20"));

  gb.display.print((char*)entry.name);
  //gb.display.print(F("Attributes: "));
  gb.display.cursorX = LCDWIDTH - 8 * gb.display.fontWidth;
  printAttributes(entry);

  gb.display.println();
  gb.display.print(F("Size: "));
  float tmp_size = entry.fileSize;
  tmp_size /= 1024;
  if (tmp_size < 1024) {
    gb.display.print(tmp_size);
    gb.display.println(F("kiB"));
  } else {
    tmp_size /= 1024;
    if (tmp_size < 1024) {
      gb.display.print(tmp_size);
      gb.display.println(F("MiB"));
    } else {
      tmp_size /= 1024;
      if (tmp_size < 1024) {
        gb.display.print(tmp_size);
        gb.display.println(F("GiB"));
      }
    }
  }
  // todo: possibly add kB, MB, GB?
  gb.display.print(F("C:"));
  dir.printFatDate(&gb.display, entry.creationDate);
  gb.display.print(F(" "));
  dir.printFatTime(&gb.display, entry.creationTime);
  gb.display.println();
  gb.display.print(F("W:"));
  dir.printFatDate(&gb.display, entry.lastWriteDate);
  gb.display.print(F(" "));
  dir.printFatTime(&gb.display, entry.lastWriteTime);
  gb.display.println();
  gb.display.print(F("A:"));
  dir.printFatDate(&gb.display, entry.lastAccessDate);
  //gb.display.println();
}

inline void file_properties_print_page_2(FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  gb.display.clear();
  gb.display.println(F("\21 Properties 1 of 2  "));

  uint32_t firstCluster = entry.firstClusterLow | ((uint32_t)entry.firstClusterHigh << 16);
  gb.display.print(F("firstCluster: "));
  gb.display.println(firstCluster);
  
  gb.display.print(F("Size: "));
  gb.display.print(entry.fileSize); // uint32_t 10 digits max
  gb.display.println(F(" B"));
}
inline void file_properties_print_page(uint8_t page, FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  switch (page)
  {
    case 1:
    file_properties_print_page_1(dir, entry, fileposition);
    break;
    case 2:
    file_properties_print_page_2(dir, entry, fileposition);
  }
}
inline void file_action_4(SdFat & sd, FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  uint8_t page = 1;
  file_properties_print_page_1(dir, entry, fileposition);
  while (true) {
    if (gb.update()) {
      if (gb.buttons.pressed(BTN_B)) return;
      if (gb.buttons.pressed(BTN_LEFT)) if (page != 1) file_properties_print_page(--page, dir, entry, fileposition);
      if (gb.buttons.pressed(BTN_RIGHT)) if (page != 2) file_properties_print_page(++page, dir, entry, fileposition);
      ;
    }
  }
}

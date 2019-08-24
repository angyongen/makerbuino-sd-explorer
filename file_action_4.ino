inline bool file_action_4_available(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  return true;
}
inline void file_action_4_printText(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  gb.display.print(F("Properties"));
}
inline void file_action_4(FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  gb.display.clear();

  gb.display.print(F("Attributes: "));
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

  gb.display.println();
  gb.display.print(F("Size: "));
  float tmp_size = entry.fileSize;
  tmp_size /= 1000;
  if (tmp_size < 1024) {
    gb.display.print(tmp_size);
    gb.display.println(F("kB"));
  } else {
    tmp_size /= 1000;
    if (tmp_size < 1024) {
      gb.display.print(tmp_size);
      gb.display.println(F("MB"));
    } else {
      tmp_size /= 1000;
      if (tmp_size < 1024) {
        gb.display.print(tmp_size);
        gb.display.println(F("GB"));
      }
    }
  }
  // todo: possibly add kiB, MiB, GiB?

  gb.display.print(F("("));
  gb.display.print(entry.fileSize); // uint32_t 10 digits max
  gb.display.println(F(" bytes)"));

  //add date created, date modified(last write), last accessed date
  while (true) {
    if (gb.update()) {
      if (gb.buttons.pressed(BTN_B)) return;
    }
  }
}

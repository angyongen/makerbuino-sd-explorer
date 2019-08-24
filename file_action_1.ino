inline bool file_action_1_available(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  return DIR_IS_SUBDIR(&entry);
}
inline void file_action_1_printText(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  gb.display.print(F("Open "));
  gb.display.print((char*)entry.name);
}
inline void file_action_1(FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  if (!dir.open(&dir, fileposition / 32, O_READ)) {
    Serial.println(F("open fail"));
  }
  dir.seekSet(0);
  fileposition = dir.curPosition();
}

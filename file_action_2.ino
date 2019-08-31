inline bool file_action_2_available(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  return (DIR_IS_FILE(&entry) && dir.isRoot() && (entry.name[0] != DIR_NAME_DELETED));
}
inline void file_action_2_printText(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  gb.display.print(F("Flash "));
  gb.display.print((char*)entry.name);
}
inline void file_action_2(FatFile & dir, dir_t & entry, uint32_t & fileposition)
{
  load_game_menu(dir, (char*)entry.name);
}

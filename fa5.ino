inline bool file_action_5_available(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  return DIR_IS_FILE(&entry);
}
inline void file_action_5_printText(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  gb.display.print(F("View"));
}

            //gb.display.println(F("Edit"));
            //gb.display.println(F("Open with..."));

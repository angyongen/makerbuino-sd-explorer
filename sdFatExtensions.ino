inline bool readNextDirRaw(FatFile &d, dir_t &dir) {
  //reads next directory entry
  FatPos_t pos; d.getpos(&pos);
  if (!d.isDir() || (0X1F & pos.position)) return false; // not a directory file or mis-positioned
  if (d.read(&dir, sizeof(dir_t)) != sizeof(dir_t)) return false; //could not read a complete entry
  return true;
}

inline uint8_t checkDirEntry(dir_t & dir) {
  if (dir.name[0] == DIR_NAME_FREE) return 0;   // last entry
  if (dir.name[0] == DIR_NAME_DELETED) return 1; // deleted entry
  if (DIR_IS_FILE_OR_SUBDIR(&dir)) return 2;     // normal file or subdirectory
  return 1; // invalid entry
}
/*
inline int16_t readNextDir(FatFile &d, dir_t &dir) {
  //returns number of entries read to reach a valid entry, 0 if failed and negative number if end of file reached
  int16_t n = 0;
  FatPos_t pos; d.getpos(&pos);
  if (!d.isDir() || (0X1F & pos.position)) return 0; // not a directory file or mis-positioned
  while (1) {
    ++n;
    if (d.read(&dir, sizeof(dir_t)) != sizeof(dir_t)) return -n; //could not read a complete entry
    if (dir.name[0] == DIR_NAME_FREE) return -n;   // last entry if DIR_NAME_FREE
    if (dir.name[0] == DIR_NAME_DELETED) continue; // skip empty entries
    if (DIR_IS_FILE_OR_SUBDIR(&dir)) return n;     // normal file or subdirectory
  }
}
*/


int16_t readNextDir(FatFile &d, dir_t &dir) {
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

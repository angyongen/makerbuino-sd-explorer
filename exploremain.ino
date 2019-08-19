
typedef int8_t fileBufferSize_t; // file # in buffer
//typedef int8_t fileNameSize_t;
typedef int16_t fileNum_t; // real file # on sd card
//sdExplorer sdExplorer(dir, file);


//const fileBufferSize_t fileBufferSize = 5;

// fileBufferSize_t firstFileIndex; // the location where the first element is located in the circular buffer
// fileNum_t filesPosition; //the file # that readNext will return
// fileNum_t filesBegin; // the first file # stored in fileBuffer
// fileNum_t filesSize; // the number of files stored in fileBuffer
//char 8_3_filenames[13][fileBufferSize];

FatPos_t startPosition; // position on disk of first displayed file
#define displayedFiles 5
#define repeatTime 3

bool rewind()
{
  debug.println(F("rewind"));

  dir.rewind();
  //filesPosition = 0;
  //filesBegin = 0;
  //filesSize = 0;
  //firstFileIndex = 0;
  //circularBuffer.clear();

  return true;
}

int16_t readNextDir(FatFile &f, dir_t &dir) { //returns number of entries read to reach a valid entry or 0 if failed
  int16_t n = 0;
  FatPos_t pos;
  f.getpos(&pos);
  // if not a directory file or mis-positioned return an error
  if (!f.isDir() || (0X1F & pos.position)) return 0;

  while (1) {
    ++n;
    if (f.read(&dir, sizeof(dir_t)) != sizeof(dir_t)) return 0;
    if (dir.name[0] == DIR_NAME_FREE) return 0; // last entry if DIR_NAME_FREE
    if (dir.name[0] == DIR_NAME_DELETED) continue; // skip empty entries
    if (DIR_IS_FILE_OR_SUBDIR(&dir)) return n; // return if normal file or subdirectory
  }
}

int16_t readPrevDir(FatFile &f) {}
void shiftPosition(int16_t change)
{
  startPosition.position += 32 * change;
}
void redraw()
{
  gb.display.clear();
  dir_t entry;
  dir.setpos(&startPosition);
  gb.display.print(F("cluster "));
  gb.display.print(startPosition.cluster);
  gb.display.print(F(":"));
  gb.display.println(startPosition.position);
  for (fileNum_t f = 0; f < displayedFiles; f++)
  {
    if (!readNextDir(dir, entry)) return;
    for (uint8_t i = 0; i < 11; i++) gb.display.print((char)entry.name[i]);
    gb.display.println();
  }
}

void state_explore()
{
  if (dir.open("/")) {
    //sdExplorer.showMenu(gb);

    fileNum_t fileNo = 0;
    boolean exit = false;
    rewind();
    gb.display.persistence = true;
    redraw();
    while (!exit) {
      if (gb.update()) {
        //for (fileNum_t f = 0; f < circularBuffer.capacity; f++)
        if (gb.buttons.pressed(BTN_A))
        {
          rewind();
        }
        if (gb.buttons.repeat(BTN_UP, repeatTime))
        {
          shiftPosition(-1);//if (getFileName(fileNo)) ++fileNo;
          redraw();
        }
        if (gb.buttons.repeat(BTN_DOWN, repeatTime))
        {
          shiftPosition(1);//if (getFileName(fileNo)) ++fileNo;
          redraw();
        }
        if (gb.buttons.pressed(BTN_B))
        {
          //enterDirectory();
        }
        /*
          if (gb.buttons.pressed(BTN_C))
          {
          fileNum_t i = circularBuffer.head;
          for (fileNum_t f = 0; f < circularBuffer.capacity; f++)
          {
            debug.print(i);
            debug.print(F("-"));
            debug.println(fileBuffer[i]);
            screen.print(i);
            screen.print(F("-"));
            screen.println(fileBuffer[i]);
            i = circularBuffer.next(i);
          }
          }
        */
      }
    }
    /*
      uint16_t x = 0;
            for (uint16_t f = x; f < x+255; f++)
            {
              if (file.open(&dir, f, O_READ))
              {
                file.printName(&Serial);
                Serial.println();
                file.close();
              } else {
                Serial.println(F("open fail"));
              }
            }
    */
  }
}





inline void debugp()
{

  //Serial.print(F("filesPos"));
  //Serial.println(filesPosition);
  //Serial.print(F("filesSz"));
  //Serial.println(filesSize);
  /*
  for (fileNum_t f = 0; f < filesSize; f++)
  {
    debug.print(f);
    debug.print(F("-"));
    debug.println(fileBuffer[f]);
  }
  */
};

/*
fileBufferSize_t AbsoluteToCircularIndex(fileNum_t fileNo)
{
  return ArrayToCircularIndex(filesBegin - fileNo);
}

fileBufferSize_t ArrayToCircularIndex(fileNum_t arrayIndex)
{
  return (firstFileIndex + arrayIndex) % fileBufferSize;
}
*/

/*
  bool sdExplorer::saveNextFileName(fileNum_t i)
  {
  readNextDir(dir);
  if ()
  {
    fileBuffer[i] = file.dirIndex();
    //file.getName(fileBuffer[i], fileNameSize);
    filesPosition++;
    if (filesSize == fileBufferSize) //about to overwrite first file
    {
      //IncrementCircularArray
      ++firstFileIndex;
      ++filesBegin;
    Serial.print(F("incca"));
    } else {
      ++filesSize; //fill until buffer full
    Serial.print(F("incs"));
    }
    //filesBegin = filesPosition - filesSize + 1;
  debugp();

    //if (file.isDir()) {fileBuffer[i][fileNameSize - 1] = '/';}
  file.close();
    return true;
  } else {
    Serial.print(F("openNext fail"));
    return false;
  }
  }

  bool sdExplorer::getFileName(fileNum_t fileNo)
  {
  if (fileNo == filesPosition)
  {
    //filearray_current = filearray_push();
    Serial.println(F("a1"));
    debugp();
    return saveNextFileName(ArrayToCircularIndex(fileNo));
  } else {
    if (fileNo >= filesBegin && fileNo < (filesBegin + filesSize))
    {
      //filearray_current = filearray_item(fileNo - filesBegin);
    Serial.println(F("a2"));
      debugp();
      return true;
    } else {
      // fileNo < filesBegin, need to read from beginning
      rewind();//sd_dir_rewind();
      for (fileNum_t f = 0; f < fileNo; f++)
      {
        if (!saveNextFileName(ArrayToCircularIndex(fileNo))) {
          return false;
        }
      }
    //filearray_current = filearray_push();
    Serial.println(F("a3"));
    return saveNextFileName(ArrayToCircularIndex(fileNo));
  }
  }
  }
*/

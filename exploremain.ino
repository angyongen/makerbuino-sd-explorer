
typedef int8_t fileBufferSize_t; // file # in buffer
typedef int8_t fileNameSize_t;
typedef int16_t fileNum_t; // real file # on sd card
  //sdExplorer sdExplorer(dir, file);
void state_explore()
{
  if (dir.open("/")) {
  //sdExplorer.showMenu(gb);
  
  fileNum_t fileNo = 0;
  boolean exit = false;
  rewind();
  while (!exit) {
    if (gb.update()) {
        //for (fileNum_t f = 0; f < circularBuffer.capacity; f++)
      if (gb.buttons.pressed(BTN_A))
      {
        rewind();
      }
      if (gb.buttons.pressed(BTN_DOWN))
      {
        if (getFileName(fileNo)) ++fileNo;
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



const fileBufferSize_t fileBufferSize = 5;
//const fileNameSize_t fileNameSize = 10;

    fileBufferSize_t firstFileIndex; // the location where the first element is located in the circular buffer
    fileNum_t filesPosition; //the file # that readNext will return
    fileNum_t filesBegin; // the first file # stored in fileBuffer
    fileNum_t filesSize; // the number of files stored in fileBuffer
    uint16_t fileBuffer[fileBufferSize];
    
 
inline void debugp()
{
  
    Serial.print(F("filesPos"));
    Serial.println(filesPosition);
    Serial.print(F("filesSz"));
    Serial.println(filesSize);
        for (fileNum_t f = 0; f < filesSize; f++)
        {
          debug.print(f);
          debug.print(F("-"));
          debug.println(fileBuffer[f]);
        }
}
};


fileBufferSize_t AbsoluteToCircularIndex(fileNum_t fileNo)
{
  return ArrayToCircularIndex(filesBegin - fileNo);
}

fileBufferSize_t ArrayToCircularIndex(fileNum_t arrayIndex)
{
  return (firstFileIndex + arrayIndex) % fileBufferSize;
}



bool sdExplorer::saveNextFileName(fileNum_t i)
{
  if (file.openNext(&dir, O_READ))
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

bool rewind()
{
  debug.println(F("rewind"));

  dir.rewind();
  filesPosition = 0;
  filesBegin = 0;
  filesSize = 0;
  firstFileIndex = 0;
  //circularBuffer.clear();

  return true;
}

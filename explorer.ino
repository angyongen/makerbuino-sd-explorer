#define screen gb.display
#define debug Serial

#include <Gamebuino.h>
#include <SdFat.h>
//#include "circularBuffer.cpp"
SdFat sd;

typedef int8_t fileBufferSize_t;
typedef int8_t fileNameSize_t;
typedef int16_t fileNum_t;

const fileBufferSize_t fileBufferSize = 10;
const fileNameSize_t fileNameSize = 13;

class sdExplorer
{
 private:
  inline bool saveNextFileName(fileNum_t i);
  Gamebuino gb;
 public:
  FatFile file;
  FatFile dir;
  fileNum_t filesPosition;
  fileNum_t filesBegin;
  fileNum_t filesSize;
  char fileBuffer[fileBufferSize][fileNameSize];
  circularBufferInfo<fileBufferSize_t> circularBuffer;

  sdExplorer()
  {
    dir = *sd.vwd();
    filesPosition = 0;
    filesBegin = 0;
    filesSize = 0;
  };
  
  fileNum_t showMenu();
  bool rewind();
  bool getFileName(fileNum_t fileNo);
};

fileNum_t sdExplorer::showMenu()
{
  fileNum_t fileNo = 0;
  boolean exit = false;
  while (!exit) {
    if (gb.update()) {
      if (gb.buttons.pressed(BTN_A))
      {
        rewind();
      }
      if (gb.buttons.pressed(BTN_DOWN))
      {
        getFileName(fileNo++);
      }
      if (gb.buttons.pressed(BTN_B))
      {
        //enterDirectory();
      }
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
    }
  }
}

bool sdExplorer::saveNextFileName(fileNum_t i)
{
  if (&dir != &file) {file.close();}
  if (file.openNext(&dir, O_READ))
  {
    file.getName(fileBuffer[i], fileNameSize);
    if (file.isDir()) {fileBuffer[i][fileNameSize - 1] = '/';}
    if (filesPosition == (filesBegin + filesSize)) 
    {
      if (filesPosition != circularBuffer.capacity) filesSize++;
      filesBegin = filesPosition - filesSize + 1;
      debug.print(F("filesPos"));
  debug.println(filesPosition);
      debug.print(F("filesSz"));
  debug.println(filesSize);
    }
    filesPosition++;
    return true;
  } else {
    return false;
  }
}

bool sdExplorer::getFileName(fileNum_t fileNo)
{
  if (fileNo == filesPosition)
  {
    filearray_current = filearray_push();
    _debug.print(filearray_current);
    return sd_saveNextFileName(filearray_current);
  } else {
    if (fileNo >= filesBegin && fileNo < (filesBegin + filesSize)) 
    {
      filearray_current = filearray_item(fileNo - filesBegin);
      _debug.print(filearray_current);
      return true;
    }
    // last resort
    sd_dir_rewind();
    for (realNum_t f = 0; f < fileNo; f++) 
    {
      if(!sd_saveNextFileName(filearray_push())) {return false;}
    }
    filearray_current = filearray_push();
    return sd_saveNextFileName(filearray_current);
  }
}

bool sdExplorer::rewind()
{
  debug.println(F("rewind"));
  
  dir.rewind();
  filesPosition = 0;
  filesBegin = 0;
  filesSize = 0;
  circularBuffer.clear();
  
  return true;
}

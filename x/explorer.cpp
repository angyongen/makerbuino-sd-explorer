
#ifndef GAMEBUINO_SD_EXPLORER
#define	GAMEBUINO_SD_EXPLORER

#define screen gb.display
#define debug Serial

//#include "circularBuffer.cpp"
#include <Gamebuino.h>
#include <SdFat.h>
SdFat sd;

typedef int8_t fileBufferSize_t; // file # in buffer
typedef int8_t fileNameSize_t;
typedef int16_t fileNum_t; // real file # on sd card

const fileBufferSize_t fileBufferSize = 10;
const fileNameSize_t fileNameSize = 13;

class sdExplorer
{
  private:
    inline bool saveNextFileName(fileNum_t i);
  public:
    FatFile file;
    FatFile dir;
    fileBufferSize_t firstFileIndex; // the location where the first element is located in the circular buffer
    fileNum_t filesPosition; //the file # that readNext will return
    fileNum_t filesBegin; // the first file # stored in fileBuffer
    fileNum_t filesSize; // the number of files stored in fileBuffer
    char fileBuffer[fileBufferSize][fileNameSize];
    //circularBufferInfo<fileBufferSize_t> circularBuffer;
    fileBufferSize_t AbsoluteToCircularIndex(fileNum_t absoluteIndex);
    fileBufferSize_t ArrayToCircularIndex(fileNum_t arrayIndex);

    sdExplorer()
    {
      dir = *sd.vwd();
      filesPosition = 0;
      filesBegin = 0;
      filesSize = 0;
    };

    fileNum_t showMenu(Gamebuino &gb);
    bool rewind();
    bool getFileName(fileNum_t fileNo);
};


fileBufferSize_t sdExplorer::AbsoluteToCircularIndex(fileNum_t fileNo)
{
  return ArrayToCircularIndex(filesBegin - fileNo);
}

fileBufferSize_t sdExplorer::ArrayToCircularIndex(fileNum_t arrayIndex)
{
  return (firstFileIndex + arrayIndex) % fileBufferSize;
}

fileNum_t sdExplorer::showMenu(Gamebuino &gb)
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
}

bool sdExplorer::saveNextFileName(fileNum_t i)
{
  if (&dir != &file) {
    file.close();
  }
  if (file.openNext(&dir, O_READ))
  {
    file.getName(fileBuffer[i], fileNameSize);
    filesPosition++;
    if (filesSize == fileBufferSize) //about to overwrite first file
    {
      //IncrementCircularArray
      ++firstFileIndex;
      --filesBegin;
    } else {
      ++filesSize; //fill until buffer full
    }
    //filesBegin = filesPosition - filesSize + 1;
    debug.print(F("filesPos"));
    debug.println(filesPosition);
    debug.print(F("filesSz"));
    debug.println(filesSize);


    if (file.isDir()) {
      fileBuffer[i][fileNameSize - 1] = '/';
    }

    return true;
  } else {
    return false;
  }
}

bool sdExplorer::getFileName(fileNum_t fileNo)
{
  if (fileNo == filesPosition)
  {
    //filearray_current = filearray_push();
    //_debug.print(filearray_current);
    return saveNextFileName(ArrayToCircularIndex(fileNo));
  } else {
    if (fileNo >= filesBegin && fileNo < (filesBegin + filesSize))
    {
      //filearray_current = filearray_item(fileNo - filesBegin);
      //_debug.print(filearray_current);
      return true;
    }
    // fileNo < filesBegin, need to read from beginning
    rewind();//sd_dir_rewind();
    for (fileNum_t f = 0; f < fileNo; f++)
    {
      if (!saveNextFileName(ArrayToCircularIndex(fileNo))) {
        return false;
      }
    }
    //filearray_current = filearray_push();
    return saveNextFileName(ArrayToCircularIndex(fileNo));
  }
}

bool sdExplorer::rewind()
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

#endif	/* GAMEBUINO_SD_EXPLORER */
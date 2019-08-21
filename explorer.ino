typedef int8_t displayedFiles_t; // file # on screen

const displayedFiles_t displayedFiles = 6;

void explorer_redraw(FatFile & dir, FatPos_t & startPosition)
{
  gb.display.clear();
  dir_t entry;
  dir.setpos(&startPosition);
  gb.display.print(F("cluster "));
  gb.display.print(startPosition.cluster);
  gb.display.print(F(":"));
  gb.display.println(startPosition.position);
  for (displayedFiles_t f = 0; f < displayedFiles; f++)
  {
    if (readNextDir(dir, entry) <= 0) return;
    for (uint8_t i = 0; i < 11; i++) gb.display.print((char)entry.name[i]);
    gb.display.println();
  }
}

uint8_t explorer_loop(uint8_t prevLoop)
{
  FatFile dir;
  FatPos_t startPosition; // position on disk of first displayed file
  uint16_t file_index;

  if (dir.open("/")) {
    displayedFiles_t fileNo = 0;
    dir.rewind();
    dir.getpos(&startPosition);
    explorer_redraw(dir, startPosition);
    while (true) {
      if (gb.update()) {
        if (gb.buttons.pressed(BTN_A))
        {
          dir_t entry;
          dir.setpos(&startPosition);
          uint16_t n = readNextDir(dir, entry);
          if (n > 0) {
            --n;
            startPosition.position += 32 * n;
            dir.setpos(&startPosition);
            file_actions_loop(dir, entry, startPosition);
          } else {
            dir.open("/");
            dir.rewind();
          }
          explorer_redraw(dir, startPosition);
        }
        if (gb.buttons.repeat(BTN_UP, repeatTime))
        {
          startPosition.position -= 32;
          explorer_redraw(dir, startPosition);
        }
        if (gb.buttons.repeat(BTN_DOWN, repeatTime))
        {
          startPosition.position += 32;
          explorer_redraw(dir, startPosition);
        }
        if (gb.buttons.pressed(BTN_B))
        {
          //enterDirectory();
        }
        if (gb.buttons.pressed(BTN_C))
        {
        }
      }
    }
  } else {

  }
}

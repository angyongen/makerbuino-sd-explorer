

#define file_actions_count 2

void file_action_0_printText(FatFile & dir, dir_t & entry)
{
  if (DIR_IS_SUBDIR(&entry)) {
    gb.display.print(F("Open "));
  } else {
    gb.display.print(F("Flash "));
  }
  gb.display.print((char*)entry.name);
}
void file_action_0(FatFile & dir, dir_t & entry, FatPos_t & fileposition)
{
  if (DIR_IS_SUBDIR(&entry)) {
    if (!dir.open(&dir, fileposition.position / 32, O_READ)) {Serial.println(F("open fail"));}
    dir.getpos(&fileposition);
    return;
  } else {
    load_game_menu(dir, (char*)entry.name);
  }
}

void file_actions_loop(FatFile & dir, dir_t & entry, FatPos_t & fileposition)
{
  int8_t currentY = LCDHEIGHT / 2;
  int8_t targetY = 0;
  int8_t option = 0;
  entry.name[8] = 0; //end string before extension; only 9 bytes copied during load_game subroutine.
  while (true) {
    if (gb.update()) {
      if (gb.buttons.pressed(BTN_A))
      {
        switch (option)
        {
          case 0:
            file_action_0(dir, entry, fileposition);
            break;
          case 1:
            break;
        }
        return;
      }
      if (gb.buttons.pressed(BTN_B)) return;
      if (gb.buttons.pressed(BTN_UP)) --option;
      if (gb.buttons.pressed(BTN_DOWN)) ++option;

      if (option < 0) option = 0;
      if (option >= file_actions_count) option = file_actions_count - 1;

      targetY = (gb.display.fontHeight + 4) - gb.display.fontHeight * option; //center the menu on the active item
      currentY = (currentY + targetY) / 2; // slowly approach targetY by taking average
      //explorer_menu_redraw(currentY, option);

      gb.display.clear();
      gb.display.cursorX = 0;
      gb.display.cursorY = currentY;
      gb.display.fontSize = 1;
      gb.display.textWrap = false;
      for (byte i = 0; i < file_actions_count; i++) {
        if (gb.display.cursorY < -gb.display.fontHeight) {
          gb.display.cursorY += gb.display.fontHeight;
          continue;
        }
        if (i == option) {
          gb.display.cursorX = 3;
          gb.display.cursorY = currentY + option * gb.display.fontHeight;
        }
        switch (i)
        {
          case 0:
            file_action_0_printText(dir, entry);
            gb.display.println();
            break;
          case 1:
            gb.display.println(F("Edit"));
            break;
          case 2:
            gb.display.println(F("Open with..."));
            break;
        }
      }
      gb.display.setColor(WHITE);
      gb.display.drawFastHLine(0, currentY + gb.display.fontHeight * option - 1, LCDWIDTH);
      gb.display.setColor(BLACK);
      gb.display.drawRoundRect(0, currentY + gb.display.fontHeight * option - 2, LCDWIDTH, (gb.display.fontHeight + 3), 3);
    }
  }
}

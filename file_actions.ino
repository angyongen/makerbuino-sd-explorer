const uint8_t file_actions_count = 4;

uint8_t map_selectedOption_realOption(uint8_t available_file_actions, uint8_t option)
{
  uint8_t o = option;
  for (uint8_t t = 0; t < file_actions_count; ++t)
  {
    if ((available_file_actions & (1 << t)) == 0) continue;
    if (o-- == 0) return t;
  }
  return file_actions_count;
}

void file_actions_loop(FatFile & dir, dir_t & entry, uint32_t & filePosition)
{
  int8_t currentY = LCDHEIGHT / 2;
  int8_t targetY = 0;
  int8_t option = 0;
  uint8_t realOption = 0;
  uint8_t available_file_actions = 0; // 1 bit for each action

  if (!file_action_1_available(dir, entry, filePosition)) available_file_actions |= 1 << 0;
  if (!file_action_2_available(dir, entry, filePosition)) available_file_actions |= 1 << 1;
  if (!file_action_3_available(dir, entry, filePosition)) available_file_actions |= 1 << 2;
  if (!file_action_4_available(dir, entry, filePosition)) available_file_actions |= 1 << 3;
  available_file_actions = ~available_file_actions;
  realOption = map_selectedOption_realOption(available_file_actions, option);

  if (available_file_actions == 0) return; //no available actions

  entry.name[8] = 0; //end string before extension
  // also to prepare for the flash file action: only 9 bytes copied during load_game subroutine.
  while (true) {
    if (gb.update()) {
      if (gb.buttons.pressed(BTN_B)) return;
      if (gb.buttons.pressed(BTN_UP)) if (option > 0) --option;
      if (gb.buttons.pressed(BTN_DOWN)) ++option;
      realOption = map_selectedOption_realOption(available_file_actions, option);
      if (realOption >= file_actions_count) --option;
      realOption = map_selectedOption_realOption(available_file_actions, option);

      targetY = (gb.display.fontHeight + 4) - gb.display.fontHeight * option; //center the menu on the active item
#ifdef DISPLAYDIRECT
      currentY = targetY; // no animations when using direct display mode
#else
      currentY = (currentY + targetY) / 2; // slowly approach targetY by taking average
#endif
      //explorer_menu_redraw(currentY, option);

      gb.display.clear();
      gb.display.cursorX = 0;
      gb.display.cursorY = currentY;
      for (uint8_t i = 0; i < file_actions_count; i++) {
        if (!(available_file_actions & (1 << i))) continue;
        if (gb.display.cursorY < -gb.display.fontHeight) {
          gb.display.cursorY += gb.display.fontHeight;
          continue;
        }
        if (i == realOption) {
          gb.display.cursorX = gb.display.fontWidth;
          gb.display.cursorY = currentY + option * gb.display.fontHeight;
        }
        switch (i)
        {
          case 0:
            file_action_1_printText(dir, entry, filePosition);
            gb.display.println();
            break;
          case 1:
            file_action_2_printText(dir, entry, filePosition);
            gb.display.println();
            break;
          case 2:
            file_action_3_printText(dir, entry, filePosition);
            gb.display.println();
            break;
          case 3:
            file_action_4_printText(dir, entry, filePosition);
            gb.display.println();
            break;
        }
      }
      gb.display.setColor(WHITE);
      gb.display.drawFastHLine(0, currentY + gb.display.fontHeight * option - 1, LCDWIDTH);
      gb.display.setColor(BLACK);
      gb.display.drawRoundRect(0, currentY + gb.display.fontHeight * option - 2, LCDWIDTH, (gb.display.fontHeight + 3), 3);

      if (gb.buttons.pressed(BTN_A))
      {
        switch (realOption)
        {
          case 0:
            file_action_1(dir, entry, filePosition);
            return;
          case 1:
            file_action_2(dir, entry, filePosition);
            break;
          case 2:
            file_action_3(dir, entry, filePosition);
            break;
          case 3:
            file_action_4(dir, entry, filePosition);
            break;
        }
      }
    }
  }
}

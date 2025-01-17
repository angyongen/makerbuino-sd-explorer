const uint8_t explorer_displaySettings_cursorMax = 3;
void explorer_displaySettings_redraw(uint8_t cursor)
{
  gb.display.clear();
  if (cursor == 0) gb.display.print(F("\20"));
  if (explorer_option.showBlankEntries()) gb.display.print(F("Hide")); else gb.display.print(F("Show"));
  gb.display.print(F(" Blank Entries"));
  gb.display.println();
  if (cursor == 1) gb.display.print(F("\20"));
  if (explorer_option.showLFNEntries()) gb.display.print(F("Hide")); else gb.display.print(F("Show"));
  gb.display.print(F(" LFN"));
  gb.display.println();
  if (cursor == 2) gb.display.print(F("\20"));
  if (explorer_option.showDeletedEntries()) gb.display.print(F("Hide")); else gb.display.print(F("Show"));
  gb.display.print(F(" Deleted"));
  gb.display.println();
  if (cursor == 3) gb.display.print(F("\20"));
  if (explorer_option.showVolumeNameEntry()) gb.display.print(F("Hide")); else gb.display.print(F("Show"));
  gb.display.println(F(" VolumeName"));
}

bool explorer_displaySettings_loop()
{
  uint8_t cursor = 0;
  explorer_displaySettings_redraw(cursor);
  while (true)
  {
    if (gb.update())
    {
      if (gb.buttons.pressed(BTN_C)) return true;
      if (gb.buttons.pressed(BTN_B)) return true;
      if (gb.buttons.pressed(BTN_A)) {
        switch (cursor)
        {
          case 0:
            explorer_option.showBlankEntries(!explorer_option.showBlankEntries());
            if (!explorer_option.showBlankEntries()) {
              explorer_option.showLFNEntries(false);
              explorer_option.showDeletedEntries(false);
              explorer_option.showVolumeNameEntry(false);
            }
            break;
          case 1:
            explorer_option.showLFNEntries(!explorer_option.showLFNEntries());
            if (explorer_option.showLFNEntries()) explorer_option.showBlankEntries(true);
            break;
          case 2:
            explorer_option.showDeletedEntries(!explorer_option.showDeletedEntries());
            if (explorer_option.showDeletedEntries()) explorer_option.showBlankEntries(true);
            break;
          case 3:
            explorer_option.showVolumeNameEntry(!explorer_option.showVolumeNameEntry());
            if (explorer_option.showVolumeNameEntry()) explorer_option.showBlankEntries(true);
            break;
        }
        explorer_displaySettings_redraw(cursor);
      };
      if (gb.buttons.repeat(BTN_UP, repeatTime)) explorer_displaySettings_redraw((cursor == 0) ? cursor = explorer_displaySettings_cursorMax : --cursor);
      if (gb.buttons.repeat(BTN_DOWN, repeatTime)) explorer_displaySettings_redraw((cursor == explorer_displaySettings_cursorMax) ? cursor = 0 : ++cursor);
    }
  }
}

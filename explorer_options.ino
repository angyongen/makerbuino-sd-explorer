const uint8_t explorer_options_curMax = 3;
void explorer_options_redraw(uint8_t cursor)
{
  gb.display.clear();
  if (cursor == 0) gb.display.print(F("\20"));
  gb.display.print(F("Volume Properties"));
  gb.display.println();
  if (cursor == 1) gb.display.print(F("\20"));
  gb.display.print(F("Display Settings"));
  gb.display.println();
  if (cursor == 2) gb.display.print(F("\20"));
  gb.display.print(F("Rewind to beginning"));
  gb.display.println();
  if (cursor == 3) gb.display.print(F("\20"));
  gb.display.print(F("Exit"));
  gb.display.println();
}

bool explorer_options_loop(SdFat & sd, uint32_t & startPosition)
{
  uint8_t cursor = 0;
  explorer_options_redraw(cursor);
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
            volume_properties_loop(sd);
            break;
          case 1:
            explorer_displaySettings_loop();
            break;
          case 2:
            startPosition = 0;
            return true;
          case 3:
            return false;
        }
        explorer_options_redraw(cursor);
      };
      if (gb.buttons.repeat(BTN_UP, repeatTime)) explorer_options_redraw((cursor == 0) ? cursor = explorer_options_curMax : --cursor);
      if (gb.buttons.repeat(BTN_DOWN, repeatTime)) explorer_options_redraw((cursor == explorer_options_curMax) ? cursor = 0 : ++cursor);
    }
  }
}

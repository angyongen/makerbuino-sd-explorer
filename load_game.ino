
#define load_game (*((void(*)(const char* filename))(0x7ffc/2)))
void load_game_menu(FatFile & dir, char * filename)
{
  if (dir.isRoot()) {
    while (true)
    {
      gb.display.clear();
      gb.display.println(F("Preparing to flash"));
      gb.display.println(filename);
      gb.display.println();
      gb.display.println(F("\25 - OK"));
      gb.display.println();
      gb.display.println(F("\26 - Cancel"));
      waitForUpdate(gb);
      while (true)
      {
        if (gb.update())
        {
          if (gb.buttons.pressed(BTN_A))
          {
            gb.display.clear();
            gb.display.println(F("Loading, please wait"));
            gb.update();
            load_game(filename);
          }
          if (gb.buttons.pressed(BTN_B))
          {
            return;
          }
        }
      }
    }
  } else {

    while (true)
    {
      gb.display.clear();
      gb.display.println(F("WARNING: "));
      gb.display.println(filename);
      gb.display.println(F("is not in the root directory"));
      gb.display.println();
      gb.display.println();
      gb.display.println(F("Press \25 or \26 to cancel"));
      if (gb.buttons.pressed(BTN_A)) return;
      if (gb.buttons.pressed(BTN_B)) return;
    }
  }
}

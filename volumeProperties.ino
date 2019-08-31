void volume_properties_redraw()
{
  gb.display.clear();
  gb.display.print(F("FAT"));
  gb.display.print(sd.fatType());
  gb.display.println(F(" Volume"));
  gb.display.print(F("blocksPerCluster:"));
  gb.display.println(sd.blocksPerCluster());
  gb.display.print(F("blocksPerFat:"));
  gb.display.println(sd.blocksPerFat());
  gb.display.print(F("fatStartBlock:"));
  gb.display.println(sd.fatStartBlock());
  gb.display.print(F("rootDirStart:"));
  gb.display.println(sd.rootDirStart());
}

void volume_properties_loop()
{
  volume_properties_redraw();
  while (true)
  {
    if (gb.update())
    {
      if (gb.buttons.pressed(BTN_A)) return;
      if (gb.buttons.pressed(BTN_B)) return;
      if (gb.buttons.pressed(BTN_C)) return;
    }
  }
}

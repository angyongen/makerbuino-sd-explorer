void gb_waitForUpdate()
{
  while (!gb.update()) {}
}

void pause() {
    gb.display.persistence = true;
    while(true) 
    { 
      if(gb.update())
      { 
        if(gb.buttons.pressed(BTN_A)){break;}
        if(gb.buttons.pressed(BTN_B)){break;}
        if(gb.buttons.pressed(BTN_C)){break;}
      }
    }
}

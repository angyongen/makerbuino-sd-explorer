
#ifndef GAMEBUINO_TOOLS
#define GAMEBUINO_TOOLS

#include <Gamebuino.h>

void waitForUpdate(Gamebuino &gb)
{
  while (!gb.update()) {}
}

void pause(Gamebuino &gb) {
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
#endif /* GAMEBUINO_TOOLS */

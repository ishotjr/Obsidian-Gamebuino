/*
 * -----------------------------------------------------------------------------
 * 
 *       ::::::::  :::::::::   :::::::: ::::::::::: ::::::::: :::::::::::     :::     ::::    ::: 
 *     :+:    :+: :+:    :+: :+:    :+:    :+:     :+:    :+:    :+:       :+: :+:   :+:+:   :+:  
 *    +:+    +:+ +:+    +:+ +:+           +:+     +:+    +:+    +:+      +:+   +:+  :+:+:+  +:+   
 *   +#+    +:+ +#++:++#+  +#++:++#++    +#+     +#+    +:+    +#+     +#++:++#++: +#+ +:+ +#+    
 *  +#+    +#+ +#+    +#+        +#+    +#+     +#+    +#+    +#+     +#+     +#+ +#+  +#+#+#     
 * #+#    #+# #+#    #+# #+#    #+#    #+#     #+#    #+#    #+#     #+#     #+# #+#   #+#+#      
 * ########  #########   ######## ########### ######### ########### ###     ### ###    ####       
 * 
 * Obsidian chronicles one inorganic lifeform's quest through
 * space to save the last remaining members of its race.
 *
 * -----------------------------------------------------------------------------
 *
 * 0.1 - initial Gamebuino PoC
 * 
 * -----------------------------------------------------------------------------
 *
 */
 
#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

// Obsidian spacecraft position and size
int craft_x = LCDWIDTH / 8;
int craft_y = LCDHEIGHT / 2;
int craft_size = 4;

void setup() {

  gb.begin();
  gb.titleScreen(F("Obsidian"));

}

void loop() {
  
  if (gb.update()) {

    // process inputs

    // up/down
    if (gb.buttons.repeat(BTN_UP, 2)) {
      if (craft_y > 0) {
        craft_y--;
      }
      gb.sound.playTick();
    }
    if (gb.buttons.repeat(BTN_DOWN, 2)) {
      if (craft_y < (LCDHEIGHT - craft_size)) {
        craft_y++;      
      }
      gb.sound.playTick();
    }

    // shoot
    if (gb.buttons.pressed(BTN_A)) {
      gb.sound.playOK();
    }
    
    // quit
    if (gb.buttons.pressed(BTN_C)) {
      gb.titleScreen(F("Obsidian"));
    }
    
    
    // update position
    gb.display.fillRect(craft_x, craft_y, craft_size, craft_size);

  }
  
}

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

// projectile position, size and state
int projectile_x = 0;
int projectile_y = 0;
int projectile_size = 2;
bool projectile_active = false;

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

      // only shoot if projectile not already active
      if (!projectile_active) {

        projectile_active = true;
        
        // start in front/center of craft
        projectile_x = craft_x + craft_size;
        projectile_y = craft_y + ((craft_size - projectile_size) / 2);
        
        gb.sound.playOK();
      }
    }
    
    // quit
    if (gb.buttons.pressed(BTN_C)) {
      gb.titleScreen(F("Obsidian"));
    }
    
    
    // update craft position
    gb.display.fillRect(craft_x, craft_y, craft_size, craft_size);

    // update projectile position
    if (projectile_active) {
      gb.display.fillRect(projectile_x, projectile_y, projectile_size, projectile_size);
      // continue forward to edge of screen
      // TODO: collision w/ enemy!
      if (projectile_x < LCDWIDTH) {
        projectile_x++;
      } else {
        // it's gone!
        projectile_active = false;
      }
    }

  }
  
}

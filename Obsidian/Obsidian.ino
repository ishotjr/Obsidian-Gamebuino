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

extern const byte font3x3[];

// Obsidian spacecraft position and size
int craft_x = LCDWIDTH / 8;
int craft_y = LCDHEIGHT / 2;
int craft_size = 4;

// projectile(s) position, size and state
const int projectiles = 3;
int projectileIndex = 0;
int projectile_x[projectiles];
int projectile_y[projectiles];
int projectile_size[projectiles];
int projectile_speed_x[projectiles];
bool projectile_active = false;
bool projectile_available = true;

void setup() {

  gb.begin();
  // remember to use F() for strings in order to save RAM!
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

      // only shoot if max. projectiles not already active
      if (projectile_available) {

        projectile_active = true;
        
        // start in front/center of craft
        
        // TODO: find available slot and fill

        projectile_size[projectileIndex] = 2;
        projectile_speed_x[projectileIndex] = 1;
        projectile_x[projectileIndex] = craft_x + craft_size;
        projectile_y[projectileIndex] = craft_y + ((craft_size - projectile_size[projectileIndex]) / 2);

        // TODO: this is not really viable once projectiles start hitting things
        projectileIndex = (projectileIndex + 1) % projectiles;
        
        gb.sound.playOK();
      }
    }
    
    // quit
    if (gb.buttons.pressed(BTN_C)) {
      gb.titleScreen(F("Obsidian"));
    }
    
    
    // update craft position
    gb.display.fillRect(craft_x, craft_y, craft_size, craft_size);

    // update projectile(s) position - skip if none active
    if (projectile_active) {

      // TODO: add projectile_active setting to skip this if none
      
      for (int i = 0; i < projectiles; i++) {

        // ignore if immobile (?)
        if (projectile_speed_x[i] > 0) {
          gb.display.fillRect(projectile_x[i], projectile_y[i], projectile_size[i], projectile_size[i]);
        }
        
        // continue forward to edge of screen
        // TODO: collision w/ enemy!

        if (projectile_x[i] < LCDWIDTH) {
          projectile_x[i] += projectile_speed_x[i];
        } else {
          // it's gone - free up slot
          projectile_x[i] = 0;
          projectile_speed_x[i] = 0;
          projectile_available = true;
        }
        
      } 
    
    }

    // display CPU load during dev
    gb.display.setFont(font3x3);
    gb.display.cursorX = LCDWIDTH - (8 * 4);
    gb.display.cursorY = LCDHEIGHT - 4;
    gb.display.print(F("CPU:"));
    gb.display.print(gb.getCpuLoad());
    gb.display.print(F("%"));
  }
  
}

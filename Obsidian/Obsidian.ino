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

// TODO: optimize types etc.

// Obsidian spacecraft position and size
int craft_x = LCDWIDTH / 8;
int craft_y = LCDHEIGHT / 2;
int craft_size = 4;

// projectile(s) position, size and state
const int projectiles = 3;
const int projectile_size_default = 2;
const int projectile_speed_default = 1;
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
        
        // find available (aka immobile) slot and fill
        projectileIndex = -1;
        for (int i = 0; i < projectiles; i++) {
          if (projectile_speed_x[i] == 0) {
            projectileIndex = i;
            break;
          }
        }

        if (projectileIndex >= 0) {
          
          // launch projectile from front/center of craft with default size and speed
          projectile_size[projectileIndex] = projectile_size_default;
          projectile_speed_x[projectileIndex] = projectile_speed_default;
          projectile_x[projectileIndex] = craft_x + craft_size;
          projectile_y[projectileIndex] = craft_y + ((craft_size - projectile_size[projectileIndex]) / 2);

          gb.sound.playOK();
        } else {
          // turns out we can't shoot, even if we thought we could
          gb.sound.playCancel();
        }
        
      } else {
        // can't shoot
        gb.sound.playCancel();        
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

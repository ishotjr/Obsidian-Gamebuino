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
 * 0.1m - initial Gamebuino META PoC
 * 
 * -----------------------------------------------------------------------------
 *
 */
 
#include <SPI.h>

// TODO: not sure if a "real" solution like this exists?
#define GB_META
#ifdef GB_META
#include <Gamebuino-Meta.h>

// this is a silly way to get quick META compatibility for now
#define LCDWIDTH gb.display.width()
#define LCDHEIGHT gb.display.height()
#define BTN_UP BUTTON_UP
#define BTN_DOWN BUTTON_DOWN
#define BTN_A BUTTON_A
#define BTN_C BUTTON_MENU

#else
#include <Gamebuino.h>
Gamebuino gb;
#endif


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

int baddie_x = (7 * LCDWIDTH) / 8;
int baddie_y = LCDHEIGHT / 2;
int baddie_radius = 3;
int baddie_speed_y = 1;
int baddie_health_default = 4;
int baddie_health = baddie_health_default;

int baddie_health_gauge_w = 12;
int baddie_health_gauge_h = 4;
int baddie_health_gauge_x = (3 * LCDWIDTH) / 5;
int baddie_health_gauge_y = 1;


void setup() {

  gb.begin();

#ifndef GB_META
  // CLASSIC only
  
  // remember to use F() for strings in order to save RAM!
  gb.titleScreen(F("Obsidian"));
#endif

}

void loop() {
  
  if (gb.update()) {

#ifdef GB_META
    // META requires explicit clear at beginning of loop
    gb.display.clear();
#endif

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
#ifdef GB_META
      // TODO: re-init?
#else
      // CLASSIC only
      
      // remember to use F() for strings in order to save RAM!
      gb.titleScreen(F("Obsidian"));
#endif
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
        
        // is projectile colliding with baddie? (if it's there)
        // TODO: update to use actual circle vs. giant box,
        // and entire projectile vs. just single point
        int baddie_bounds_x_min = baddie_x - baddie_radius;
        int baddie_bounds_x_max = baddie_x + baddie_radius;
        int baddie_bounds_y_min = baddie_y - baddie_radius;
        int baddie_bounds_y_max = baddie_y + baddie_radius;
        if ((baddie_health > 0) && 
          ((projectile_x[i] >= baddie_bounds_x_min) && (projectile_x[i] <= baddie_bounds_x_max)) && 
          ((projectile_y[i] >= baddie_bounds_y_min) && (projectile_y[i] <= baddie_bounds_y_max))) {
          
            baddie_health--;

            // it's gone - free up slot
            projectile_x[i] = 0;
            projectile_speed_x[i] = 0;
            projectile_available = true;

        } else {
          // continue forward to edge of screen
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
    
    }

    // quick test of what shooting baddie w/b like
    if (baddie_health > 0) {
      gb.display.fillCircle(baddie_x, baddie_y, baddie_radius);
    } else {
      // respawn every 15s (approx, based on 20fps)
      // TODO: something better, e.g. relative to death time?
      if (gb.frameCount % 300 == 0) {
        baddie_health = baddie_health_default;
      }
    }

    // move baddie
    baddie_y = baddie_y + baddie_speed_y;
    // TODO: incorporate baddie size1
    if (((baddie_y + baddie_radius) > LCDHEIGHT) || ((baddie_y - baddie_radius) < 0)) {
      baddie_speed_y = baddie_speed_y * -1;
    }

    // baddie health gauge
    gb.display.drawRect(baddie_health_gauge_x, baddie_health_gauge_y, baddie_health_gauge_w, baddie_health_gauge_h);
    gb.display.fillRect(baddie_health_gauge_x, baddie_health_gauge_y, 
      (baddie_health_gauge_w * baddie_health) / baddie_health_default, baddie_health_gauge_h);

    // display CPU load during dev
    gb.display.setFont(font3x3);
    gb.display.cursorX = 0; //LCDWIDTH - (8 * 4);
    gb.display.cursorY = LCDHEIGHT - 4;
    gb.display.print(F("CPU:"));
    gb.display.print(gb.getCpuLoad());
    gb.display.print(F("%"));
  }
  
}

#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "analogSound.h"

#include "images/sprite1.h"
#include "images/sprite2.h"
#include "images/play.h"
#include "images/play.h"

Player player = {
    .x = 30,
    .y = 80,
    .width = 16,
    .height = 16,
    .dx = 1,
    .dy = 1,
    .frame = 0,
    .score = 0
  };
  
Target targets[MAX_TARGETS];

extern u32 currentButtons;
extern int framePlay;

int targetCooldown = 0;

void updatePlayer(void) {
    if (KEY_DOWN(BUTTON_UP, currentButtons) && player.y > 12) {
        player.y -= player.dy;
    }
    if (KEY_DOWN(BUTTON_DOWN, currentButtons) && player.y < 160 - player.height) {
        player.y += player.dy;
    }
    if (KEY_DOWN(BUTTON_LEFT, currentButtons) && player.x > 0) {
        player.x -= player.dx;
    }
    if (KEY_DOWN(BUTTON_RIGHT, currentButtons) && player.x < 240 - player.width) {
        player.x += player.dx;
    }

    if (frame % SPRITE_ANIM_SPEED == 0) {
        player.frame = !player.frame;
    }
}

void drawPlayer(void) {
    if (framePlay % SPRITE_ANIM_SPEED == 0) {
        player.frame = !player.frame;
      }
    
      if (player.x >= 0 && player.x <= 240) {
        drawTransparentImageDMA(player.y, player.x, player.width, player.height,
                                player.frame == 0 ? sprite1 : sprite2);
      }
}
  
void updateTargets(void) {
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].active) {
            targets[i].x -= targets[i].dx;

            if (!targets[i].justSpawned && targets[i].x <= WIDTH - targets[i].width) {
                targets[i].justSpawned = 1;
            }

            if (targets[i].justSpawned &&
                checkCollision(player.x, player.y, player.width, player.height,
                               targets[i].x, targets[i].y, targets[i].width, targets[i].height)) {
                playAnalogSound(9);
                targets[i].active = 0;
                targets[i].cooldown = 60;
                targets[i].justSpawned = 0;
                player.score += 5;
            }

            if (targets[i].x < 0) {
                targets[i].active = 0;
                targets[i].cooldown = 60;
                targets[i].justSpawned = 0;
            }
        } else {
            if (targets[i].cooldown > 0) {
                targets[i].cooldown--;
            }
        }
    }

    if (framePlay % 50 == 0) {
        for (int i = 0; i < MAX_TARGETS; i++) {
            if (!targets[i].active && targets[i].cooldown == 0) {
                targets[i].x = WIDTH + 4;
                targets[i].y = 10 + rand() % (HEIGHT - 17);
                targets[i].width = 3;
                targets[i].height = 3;
                targets[i].dx = 1;
                targets[i].active = 1;
                targets[i].justSpawned = 0;
                targets[i].cooldown = -1;
                targets[i].color = RGB(rand() % 32, rand() % 32, rand() % 32);
                break;
            }
        }
    }
}


  
void drawTargets(void) {
    for (int i = 0; i < MAX_TARGETS; i++) {
      if (targets[i].active && targets[i].justSpawned) {
        drawRectDMA(targets[i].y, targets[i].x, targets[i].width, targets[i].height, targets[i].color);
      }
    }

    
}
  
int checkCollision(int x1, int y1, int w1, int h1,
    int x2, int y2, int w2, int h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 &&
    y1 < y2 + h2 && y1 + h1 > y2;
}

#ifndef MAIN_H
#define MAIN_H

#include "gba.h"


#define SPRITE_ANIM_SPEED 10
#define SPRITE_Y 130
#define SPRITE_SPEED 1
#define BLINKY_Y 122

#define MAX_TARGETS 20
extern Color colors[];


enum TargetState {
    TARGET_INACTIVE,
    TARGET_COOLDOWN,
    TARGET_ACTIVE,
  };
  
// TODO: Create any necessary structs
typedef struct {
    int x, y;
    int width, height;//16*16
    int dx, dy;
    int frame;
    int score;
} Player;

typedef struct {
    int x, y;
    int width, height;
    int dx;
    int active;
    int justSpawned;
    int cooldown;
    Color color;
} Target;


extern Player player;
extern Target targets[MAX_TARGETS];
extern int frame;
extern int totalTargetsEaten;
extern int currentEnemySprite;

void updatePlayer(void);
void drawPlayer(void);

void updateTargets(void);
void drawTargets(void);


int checkCollision(int x1, int y1, int w1, int h1,
                   int x2, int y2, int w2, int h2);


#endif

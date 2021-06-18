#ifndef _ENEMY_H
#define _ENEMY_H

#include <stdlib.h>
#include "primlib.h"

typedef struct Target
{
  short int x;
  short int y;

  enum color kolor;
  short int width;
  short int height;

  char x_dir;
  char y_dir;
  char speed;

  char lives;
  char points;
  short int bullet_distance;
  
  struct Target *next;
  struct Target *prev;
} Target;

typedef struct Bomb
{
  short int x;
  short int y;
  short int width;
  
  char damage;
  char speed;
} Bomb;


Target *init_target(char);
void draw_target(Target *target);
void move_target(Target *target);
Bomb *init_bomb(short int x, short int y, short int width);

#endif /* _TARGET_H_ */
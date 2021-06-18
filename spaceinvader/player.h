#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdlib.h>
#include "primlib.h"

typedef struct Player
{
  short int x;
  short int y;

  short int width;
  short int height;
  enum color kolor;

  char health;
  int points;
} Player;

typedef struct Bullet
{
  short int x;
  short int y;

  short int width;
  short int height;
  enum color kolor;

  char is_used;
  char speed;
} Bullet;

Player *init_player();
Bullet *init_bullet(Player * player);
void move_bullet(Bullet *bullet, Player * player);
void draw_bullet(Bullet * bullet, Player * player);
void draw_player(Bullet *, Player *);
void new_level(int);

#endif /* _PLAYER_H_ */
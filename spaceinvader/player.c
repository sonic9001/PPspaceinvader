#include "player.h"

Player *init_player()
{
    Player *player = calloc(1, sizeof(Player));
    if (player == NULL)
    {
        printf("Player initialisation failed.\n");
        return NULL;
    }

    player->height = 30;
    player->width = 60;
    player->x = gfx_screenWidth() / 2;
    player->y = gfx_screenHeight() - player->height - 10;
    player->kolor = WHITE;
    player->health = 100;
    player->points = 0;

    return player;
}

Bullet *init_bullet(Player *player)
{
    int i;
    Bullet *bullet = calloc(3, sizeof(Bullet));

    if (bullet == NULL)
    {
        printf("Bullet initialisation error\n");
        return NULL;
    }

    for (i = 0; i < 3; ++i)
    {
        bullet[i].is_used = 0;
        bullet[i].height = 30;
        bullet[i].width = 20;
        bullet[i].speed = 5;
        bullet[i].kolor = RED;
        bullet[i].x = player->x;
        bullet[i].y = player->y;
    }

    return bullet;
}

void draw_bullet(Bullet *bullet, Player *player)
{
    if (!bullet->is_used)
    {
        gfx_filledTriangle(player->x - player->width / 6, player->y + player->height / 2,
                           player->x + player->width / 6, player->y + player->height / 2,
                           player->x, player->y - player->height / 2, bullet->kolor);
    }
    else
    {
        gfx_filledTriangle(bullet->x - bullet->width / 2, bullet->y + bullet->height / 2,
                           bullet->x + bullet->width / 2, bullet->y + player->height / 2,
                           bullet->x, bullet->y - bullet->height / 2, bullet->kolor);
    }
}

void move_bullet(Bullet *bullet, Player *player)
{
    if (bullet->is_used)
        bullet->y -= 10.0;
    else
    {
        bullet->x = player->x;
        bullet->y = player->y;
    }

    if (bullet->y < 0)
    {
        bullet->is_used = 0;
        bullet->x = player->x;
        bullet->y = player->y;
    }
}

void draw_player(Bullet *bullet, Player *player)
{
    /*Lewy dolny trojkat*/
    gfx_filledTriangle(player->x - player->width / 2, player->y + player->height / 2,
                       player->x - player->width / 3, player->y - player->height / 2,
                       player->x - player->width / 6, player->y + player->height / 2, player->kolor);

    /*Prawy gorny trojkat*/
    gfx_filledTriangle(player->x + player->width / 2, player->y + player->height / 2,
                       player->x + player->width / 3, player->y - player->height / 2,
                       player->x + player->width / 6, player->y + player->height / 2, player->kolor);

    /*Duzy gorny trojkat*/
    gfx_filledTriangle(player->x - player->width / 3, player->y - player->height / 2,
                       player->x + player->width / 3, player->y - player->height / 2,
                       player->x, player->y - 2 * player->height, player->kolor);
}
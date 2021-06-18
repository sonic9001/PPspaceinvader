#include <stdlib.h>

#include "primlib.h"
#include "target.h"
#include "list.h"
#include "player.h"
#include "level.h"

int main()
{
  if (gfx_init())
    exit(3);

  /*Init*/
  char tick = 0;
  int i;

  /*Inicjalizacja ustawien poziomu*/
  struct level_conf level;

  level.current_level = 0;
  level.enemies_left = 0;
  level.gen_new_level = 1;
  level.next_level_enemies = 5;

  char cooldown = 0;
  char shootin_timer = 0;

  /*Player init*/
  Player *player = init_player();
  if (player == NULL)
    return -1;

  /*Bullets init*/
  /*Generowane są 3 pociski, ktorymi strzela sie polautomatem*/
  Bullet *bullet = init_bullet(player);
  if (bullet == NULL)
    return -1;

  /*Lists init*/
  List *lista = List_create();
  if (lista == NULL)
    return -1;

  Bomb *bomb = NULL;

  char points[50] = " ";
  char lvl_info[50] = " ";

  while (!gfx_isKeyDown(SDLK_q) && player->health > 0)
  {
    /*Rysowanie*/
    /*Rysowanie tła*/
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
    /*Rysowanie gracza*/
    draw_player(bullet, player);

    /*Rysowanie pociskow*/
    for (i = 0; i < 3; ++i)
    {
      draw_bullet(&bullet[i], player);
    }
    /*Rysowanie targetow*/
    for (lista->current = lista->first; lista->current != NULL; lista->current = lista->current->next)
    {
      draw_target(lista->current);
    }

    /*Jeśli bomba jest w ruch to nalezy ja rysowac*/
    if (bomb != NULL)
    {
      gfx_filledTriangle(bomb->x - bomb->width / 2, bomb->y - bomb->width / 2, bomb->x + bomb->width / 2, bomb->y - bomb->width / 2,
                         bomb->x, bomb->y + bomb->width / 2, YELLOW);
    }

    /*Rysowanie paska zycia*/
    gfx_filledRect(gfx_screenWidth() / 2 - 50, gfx_screenHeight() - 10, gfx_screenWidth() / 2 - 50 + player->health, gfx_screenHeight(), RED);

    /*Rysowanie napisu z liczba punktow oraz informacja o aktualnym poziomie*/
    gfx_textout(5, gfx_screenHeight() - 10, points, RED);
    gfx_textout(gfx_screenWidth() - 100, gfx_screenHeight() - 10, lvl_info, RED);

    /*Napis rozpoczynajacy gre*/
    if(!tick){
      gfx_textout(gfx_screenWidth()/2 - 80, gfx_screenHeight()/2, "Press any key to start!", GREEN);
    }

    /*Odswiezenie ekranu*/
    gfx_updateScreen();
    /*Koniec Rysowania*/
    
    /*Stany*/
    /*Pauza zwiazana z napisem rozpoczynajacym gre*/
    if(!tick){
      gfx_getkey();
    }
    
    /*Generowanie nastepnego poziomu*/
    if(lista->count == 0){
      gen_new_level(&level);
      cooldown = 0;
    }

    /*Uzupełnianie wrogów*/
    if (level.enemies_left > 0 && cooldown == 0){
      if (List_push_enemy(lista, tick)){
        printf("Pushing failed\n");
      }
      cooldown = 50;
      level.enemies_left--;
      }

    /*Ruch pocisku*/
    for (i = 0; i < 3; ++i)
    {
      move_bullet(&bullet[i], player);
    }
    
    /*Ruch bomby*/
    if (bomb != NULL)
    {
      if (bomb->y <= gfx_screenHeight())
        bomb->y += bomb->speed;
      else
      {
        free(bomb);
        bomb = NULL;
      }
    }
    
    /*Poruszanie sie targetami*/
    for (lista->current = lista->first; lista->current != NULL; lista->current = lista->current->next)
    {
      move_target(lista->current);
    }

    /*Sprawdzanie kolizji target-pocisk*/
    for (lista->current = lista->first; lista->current != NULL; lista->current = lista->current->next)
    {
      for (i = 0; i < 3; i++)
      {
        if (hypot(lista->current->x - bullet[i].x, lista->current->y - bullet[i].y) < 100)
        {
          lista->current->lives -= 1;
          player->points += 1;
          bullet[i].is_used = 0;
          bullet[i].x = player->x;
          bullet[i].y = player->y;
          
          /*Generowanie bomby w przypadku trafienia*/
          if (bomb == NULL)
          {
            bomb = calloc(1, sizeof(Bomb));
            bomb->x = lista->current->x;
            bomb->y = lista->current->y + lista->current->width;
            bomb->width = lista->current->width / 2;
            bomb->damage = lista->current->points;
            bomb->speed = lista->current->speed * 2;
          }
        }
      }
    }

    for (lista->current = lista->first; lista->current != NULL; lista->current = lista->current->next)
    {
      /*Generowanie bomby w przypadku wejscia miedzy linie y = 500 oraz y = 400 oraz kiedy wspolrzedna x targetu jest w odleglosci 30 od wspolrzedniej x gracza*/
      if (lista->current->y < 500 && lista->current->y > 400 && lista->current->x - player->x < 30 && lista->current->x - player->x > -30 && bomb == NULL)
      {
        bomb = calloc(1, sizeof(Bomb));
        bomb->x = lista->current->x;
        bomb->y = lista->current->y + lista->current->width;
        bomb->width = lista->current->width / 2;
        bomb->damage = lista->current->points;
        bomb->speed = lista->current->speed * 2;
      }
    }
    /*Uderzenie przeciwnika przez bombę*/
    if (bomb != NULL && hypot(bomb->x - player->x, bomb->y - player->y) < 30)
    {
      player->health -= bomb->damage;
      free(bomb);
      bomb = NULL;
    }

    /*Usunięcie targetu po wyswietleniu animacji eksplozji*/
    for (lista->current = lista->first; lista->current != NULL;)
    {
      if (lista->current->lives < -15)
      {
        lista->current = List_remove(lista, lista->current);
        tick = 1; //Nadanie wiekszej losowosci, cooldown juz nie jest zblizony do tick
      }
      else
      {
        lista->current = lista->current->next;
      }
    }

    /*Ruch naszego statku*/
    if (gfx_isKeyDown(SDLK_RIGHT) && player->x + player->width / 2 <= gfx_screenWidth())
      player->x += 5;
    if (gfx_isKeyDown(SDLK_LEFT) && player->x - player->width / 2 >= 0)
      player->x -= 5;
    
    /*Obsluga strzelania*/
    for (i = 0; i < 3; ++i)
    {
      if (gfx_isKeyDown(SDLK_SPACE) && !bullet[i].is_used && !shootin_timer)
      {
        bullet[i].is_used = 1;
        shootin_timer = 15;
        break;
      }
    }

    /*Generowanie napisu z punktacja oraz poziomem*/
    sprintf(points, "Points: %d", player->points);
    sprintf(lvl_info, "Level: %d", level.current_level);
    
    /*Obsluga zmiennych 'czasowych'*/
    if (++tick >= 100)
    {
      tick = 1;
    }
    
    if (cooldown > 0)
      --cooldown;

    if (shootin_timer > 0)
      --shootin_timer;
    /*Koniec Stanów*/
    SDL_Delay(15);
  }
  /*Obraz konczacy program*/
  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
  gfx_textout(gfx_screenWidth()/2, gfx_screenHeight() / 2, "GAME OVER :(", GREEN);
  gfx_updateScreen();
  SDL_Delay(3000);

  /*Zwolnienie zaalokowanej pamieci*/
  free(player);
  free(bullet);
  List_purge(lista);
  if (bomb != NULL)
    free(bomb);

  return 0;
}
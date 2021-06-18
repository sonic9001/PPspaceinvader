#include "target.h"
#include "player.h"
#include "list.h"

Target *init_target(char tick)
{
    /*Inicjalizacja targetu. Zwraca wskaznik do utworzonego obiektu. Zwraca NULL jeśli alokacja pamieci sie nie powiedzie*/
    Target *ptr;

    if ((ptr = calloc(1, sizeof(Target))) == NULL)
    {
        printf("Target calloc failed\n");
        return NULL;
    }
    /*W zaleznosci od wartosci zmiennej tick tworzy rozny rodzaj targetu*/
    if (!(tick % 4))
    {
        /*Target drugiego stopnia*/
        ptr->height = ptr->width = 40;
        ptr->kolor = BLUE;
        ptr->lives = 3;
        ptr->speed = 3;
        ptr->points = 3;
    }
    else if (!(tick % 5))
    {
        /*Najmocniejszy target*/
        ptr->height = ptr->width = 60;
        ptr->kolor = GREEN;
        ptr->lives = 5;
        ptr->speed = 2;
        ptr->points = 5;
    }
    else
    {
        /*Najslabszy target*/
        ptr->height = ptr->width = 20;
        ptr->kolor = MAGENTA;
        ptr->lives = 1;
        ptr->speed = 5;
        ptr->points = 1;
    }

    /*jesli na najmlodszej pozycji wystepuje 1 to % 2 jest rowne 1*/
    tick % 2 ? (ptr->x_dir = 1) : (ptr->x_dir = -1);
    tick % 2 ? (ptr->y_dir = 1) : (ptr->y_dir = -1);

    tick % 2 ? (ptr->x = ptr->width) : (ptr->x = gfx_screenWidth() - ptr->width);
    ptr->y = tick;

    ptr->next = NULL;
    ptr->prev = NULL;

    return ptr;
}

void draw_target(Target *target)
{
    /*Jesli zmienna lives typu target jest dodatnia to go rysuje*/
    if (target->lives > 0)
    {
        /*Gorne kolo*/
        gfx_filledCircle(target->x, target->y - target->height/4, target->width/4, target->kolor);
        
        /*Lewe kolo*/
        gfx_filledCircle(target->x - target->width/2, target->y, target->width/4, target->kolor);
        
        /*Prawe kolo*/
        gfx_filledCircle(target->x + target->width/2, target->y, target->width/4, target->kolor);
        
        /*Cialo targetu*/
        gfx_filledRect(target->x - target->width / 2, target->y - target->height / 4, target->x + target->width / 2, target->y + target->height / 4, target->kolor);
        
        /*Pocisk*/
        gfx_filledTriangle(target->x - target->width/4, target->y + target->width/4,
                            target->x + target->width/4, target->y + target->width/4,
                            target->x, target->y + target->width/2 , YELLOW);
    }
    else
    {
        /*Jesli zmienna lives jest ujemna, rysowana jest eksplozja*/
        gfx_filledCircle(target->x, target->y, target->lives-- * (-1) * 15, YELLOW);
    }
}

void move_target(Target *target)
{
    /*Funkcja odpowiedzialna za ruch targetu. Porusza sie on po planszy a jego ruchy ograniczane sa bokami planszy oraz pozioma linia na wysokosci 500px */
    if (target->x > gfx_screenWidth())
        target->x_dir = -1;
    else if (target->x < 0)
        target->x_dir = 1;

    if (target->y > 500)
        target->y_dir = -1;
    else if (target->y < 0)
        target->y_dir = 1;
    if (target->lives > 0)
    {
        target->x += target->x_dir * target->speed;
        target->y += target->y_dir * target->speed;
    }
}

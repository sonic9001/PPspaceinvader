#include "list.h"

List *List_create()
{
    /*Funkcja odpowiedzialna za alokacje pamieci dla tablicy.*/
    List *ptr;
    ptr = calloc(1, sizeof(List));

    if (!ptr)
    {
        printf("List initialisation failed\n");
        return NULL;
    }

    return ptr;
}

int List_push_enemy(List *list, char tick)
{
    /*Funkcja odpowiedzialna za dodawanie elementu do listy*/
    /*By target zostal dodany do listy, najpierw jest tworzony*/
    Target *target = init_target(tick);

    if (target == NULL)
    {
        printf("Pushing failed\n");
        return -1;
    }

    if (list->last == NULL)
    {
        list->first = target;
        list->last = target;
    }
    else
    {
        list->last->next = target;
        target->prev = list->last;
        list->last = target;
    }
    list->count++;
    return 0;
}

Target *List_remove(List *list, Target *target)
{
    /*Funkcja odpowiedzialna za usuwanie targetu z listy.*/

    /*Sprawdzenie czy lista jest pusta*/
    if (!(list->first) && !(list->last))
    {
        printf("Lista pusta\n");
        return NULL;
    }
    list->count--;
    /*Sprawdzenie czy target jest jedyną wartością na liście*/
    if (target == list->first && target == list->last)
    {
        list->first = NULL;
        list->last = NULL;
        free(target);
        return NULL;
    }
    else if (target == list->first)
    {
        /*Sprawdzenie czy usuwany target jest pierwszy na liscie*/
        list->first = target->next;
        list->first->prev = NULL;
        free(target);
        return list->first;
    }
    else if (target == list->last)
    {
        /*Sprawdzenie czy usuwany target nie jest ostatnia wartoscia na liscie*/
        list->last = target->prev;
        list->last->next = NULL;
        free(target);
        return NULL;
    }
    else
    {
        Target *after = target->next;
        Target *before = target->prev;
        after->prev = before;
        before->next = after;
        free(target);

        return after;
    }
}

void List_purge(List *list)
{
    /*Usuwanie listy*/
    /*Sprawdzenie czy lista jest pusta*/
    if (list->first != NULL)
    {
        for (list->current = list->first->next; list->current != NULL; list->current = list->current->next)
        {
            free(list->current->prev);
            list->count--;
        }
        free(list->last);
        list->count--;
    }
    free(list);
}

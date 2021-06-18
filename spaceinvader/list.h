#ifndef _LIST_H_
#define _LIST_H_

#include "target.h"
#include <stdlib.h>

typedef struct List{
  Target * first;
  Target * last;
  Target * current;
  int count;
} List;

List *List_create();
int List_push_enemy(List * list, char tick);
Target * List_remove(List * list, Target * target);
void List_purge(List *list);

#endif /* _LIST_H_ */
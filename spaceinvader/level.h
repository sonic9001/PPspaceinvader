#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>

#include "primlib.h"

struct level_conf {
    int enemies_left;
    int next_level_enemies;
    int current_level;
    char gen_new_level;
  };

void gen_new_level(struct level_conf * level);

#endif
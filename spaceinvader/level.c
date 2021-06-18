#include "level.h"

void gen_new_level(struct level_conf * level){
    level->gen_new_level = 0;
    level->enemies_left = level->next_level_enemies;
    level->next_level_enemies += 5;
    level->current_level++;
}

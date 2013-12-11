#include "my_strategy.h"

#include <math.h>
#include <stdlib.h>

void move(const struct ct_trooper *self, const struct ct_world *world,
	 const struct ct_game *game, struct ct_move *move)
{
    int i;

    if (self->action_points < game->standing_move_cost) {
        return;
    }

    if (self->action_points >= self->shoot_cost) {
        for (i = 0; i < world->troopers_count; ++i) {
            const struct ct_trooper *trooper = &world->troopers[i];
            if (!trooper->is_teammate)
                if (is_visible(world, self->shooting_range, self->position, self->stance, trooper->position, trooper->stance)) {
                    move->action = ACT_SHOOT;
                    move->position = trooper->position;
                    return;
                }
        }
    }
    
    move->action = ACT_MOVE;

    if (rand() % 2 == 0) {
        move->direction = (rand() & 1) && self->position.y > 0
            && CELL_FREE == world->cells[self->position.y - 1][self->position.x] ? DIR_NORTH : DIR_SOUTH;
    } else {
        move->direction = (rand() & 1) && self->position.x > 0
            && CELL_FREE == world->cells[self->position.y][self->position.x - 1] ? DIR_WEST : DIR_EAST;
    }

}

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

    move->action = ACT_MOVE;

    if (0 == rand() % 2) {
        move->direction = (0 == rand() % 2) ? DIR_NORTH : DIR_SOUTH;
    } else {
        move->direction = (0 == rand() % 2) ? DIR_WEST : DIR_EAST;
    }

}

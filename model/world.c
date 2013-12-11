#include "world.h"

void world_dispose(struct ct_mutable_world *world)
{
    int i;
    int players_count = world->players_count;

    if (NULL != world->players) {
        for (i = 0; i < players_count; ++i) {
            struct ct_player *player = &world->players[i];
            if (NULL != player->name) {
                free((void*)player->name);
                player->name = NULL;
            }
        }
        free(world->players);
        world->players = NULL;
    }

    if (NULL != world->troopers) {
        free(world->troopers);
        world->troopers = NULL;
    }

    if (NULL != world->bonuses) {
        free(world->bonuses);
        world->bonuses = NULL;
    }

    if (NULL != world->cell_visibilities) {
        free(world->cell_visibilities);
        world->cell_visibilities = NULL;
    }
}
#ifndef _WORLD_H_
#define _WORLD_H_

#include "bonus.h"
#include "player.h"
#include "trooper.h"

#define MAP_WIDTH           30
#define MAP_WIDTH_ALIGNED   32
#define MAP_HEIGHT          20

enum ct_cell_type {
    CELL_UNKNOWN        = -1,
    CELL_FREE           = 0,
    CELL_LOW_COVER      = 1,
    CELL_MEDIUM_COVER   = 2,
    CELL_HIGH_COVER     = 3
};

typedef unsigned char ct_cell_visibilities_t[_STANCE_COUNT_][MAP_HEIGHT][MAP_WIDTH + 2][MAP_HEIGHT][MAP_WIDTH + 2];

struct ct_world {
    int move_index;
    int width;
    int height;
    const struct ct_player *players;
    int players_count;
    const struct ct_trooper *troopers;
    int troopers_count;
    const struct ct_bonus *bonuses;
    int bonuses_count;

    enum ct_cell_type cells[MAP_HEIGHT][MAP_WIDTH_ALIGNED];
    ct_cell_visibilities_t *cell_visibilities;
};

struct ct_mutable_world {
    int move_index;
    int width;
    int height;
    struct ct_player *players;
    int players_count;
    struct ct_trooper *troopers;
    int troopers_count;
    struct ct_bonus *bonuses;
    int bonuses_count;

    enum ct_cell_type cells[MAP_HEIGHT][MAP_WIDTH_ALIGNED];
    ct_cell_visibilities_t *cell_visibilities;
};

static ct_bool is_visible(const struct ct_world *world, double max_range,
                          struct ct_point viewer, enum ct_trooper_stance viewer_stance,
                          struct ct_point object, enum ct_trooper_stance object_stance) 
{
    int min_stance = object_stance > viewer_stance ? viewer_stance : object_stance;
    int dx = object.x - viewer.x;
    int dy = object.y - viewer.y;
    
    return dx * dx + dy *dy <= max_range * max_range
           && (*world->cell_visibilities)[min_stance][viewer.y][viewer.x][object.y][object.x];
}
/*
 * Free mutable copy of the world structure. User shouldn't call this function.
 */
void world_dispose(struct ct_mutable_world *world);

#endif

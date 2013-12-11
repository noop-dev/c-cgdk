#ifndef _MOVE_H_
#define _MOVE_H_

#include "unit.h"

enum ct_action_type {
    ACT_UNKNOWN = -1,
    ACT_END_TURN = 0,
    ACT_MOVE = 1,
    ACT_SHOOT = 2,
    ACT_RAISE_STANCE = 3,
    ACT_LOWER_STANCE = 4,
    ACT_THROW_GRENADE = 5,
    ACT_USE_MEDIKIT = 6,
    ACT_EAT_FIELD_RATION = 7,
    ACT_HEAL = 8,
    ACT_REQUEST_ENEMY_DISPOSITION = 9
};

enum ct_direction {
    DIR_UNKNOWN = -1,
    DIR_CURRENT_POINT = 0,
    DIR_NORTH = 1,
    DIR_EAST = 2,
    DIR_SOUTH = 3,
    DIR_WEST = 4
};

struct ct_move {
    enum ct_action_type action;
    enum ct_direction direction;
    struct ct_point position;
};

#endif

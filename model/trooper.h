#ifndef _TROOPER_H_
#define _TROOPER_H_

#include "unit.h"

enum ct_trooper_type {
    TROOPER_UNKNOWN         = -1,
    TROOPER_COMMANDER       = 0,
    TROOPER_FIELD_MEDIC     = 1,
    TROOPER_SOLDIER         = 2,
    TROOPER_SNIPER          = 3,
    TROOPER_SCOUT           = 4
};

enum ct_trooper_stance {
    STANCE_UNKNOWN          = -1,
    STANCE_PRONE            = 0,
    STANCE_KNEELING         = 1,
    STANCE_STANDING         = 2,
    _STANCE_COUNT_          = 3
};

struct ct_trooper {
    long long id;
    struct ct_point position;
    long long player_id;
    int teammate_index;
    ct_bool is_teammate;
    enum ct_trooper_type type;
    enum ct_trooper_stance stance;
    int hitpoints;
    int max_hitpoints;
    int action_points;
    int initial_action_points;
    double vision_range;
    double shooting_range;
    int shoot_cost;
    int standing_damage;
    int kneeling_damage;
    int prone_damage;
    int damage;
    ct_bool is_holding_grenade;
    ct_bool is_holding_medkit;
    ct_bool is_holding_field_ration;
};

static int get_trooper_damage(const struct ct_trooper *trooper, enum ct_trooper_stance stance) {
    switch (stance) {
        case STANCE_PRONE:
            return trooper->prone_damage;
        case STANCE_KNEELING:
            return trooper->kneeling_damage;
        case STANCE_STANDING:
            return trooper->standing_damage;
        default:
            exit(10030);
    }
}

#endif

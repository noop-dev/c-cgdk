#ifndef _BONUS_H_
#define _BONUS_H_

#include "unit.h"

enum ct_bonus_type {
    BONUS_UNKNOWN       = -1,
    BONUS_GRENADE       = 0,
    BONUS_MEDIKIT       = 1,
    BONUS_FIELD_RATION  = 2
};

struct ct_bonus {
    long long id;
    struct ct_point position;
    enum ct_bonus_type type;
};

#endif

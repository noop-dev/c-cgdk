#ifndef _PLAYER_H_
#define _PLAYER_H_

struct ct_player {
    long long id;
    const char * name;
    int score;
    ct_bool strategy_crashed;
    struct ct_point approximate_position;
};

#endif

#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "model/game.h"
#include "model/move.h"
#include "model/trooper.h"
#include "model/world.h"

extern void move(const struct ct_trooper *self, const struct ct_world *world,
	             const struct ct_game *game, struct ct_move *move);

#endif

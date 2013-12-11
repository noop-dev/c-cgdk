#ifndef _PLAYER_CONTEXT_H_
#define _PLAYER_CONTEXT_H_

#include "trooper.h"
#include "world.h"

struct ct_player_context {
    struct ct_trooper trooper;
    struct ct_mutable_world world;
};

static void player_context_dispose(struct ct_player_context *context)
{
    world_dispose(&context->world);
}

#endif

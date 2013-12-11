#include "runner.h"
#include "strategy.h"

int main(int argc, char* argv[])
{
    struct runner runner;
    if (argc == 4) {
        runner_init(&runner, argv[1], argv[2], argv[3]);
    } else {
        runner_init(&runner, "127.0.0.1", "31001", "0000000000000000");
    }
    runner_run(&runner);
    return 0;
}

void runner_init(struct runner * const runner, const char* host, const char* port, const char* token) 
{
    client_init(&runner->client, host, atoi(port), token);
}

static void move_init(struct ct_move *move)
{
    move->action = ACT_END_TURN;
    move->direction = DIR_UNKNOWN;
    move->position = point(-1, -1);
}

void runner_run(struct runner * const runner)
{
    int team_size;
    struct ct_game game;
    struct ct_player_context player_context;

    client_write_token(&runner->client, runner->client.token);
    team_size = client_read_team_size(&runner->client);
    client_write_protocol_version(&runner->client);

    client_read_game_context(&runner->client, &game);

    while (0 == client_read_player_context(&runner->client, &player_context)) {
        const struct ct_trooper *player_trooper = &player_context.trooper;

        struct ct_move player_move;
        move_init(&player_move);
        move(player_trooper, (const struct ct_world*)&player_context.world, &game, &player_move);
        client_write_move(&runner->client, &player_move);
        player_context_dispose(&player_context);
    }

	client_close(&runner->client);
}

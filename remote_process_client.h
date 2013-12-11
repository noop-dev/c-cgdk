#ifndef _REMOTE_PROCESS_CLIENT_H_
#define _REMOTE_PROCESS_CLIENT_H_

#include "platform/net.h"

#include "model/bonus.h"
#include "model/game.h"
#include "model/move.h"
#include "model/player.h"
#include "model/player_context.h"
#include "model/trooper.h"
#include "model/world.h"

#define CLIENT_WRITE_BUFFER_SIZE 0x8000

struct remote_process_client {
    SOCKET socket;
    int is_little_endian;
    const char *token;
    ct_bool cells_read, cell_visibilities_read;

    char write_buffer[CLIENT_WRITE_BUFFER_SIZE];
    char *write_pointer;
    


    enum ct_cell_type cells[MAP_HEIGHT][MAP_WIDTH + 2];
                /* [stance][a_y][a_x][b_y][b_x] */
    ct_cell_visibilities_t *cell_visibilities;
};

enum ct_message_type {
    MSG_UNKNOWN                 = 0,
    MSG_GAME_OVER               = 1,
    MSG_AUTHENTICATION_TOKEN    = 2,
    MSG_TEAM_SIZE               = 3,
    MSG_PROTOCOL_VERSION        = 4,
    MSG_GAME_CONTEXT            = 5,
    MSG_PLAYER_CONTEXT          = 6,
    MSG_MOVE_MESSAGE            = 7
};

void client_init(struct remote_process_client *client, const char* host, unsigned port, const char *token);
void client_close(struct remote_process_client *client);

void client_write_token(struct remote_process_client *client, const char *token);
void client_write_protocol_version(struct remote_process_client *client);
void client_write_move(struct remote_process_client *client, const struct ct_move *move);

int client_read_team_size(const struct remote_process_client *client);
void client_read_game_context(const struct remote_process_client *client, struct ct_game *game);

/*
 * Reads player context, returns 0 if success, -1 if the game has ended
 * Caller is responsible for calling player_context_dispose() on the returned structure, then
   it can be reused.
 */
int client_read_player_context(struct remote_process_client *client, struct ct_player_context *context);


#endif

#include "remote_process_client.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct remote_process_client remote_process_client;

#define WRITE_BYTE(x) client_write_byte(client, x)
#define WRITE_INT32(x) client_write_int32(client, x)
#define WRITE_STRING(x) client_write_string(client, x)
#define FLUSH() client_flush_buffer(client);

#define READ_BYTE() client_read_byte(client)
#define READ_BOOL() (ct_bool)READ_BYTE()
#define READ_ENUM(enum_type) (enum enum_type)(signed char)client_read_byte(client)
#define READ_INT32() client_read_int32(client)
#define READ_INT64() client_read_int64(client)
#define READ_DOUBLE() client_read_double(client)
#define READ_STRING() client_read_string(client);

#define VERIFY(actual_value, expected_value) if (actual_value != expected_value) { exit(10011); }

#define FIX_BYTE_ORDER(size)                    \
    if (!client->is_little_endian)              \
        reverse_in_place((char*)&(data), size);

static int is_little_endian()
{
    union {
        uint16_t value;
        unsigned char bytes[2];
    } test = {0x0201};
    
    return 1 == test.bytes[0]; 
}

static void reverse_in_place(char *data, size_t size)
{
    char *end = data + (size - 1);
    while (end > data) {
        char tmp = *data;
        *data++ = *end;
        *end-- = tmp;
    }
}

static void reverse(char *dest, const char *src, size_t size) 
{
    src += size - 1;
    while (size--) {
        *dest++ = *src--;
    }
}

static unsigned char client_read_byte(const remote_process_client *client)
{
    unsigned char data;
    if (0 != socket_recv(client->socket, (char*)&data, 1)) {
        exit(10012);
    }
    return data;
}

static void client_read_bytes(const remote_process_client *client, void *data, size_t size) 
{
    if (0 != socket_recv(client->socket, (char*)data, size)) {
        exit(10012);
    }
}

static int32_t client_read_int32(const remote_process_client *client)
{
    int32_t data;
    client_read_bytes(client, &data, 4);
    FIX_BYTE_ORDER(4);
    return data;
}

static int64_t client_read_int64(const remote_process_client *client)
{
    int64_t data;
    client_read_bytes(client, &data, 8);
    FIX_BYTE_ORDER(8);
    return data;
}

static double client_read_double(const remote_process_client *client)
{
    double data;
    client_read_bytes(client, &data, 8);
    FIX_BYTE_ORDER(8);
    return data;
}

static char *client_read_string(const remote_process_client *client)
{
    int32_t length = READ_INT32();
    char *str;
    if (length < 0) {
        exit(10014);
    }

    str = (char*)malloc(length + 1);
    if (NULL == str) {
        /* Out of memory, probably incorrect length */
        exit(10014);
    }

    client_read_bytes(client, str, (size_t)length);
    str[length] = 0;
    return str;
}

int client_read_boolean(const remote_process_client *client)
{
    return 0 != client_read_byte(client);
}

static void client_flush_buffer(remote_process_client *client)
{
    if (client->write_pointer == client->write_buffer)
        return;

    if (0 != socket_send(client->socket, client->write_buffer, client->write_pointer - client->write_buffer)) {
        exit(10013);
    }
    client->write_pointer = client->write_buffer;
}

static void client_write_bytes(remote_process_client *client, const void *data, size_t size)
{
    ptrdiff_t bytes_left;
    while ((bytes_left = client->write_buffer + CLIENT_WRITE_BUFFER_SIZE - client->write_pointer) <= (ptrdiff_t)size) {
        memcpy(client->write_pointer, data, bytes_left);
        data = (const char*)data + bytes_left;
        size -= bytes_left;
        client->write_pointer += bytes_left;
        client_flush_buffer(client);
    }
    memcpy(client->write_pointer, data, size);
    client->write_pointer += size;
}

static void client_write_byte(remote_process_client *client, unsigned char x)
{
    client_write_bytes(client, &x, 1);
}

static void client_write_int32(remote_process_client *client, int32_t data)
{
    FIX_BYTE_ORDER(4);
    client_write_bytes(client, &data, 4);
}

static void client_write_double(remote_process_client *client, double data)
{
    FIX_BYTE_ORDER(8);
    client_write_bytes(client, &data, 8);
}

static void client_write_string(remote_process_client *client, const char *str)
{
    int32_t length = (int32_t)strlen(str);
    WRITE_INT32(length);
    client_write_bytes(client, str, length);
}

void client_init(remote_process_client *client, const char* host, unsigned port, const char *token)
{
    
    client->is_little_endian = is_little_endian();
    client->socket = socket_init_tcp();
    client->token = token;
    client->write_pointer = client->write_buffer;

    client->cells_read = client->cell_visibilities_read = 0;
    memset(client->cells, 0, sizeof(client->cells));
    client->cell_visibilities = NULL;

    if (0 != socket_connect(client->socket, host, port)) {
        exit(10001);
    }

    socket_enable_nagle_algorithm(client->socket, 0);
}

void client_close(remote_process_client *client)
{
    if (INVALID_SOCKET != client->socket) {
        socket_close(client->socket);
        client->socket = INVALID_SOCKET;
    }
    if (NULL != client->cell_visibilities) {
        free(client->cell_visibilities);
    }
}

void client_write_token(remote_process_client *client, const char *token)
{
    WRITE_BYTE(MSG_AUTHENTICATION_TOKEN);
    WRITE_STRING(token);
    FLUSH();
}

void client_write_protocol_version(remote_process_client *client)
{
    WRITE_BYTE(MSG_PROTOCOL_VERSION);
    WRITE_INT32(2);
    FLUSH();
}

void client_write_move(remote_process_client *client, const struct ct_move *move)
{
    WRITE_BYTE(MSG_MOVE_MESSAGE);
    WRITE_BYTE(1);
    WRITE_BYTE((signed char)move->action);
    WRITE_BYTE((signed char)move->direction);
    WRITE_INT32(move->position.x);
    WRITE_INT32(move->position.y);
    FLUSH();
}

int client_read_team_size(const remote_process_client *client) {
    VERIFY(READ_ENUM(ct_message_type), MSG_TEAM_SIZE);
    return READ_INT32();
}

void client_read_game_context(const remote_process_client *client, struct ct_game *game) {
    VERIFY(READ_ENUM(ct_message_type), MSG_GAME_CONTEXT);
    if (!READ_BOOL()) {
        exit(10017);
    }

    memset(game, 0xCC, sizeof(struct ct_game));
    game->move_count = READ_INT32();
    game->last_player_elimination_score = READ_INT32();
    game->player_elimination_score = READ_INT32();
    game->trooper_elimination_score = READ_INT32();
    game->trooper_damage_score_factor = READ_DOUBLE();
    game->stance_change_cost = READ_INT32();
    game->standing_move_cost = READ_INT32();
    game->kneeling_move_cost = READ_INT32();
    game->prone_move_cost = READ_INT32();
    game->commander_aura_bonus_action_points = READ_INT32();
    game->commander_aura_range = READ_DOUBLE();
    game->commander_request_enemy_disposition_cost = READ_INT32();
    game->commander_request_enemy_disposition_max_offset = READ_INT32();
    game->field_medic_heal_cost = READ_INT32();
    game->field_medic_heal_bonus_hitpoints = READ_INT32();
    game->field_medic_heal_self_bonus_hitpoints = READ_INT32();
    game->sniper_standing_stealth_bonus = READ_DOUBLE();
    game->sniper_kneeling_stealth_bonus = READ_DOUBLE();
    game->sniper_prone_stealth_bonus = READ_DOUBLE();
    game->sniper_standing_shooting_range_bonus = READ_DOUBLE();
    game->sniper_kneeling_shooting_range_bonus = READ_DOUBLE();
    game->sniper_prone_shooting_range_bonus = READ_DOUBLE();
    game->scout_stealth_bonus_negation = READ_DOUBLE();
    game->grenade_throw_cost = READ_INT32();
    game->grenade_throw_range = READ_DOUBLE();
    game->grenade_direct_damage = READ_INT32();
    game->grenade_collateral_damage = READ_INT32();
    game->medikit_use_cost = READ_INT32();
    game->medikit_bonus_hitpoints = READ_INT32();
    game->medikit_heal_self_bonus_hitpoints = READ_INT32();
    game->field_ration_eat_cost = READ_INT32();
    game->field_ration_bonus_action_points = READ_INT32();
}

static void client_read_players(const remote_process_client *client, struct ct_mutable_world *world) {
    int players_count = READ_INT32();
    int i;
    world->players_count = players_count;
    if (players_count < 0) {
        exit(10003);
    }

    world->players = (struct ct_player*)malloc(players_count * sizeof(struct ct_player));
    memset(world->players, 0xCC, players_count * sizeof(struct ct_player));

    for (i = 0; i < players_count; ++i) {
        struct ct_player *player = &world->players[i];
        if (!READ_BOOL()) {
            exit(10004);
        }
        player->id = READ_INT64();
        player->name = READ_STRING();
        player->score = READ_INT32();
        player->strategy_crashed = READ_BOOL();
        player->approximate_position.x = READ_INT32();
        player->approximate_position.y = READ_INT32();
    }
}


static void client_read_trooper(const remote_process_client *client, struct ct_trooper *trooper) {
    if (!READ_BOOL()) {
        exit(10006);
    }
    memset(trooper, 0xCC, sizeof(*trooper));

    trooper->id = READ_INT64();
    trooper->position.x = READ_INT32();
    trooper->position.y = READ_INT32();
    trooper->player_id = READ_INT64();
    trooper->teammate_index = READ_INT32();
    trooper->is_teammate = READ_BOOL();
    trooper->type = READ_ENUM(ct_trooper_type);
    trooper->stance = READ_ENUM(ct_trooper_stance);
    trooper->hitpoints = READ_INT32();
    trooper->max_hitpoints = READ_INT32();
    trooper->action_points = READ_INT32();
    trooper->initial_action_points = READ_INT32();
    trooper->vision_range = READ_DOUBLE();
    trooper->shooting_range = READ_DOUBLE();
    trooper->shoot_cost = READ_INT32();
    trooper->standing_damage = READ_INT32();
    trooper->kneeling_damage = READ_INT32();
    trooper->prone_damage = READ_INT32();
    trooper->damage = READ_INT32();
    trooper->is_holding_grenade = READ_BOOL();
    trooper->is_holding_medkit = READ_BOOL();
    trooper->is_holding_field_ration = READ_BOOL();
}

static void client_read_troopers(const remote_process_client *client, struct ct_mutable_world *world) {
    int i;
    int troopers_count = READ_INT32();
    world->troopers_count = troopers_count;

    if (troopers_count < 0) {
        exit(10005);
    }

    world->troopers = (struct ct_trooper*)malloc(troopers_count * sizeof(struct ct_trooper));
    memset(world->troopers, 0xCC, troopers_count * sizeof(struct ct_trooper));

    for (i = 0; i < troopers_count; ++i) {
        client_read_trooper(client, &world->troopers[i]);
    }
}

static void client_read_bonuses(const remote_process_client *client, struct ct_mutable_world *world) {
    int i;
    int bonuses_count = READ_INT32();
    world->bonuses_count = bonuses_count;

    if (bonuses_count < 0) {
        exit(10009);
    }

    world->bonuses = (struct ct_bonus*)malloc(bonuses_count * sizeof(struct ct_bonus));
    memset(world->bonuses, 0xCC, bonuses_count * sizeof(struct ct_bonus));

    for (i = 0; i < bonuses_count; ++i) {
        struct ct_bonus *bonus = &world->bonuses[i];
        if (!READ_BOOL()) {
            exit(10010);
        }
        bonus->id = READ_INT64();
        bonus->position.x = READ_INT32();
        bonus->position.y = READ_INT32();
        bonus->type = READ_ENUM(ct_bonus_type);
    }
}

static void client_read_cells(remote_process_client *client, struct ct_mutable_world *world) {
    int width, height, x, y;

    if (!client->cells_read)
    {
        client->cells_read = 1;
        memset(client->cells, 0, sizeof(client->cells));
        width = world->width;
        height = world->height;
        /* I won't actually read sizes here, instead I'll just verify their correctness */
        if (READ_INT32() != width) {
            exit(10007);
        }

        for (x = 0; x < width; ++x) {
            if (READ_INT32() != height) {
                exit(10008);
            }
            for (y = 0; y < height; ++y) {
                client->cells[y][x] = READ_ENUM(ct_cell_type);
            }
        }
    }
    assert(sizeof(client->cells) == sizeof(world->cells));
    memcpy(world->cells, client->cells, sizeof(client->cells));
}

static void client_read_cell_visibilities(remote_process_client *client, struct ct_mutable_world *world) {
    int width, height, x0, y0, x1, y1;

    if (!client->cell_visibilities_read)
    {
        client->cell_visibilities_read = 1;
        client->cell_visibilities = (ct_cell_visibilities_t *)malloc(sizeof(ct_cell_visibilities_t));
        memset(client->cell_visibilities, 0, sizeof(*client->cell_visibilities));
        width = world->width;
        height = world->height;
        /* I won't actually read sizes here, instead I'll just verify their correctness */
        if (READ_INT32() != width) {
            exit(10018);
        }
        if (READ_INT32() != height) {
            exit(10019);
        }
        if (READ_INT32() != _STANCE_COUNT_) {
            exit(10020);
        }

        for (x0 = 0; x0 < width; ++x0) {
            for (y0 = 0; y0 < height; ++y0) {
                for (x1 = 0; x1 < width; ++x1) {
                    for (y1 = 0; y1 < height; ++y1) {
                        int i;
                        ct_bool vis[_STANCE_COUNT_];
                        client_read_bytes(client, vis, _STANCE_COUNT_);
                        for (i = 0; i < _STANCE_COUNT_; ++i) {
                            (*client->cell_visibilities)[i][y0][x0][y1][x1] = vis[i];
                        }
                    }
                }
            }
        }
    }

    world->cell_visibilities = (ct_cell_visibilities_t *)malloc(sizeof(ct_cell_visibilities_t));
    memcpy(world->cell_visibilities, client->cell_visibilities, sizeof(*world->cell_visibilities));
}

static void client_read_world(remote_process_client *client, struct ct_mutable_world *world) {
    if (!READ_BOOL()) {
        exit(10002);
    }

    memset(world, 0xCC, sizeof(*world));

    world->move_index = READ_INT32();
    world->width = READ_INT32();
    world->height = READ_INT32();

    /* Will return error if the map sizes are bigger than we expect */
    if (world->width > MAP_WIDTH_ALIGNED) {
        exit(10007);
    }
    if (world->height > MAP_HEIGHT) {
        exit(10008);
    }

    client_read_players(client, world);
    client_read_troopers(client, world);
    client_read_bonuses(client, world);

    client_read_cells(client, world);
    client_read_cell_visibilities(client, world);
}

int client_read_player_context(remote_process_client *client, struct ct_player_context *context) {
    enum ct_message_type message_type = READ_ENUM(ct_message_type);
    if (MSG_GAME_OVER == message_type) {
        return -1;
    }
    VERIFY(message_type, MSG_PLAYER_CONTEXT);
    if (!READ_BOOL()) {
        return -1;
    }
    client_read_trooper(client, &context->trooper);
    client_read_world(client, &context->world);
    return 0;
}

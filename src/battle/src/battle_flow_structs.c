#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "battle/battle_flow_structs.h"

/* Stub for the player_new function in player.h game-state module */

battle_player_t *new_ctx_player(char* p_id, class_t *c_type, stat_t *stats, move_t *moves, battle_item_t* items)
{
      battle_player_t *ctx_player = calloc(1, sizeof(battle_player_t));
      assert(ctx_player != NULL);

      ctx_player->player_id = p_id;
      ctx_player->class_type = c_type;
      ctx_player->stats = stats;
      ctx_player->moves = moves;
      ctx_player->items = items;

      return ctx_player;
}

/* Sets up pointer to npc struct, stub for an npc representing the enemy */
// npc_enemy_t *make_npc_enemy(char* npc_id, class_t *c_type, stat_t *stats, move_t *moves, battle_item_t* items,difficulty_t ai)
// {
//     npc_enemy_t *npc_e = calloc(1, sizeof(npc_enemy_t));
//     assert(npc_e != NULL);

//     npc_e->npc_id = npc_id;
//     npc_e->class_type = c_type;
//     npc_e->stats = stats;
//     npc_e->moves = moves;
//     npc_e->items = items;
//     npc_e->ai = ai;
//     npc_e->prev = NULL;
//     npc_e->next = NULL;

//     return npc_e;
// }

/* Stub for the game_new function in game.h game-state module */
battle_game_t *new_battle_game()
{
      battle_game_t *g = calloc(1, sizeof(battle_game_t));
      assert(g != NULL);
      g->player = NULL;
      g->battle = NULL;

      return g;
}

#include <stdio.h>
#include "action_management/action_structs.h"
#include "action_management/actionmanagement.h"
#include <cli/operations.h>
#include "common/ctx.h"
#include "ui/ui.h"
#include "game-state/game.h"
#include "../src/custom-actions/include/interface.h"

/* This demo uses monkeypatching to showcase changes to custom-actions
and the player module */


/* Ascii art generated from 
https://patorjk.com/software/taag/#p=display&h=2&v=0&f=Poison&t=Actn-Mgmt%0A */
const char *banner =   
"\n\n @@@@@@    @@@@@@@  @@@@@@@  @@@  @@@             @@@@@@@@@@    @@@@@@@@  @@@@@@@@@@   @@@@@@@  \n"
"@@@@@@@@  @@@@@@@@  @@@@@@@  @@@@ @@@             @@@@@@@@@@@  @@@@@@@@@  @@@@@@@@@@@  @@@@@@@  \n"
"@@!  @@@  !@@         @@!    @@!@!@@@             @@! @@! @@!  !@@        @@! @@! @@!    @@!    \n"
"!@!  @!@  !@!         !@!    !@!!@!@!             !@! !@! !@!  !@!        !@! !@! !@!    !@!    \n"
"@!@!@!@!  !@!         @!!    @!@ !!@!  @!@!@!@!@  @!! !!@ @!@  !@! @!@!@  @!! !!@ @!@    @!!    \n"
"!!!@!!!!  !!!         !!!    !@!  !!!  !!!@!@!!!  !@!   ! !@!  !!! !!@!!  !@!   ! !@!    !!!    \n"
"!!:  !!!  :!!         !!:    !!:  !!!             !!:     !!:  :!!   !!:  !!:     !!:    !!:    \n"
":!:  !:!  :!:         :!:    :!:  !:!             :!:     :!:  :!:   !::  :!:     :!:    :!:    \n"
"::   :::   ::: :::     ::     ::   ::             :::     ::    ::: ::::  :::     ::      ::    \n"
" :   : :   :: :: :     :     ::    :               :      :     :: :: :    :      :       :  \n";


/* Creates a sample in-memory game */
chiventure_ctx_t *create_sample_ctx()
{
  game_t *game = game_new("Welcome to Chiventure!");

  /* Create two rooms (room1 and room2). room1 is the initial room */
  room_t *room1 = room_new("room1", "This is room 1", "Verily, this is the first room.");
  room_t *room2 = room_new("room2", "This is room 2", "Truly, this is the second room.");
  add_room_to_game(game, room1);
  add_room_to_game(game, room2);
  game->curr_room = room1;
  create_connection(game, "room1", "room2", "NORTH");


  player_t *player = player_new("test");
  add_player_to_game(game, player);
  game->curr_player = player;

  /* Create context */
  chiventure_ctx_t *ctx = chiventure_ctx_new(game);
  game = ctx->game;
  
  return ctx;
}

char *raiseDmg(char *tokens[TOKEN_LIST_SIZE], chiventure_ctx_t *ctx)
{
  attribute_t **args = malloc(sizeof(attribute_t*) *3);
  attribute_t *dmgIncrease = malloc(sizeof(attribute_t));
  char *name = "Dmg";
  attribute_value_t a1;
  a1.int_val = 3;
  dmgIncrease->attribute_key = strdup(name);
  dmgIncrease->attribute_tag = INTEGER;
  dmgIncrease->attribute_value = a1;

  attribute_t *wepDmg = get_attribute(get_item_in_hash(ctx->game->curr_player->inventory, "A sword"), "Dmg");
  args[0] = dmgIncrease;
  args[1] = wepDmg;
  args[2] = wepDmg;
  AST_block_t *actDmg = AST_action_block_new(ADDITION, 3, args);
  custom_action_t *CA = custom_action_new("damage increase", "item", "A sword", "action", actDmg);
  
  do_custom_action(CA);
  return "Damage Raised";
}

char *seeDmg(char *tokens[TOKEN_LIST_SIZE], chiventure_ctx_t *ctx)
{
  int num =  get_attribute(get_item_in_hash(ctx->game->curr_player->inventory, "A sword"), "Dmg")->attribute_value.int_val;
  char *str;
  sprintf(str, "%d", num);
  return str;
}

char *print_player_strength(char *tokens[TOKEN_LIST_SIZE], chiventure_ctx_t *ctx)
{
    player_t *player = ctx->game->curr_player;
    char *str;

    double strength = player_get_stat_current(player, "strength");

    if (strength == -1)
    {
        return "To you, strength is a foreign concept.";
    } else
    {
        sprintf(str, "Your strength is %.2f.", strength);

        return str;
    }
}

char *learn_strength(char *tokens[TOKEN_LIST_SIZE], chiventure_ctx_t *ctx)
{
    player_t *player = ctx->game->curr_player;

    double strength = player_get_stat_current(player, "strength");
    if (strength != -1)
    {
        return "You already have the strength stat.";
    }


    stats_global_t *strength_stat = stats_global_new("strength", 100);
    stats_t *s = stats_new(strength_stat, 100);

    int added = player_add_stat(player, s);

    return "Knowledge is power. You now have the strength stat.";
}

char *change_strength(char *tokens[TOKEN_LIST_SIZE], chiventure_ctx_t *ctx)
{
    if (tokens[1] == NULL)
    {
        return "Please give a strength value.";
    }

    char *str = malloc(100);
    player_t *player = ctx->game->curr_player;

    double strength = player_get_stat_current(player, "strength");

    if (strength == -1)
    {
        return "You do not know strength.";
    }

    double change = atoi(tokens[1]) + 0.0;

    int rc = player_change_stat(player, "strength", change);
    strength = player_get_stat_current(player, "strength");

    sprintf(str, "You have gained %.2f strength.", change);

    return str;
}

int main(int argc, char **argv)
{
    chiventure_ctx_t *ctx = create_sample_ctx();
    char *name = "Dmg";
    attribute_value_t a1;
    a1.int_val = 30;
    
    attribute_t *wepDmg = malloc(sizeof(attribute_t));
    wepDmg->attribute_key = strdup(name);
    wepDmg->attribute_tag = INTEGER;
    wepDmg->attribute_value = a1;
    item_t *sword = item_new("A sword", "A sword", "A sword");
    add_attribute_to_hash(sword, wepDmg);
    add_item_to_player(ctx->game->curr_player, sword);
 
    add_entry("SEEDMG", seeDmg, NULL, ctx->table);
    add_entry("RAISEDMG", raiseDmg, NULL, ctx->table);

    add_entry("MYSTRENGTH", print_player_strength, NULL, ctx->table);
    add_entry("LEARNSTRENGTH", learn_strength, NULL, ctx->table);
    add_entry("CHANGESTRENGTH", change_strength, NULL, ctx->table);

    /* Start chiventure */
    start_ui(ctx, banner);

    game_free(ctx->game);

    return 0;
}

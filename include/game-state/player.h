/*
 * Lindsey Hsi, Patrick Koopmans, Jessica Xia
 * A player structure
 */

#ifndef _PLAYER_H
#define _PLAYER_H

#include "game_state_common.h"
#include "stats.h"
#include "item.h"
#include "playerclass/class.h"

/* A player in game */
typedef struct player {
    /* hh is used for hashtable, as provided in uthash.h*/
    UT_hash_handle hh;

    /* Unique id identifying the player */
    char *player_id;

    /* The player's current health, separate from their maximum health 
    most likely going to be stored in player_stats in the future but must
    be kept to prevent old functions from breaKing */
    int health;

    /* The player's current level */
    int level;

    /* The cumulative total of experience points acquired by the player */
    int xp;

    /* A string containing the player's race */
    char *player_race;

    /* The player's current class. class_t contains the base health, stats, and skills for that class at
    the beginning of a game. These may change throughout the game, so their current states are stored 
    in the health, player_stats, player_skills fields in this player struct */
    class_t *player_class;

    /* All of the stats, with their values, the player has */
    stats_hash_t *player_stats;

    /* The current combat skills known to the player */
    skill_inventory_t *player_combat_skills;

    /* The current noncombat skills known to the player */
    skill_inventory_t *player_noncombat_skills;

    /* All of the effects the player is currently experiencing */
    effects_hash_t *player_effects;

    /* The current items held by the player*/
    item_hash_t *inventory;
} player_t;

/* This typedef is to distinguish between player_t pointers which are 
* used to point to the player_t structs themselves, and those which are used
* to hash player_t structs with the UTHASH macros as specified
* in src/common/include */
typedef struct player player_hash_t;

/*
 * Sets an allocated player_t object's player_class field to given class_t class
 *  as well as setting 
 *
 * Parameters:
 *  plyr: A player. Must point to already allocated memory.
 *  player_class: The player's class. Contains starting fields for
 *                skills and stats
 *
 * Returns:
 *  SUCCESS on success, FAILURE if an error occurs.
 */
int player_set_class(player_t *plyr, class_t *player_class);

/*
 * Allocates and creates a new player with given ID, starting at level
 *  1 with 0 xp and 5 health. 
 *
 * Parameters:
 *  player_id: the unique string ID of the player
 *
 * Returns:
 *  Pointer to allocated player
 */
player_t *player_new(char *player_id);

/*
 * Frees resources associated with a player
 *
 * Parameters:
 *  plyr: the player to be freed
 *
 * Returns:
 *  SUCCESS if successful
 */
int player_free(player_t *plyr);

/* Deletes a hashtable of players
 * Implemented with macros provided by uthash.h
 *
 * Parameters:
 *  hashtable of players that need to be deleted
 *
 * Returns:
 *  SUCCESS if successful
 */
int delete_all_players(player_hash_t* players);

/*
 * Returns the health of a player
 *
 * Parameters:
 *  plyr: the player
 *
 * Returns:
 *  int, the player's health
 */
int get_health(player_t *plyr);

/*
 * Changes the health of the player
 *
 * Parameters:
 *  plyr: the player
 *
 * Returns:
 *  int, remaining health
 */
int change_health(player_t *plyr, int change, int max);

/*
 * Returns the level of the player
 *
 * Parameters:
 *  plyr: the player
 *
 * Returns:
 *  int, the player's level
 */
int get_level(player_t *plyr);

/*
 * Increments the level of the player by given amt
 *
 * Parameters:
 *  plyr: the player
 *  change: the desired amount to increment in player level
 *
 * Returns:
 *  int, the new level
 */
int change_level(player_t *plyr, int change);

/*
 * Returns the experience points of the player
 *
 * Parameters:
 *  plyr: the player
 *
 * Returns:
 *  int, the player's experience
 */
int get_xp(player_t *plyr);

/*
 * Changes the experience (xp) points of the player
 *
 * Parameters:
 *  plyr: the player
 *  points: how much to change xp (positive or negative)
 *
 * Returns:
 *  int, the player's new xp
 */
int change_xp(player_t *plyr, int points);

/*
 * Returns the inventory list
 *
 * Parameters:
 *  plyr: the player
 *
 * Returns:
 *  hashtable of items, the inventory
 */
item_hash_t* get_inventory(player_t *plyr);


/* Adds an item to the given player
 *
 * Parameters:
 *  player struct
 *  item struct
 *
 * Returns:
 *  SUCCESS if successful, FAILURE if failed
 */
int add_item_to_player(player_t *player, item_t *item);

/* Removes an item from the given player
 * Note that the memory associated with this item is not freed
 * 
 * Parameters:
 *  player struct
 *  item struct
 * 
 * Returns:
 *  SUCCESS if successful, FAILURE if failed
 */
int remove_item_from_player(player_t *player, item_t *item);

/*
 * Function to get a linked list (utlist) of all the items in the player's inventory
 *
 * Parameters:
 *  player
 *
 * Returns:
 *  linked list of pointers to items (the head element)
 */
item_list_t *get_all_items_in_inventory(player_t *player);

/*
 * Returns if the item is in the player's inventory
 *
 * Parameters:
 *  player struct
 *  item struct
 *
 * Returns:
 *  true if item is inventory, false otherwise
 */
bool item_in_inventory(player_t *player, item_t *item);

/*
 * Assigns a table of stats to a player
 *
 * Parameters:
 *  player: the players
 *  sh: the stats hashtable
 *
 * Returns:
 *  SUCCESS on success, FAILURE if an error occurs.
 */
int assign_stats_player(player_t *plyr, stats_hash_t *sh);

#endif

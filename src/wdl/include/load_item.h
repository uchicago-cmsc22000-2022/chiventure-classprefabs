/*
 * Header file for loading item object into game struct
 */

#ifndef INCLUDE_LOAD_ITEM_H
#define INCLUDE_LOAD_ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parse.h"

/* load_items
 * accesses the document object and obtain list of items to parse into the
 * game_state structs and adds to the inputted game pointer
 * 
 * parameters:
 * - obj: the document object
 * - g: a game pointer as a return parameter
 *
 * side effects:
 * - adds items into the inputted game pointer and 
 *   their respective rooms
 *
 * returns:
 * - 0 for successful parse
 * - 1 for unsuccessful parse
 */
item_t* load_items(obj_t *doc, game_t *g );

/* load_actions
 * loads actions into items
 */
int load_actions(obj_t *doc, item_t *i);

#endif /* INCLUDE_LOAD_ITEM_H */

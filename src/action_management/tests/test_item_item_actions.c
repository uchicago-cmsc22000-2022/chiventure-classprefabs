#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "actionmanagement.h"
#include "action_structs.h"
#include "item.h"
#include "game.h"

#define BUFFER_SIZE (100)

bool execute_do_item_item_action(char *act_name, enum action_kind kind, char *allowed_act_name1, enum action_kind allowed_kind1, char *allowed_act_name2, enum action_kind allowed_kind2)
{
    player_t *player = player_new("player", 1);
    game_t *g = game_new("a dummy game");
    add_player_to_game(g, player);
    set_curr_player(g, player);
    action_type_t *a = action_type_new(act_name, kind);
    action_type_t *allowed_a1 = action_type_new(allowed_act_name1, allowed_kind1);
    action_type_t *allowed_a2 = action_type_new(allowed_act_name2, allowed_kind2);
    item_t *direct = item_new("direct", "The direct item", "The directmost object of interest");
    item_t *indirect = item_new("indirect", "The indirect item", "The indirectmost object of interest");
    add_allowed_action(direct, allowed_act_name1, allowed_a1);
    add_allowed_action(indirect, allowed_act_name2, allowed_a2);

    char *expected_output = malloc(BUFFER_SIZE);
    sprintf(expected_output, "Requested action %s with %s on %s",
            a->c_name, direct->item_id, indirect->item_id);

    bool rc;
    if (strcmp(do_item_item_action(g, a, direct, indirect), expected_output) == 0) 
    {
        rc = true;
    }
    else 
    {
        rc = false;
    }

    free(expected_output);
    item_free(direct);
    item_free(indirect);
    action_type_free(a);
    game_free(g);

    return rc;
}

Test(item_item_actions, wrong_kind_ITEM)
{
    bool rc = execute_do_item_item_action("dummy", ITEM, "dummy", ITEM, "dummy", ITEM);

    cr_assert_eq(rc, false,
                  "execute_do_item_item_action returned true for wrong kind 1");
}

Test(item_item_actions, wrong_kind_PATH)
{

    bool rc = execute_do_item_item_action("dummy", PATH, "dummy", PATH, "dummy", PATH);

    cr_assert_eq(rc, false,
                 "execute_do_item_item_action returned true for wrong kind 2");
    ;
}

Test(item_item_actions, correct_kind_ITEM_ITEM)
{
    bool rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy", ITEM_ITEM, "dummy", ITEM_ITEM);

    cr_assert_eq(rc, true,
                 "execute_do_item_item_action returned false for correct kind 3");
}

Test(item_item_actions, correct_allowed_actions)
{
    bool rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy", ITEM_ITEM, "dummy", ITEM_ITEM);

    cr_assert_eq(rc, true,
                 "execute_do_item_item_action returned false for correct allowed actions in indirect and direct");
}

Test(item_item_actions, wrong_allowed_actions_direct)
{
    bool rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy_allowed", ITEM_ITEM, "dummy", ITEM_ITEM);

    cr_assert_eq(rc, false,
                 "execute_do_item_item_action returned true for incorrect allowed actions name in direct");
}

Test(item_item_actions, wrong_allowed_actions_indirect)
{
    bool rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy", ITEM_ITEM, "dummy_allowed", ITEM_ITEM);

    cr_assert_eq(rc, false,
                 "execute_do_item_item_action returned true for incorrect allowed actions name in indirect");
}

Test(item_item_actions, wrong_allowed_actions)
{
    bool rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy_allowed", ITEM_ITEM, "dummy_allowed", ITEM_ITEM);

    cr_assert_eq(rc, false,
                 "execute_do_item_item_action returned true for incorrect allowed actions name in indirect and direct");
}

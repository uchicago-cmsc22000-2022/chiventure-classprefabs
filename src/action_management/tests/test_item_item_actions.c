#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include "actionmanagement.h"
#include "action_structs.h"
#include "item.h"
#include "game.h"
#include "player.h"

#define BUFFER_SIZE (100)
#define KIND_ERROR (1)
#define DIRECT_NOT_ALLOWED (2)
#define INDIRECT_NOT_ALLOWED (3)
#define WRONG_STRING (4)

int execute_do_item_item_action(char *act_name, enum action_kind kind, char *allowed_act_name1, enum action_kind allowed_kind1, char *allowed_act_name2, enum action_kind allowed_kind2)
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

    char *kind_error = malloc(BUFFER_SIZE);
    sprintf(kind_error, "The action type provided is not of the correct kind");

    char *allowed_direct_error = malloc(BUFFER_SIZE);
    sprintf(allowed_direct_error, "Action %s can't be requested on item %s",
            a->c_name, direct->item_id);

    char *allowed_indirect_error = malloc(BUFFER_SIZE);
    sprintf(allowed_indirect_error, "Action %s can't be requested on item %s",
            a->c_name, indirect->item_id);

    int rc;
    if (strcmp(do_item_item_action(g, a, direct, indirect), expected_output) == 0) 
    {
        rc = SUCCESS;
    }
    else if (strcmp(do_item_item_action(g, a, direct, indirect), kind_error) == 0)
    {
        rc = KIND_ERROR;
    }
    else if (strcmp(do_item_item_action(g, a, direct, indirect), allowed_direct_error) == 0)
    {
        rc = DIRECT_NOT_ALLOWED;
    }
    else if (strcmp(do_item_item_action(g, a, direct, indirect), allowed_indirect_error) == 0)
    {
        rc = INDIRECT_NOT_ALLOWED;
    }
    else
    {
        rc = WRONG_STRING;
    }

    free(expected_output);
    free(kind_error);
    free(allowed_direct_error);
    free(allowed_indirect_error);
    item_free(direct);
    item_free(indirect);
    action_type_free(a);
    game_free(g);

    return rc;
}

Test(item_item_actions, wrong_kind_ITEM)
{
    int rc = execute_do_item_item_action("dummy", ITEM, "dummy", ITEM, "dummy", ITEM);

    cr_assert_eq(rc, KIND_ERROR,
                 "execute_do_item_item_action returned %d for wrong kind 1, expected KIND_ERROR (1)", rc);
}

Test(item_item_actions, wrong_kind_PATH)
{

    int rc = execute_do_item_item_action("dummy", PATH, "dummy", PATH, "dummy", PATH);

    cr_assert_eq(rc, KIND_ERROR,
                 "execute_do_item_item_action returned %d for wrong kind 2, expected KIND_ERROR (1)", rc);
    ;
}

Test(item_item_actions, correct_kind_ITEM_ITEM)
{
    int rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy", ITEM_ITEM, "dummy", ITEM_ITEM);

    cr_assert_eq(rc, SUCCESS,
                 "execute_do_item_item_action returned %d for correct kind 3, expected SUCCESS (0)", rc);
}

Test(item_item_actions, correct_allowed_actions)
{
    int rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy", ITEM_ITEM, "dummy", ITEM_ITEM);

    cr_assert_eq(rc, SUCCESS,
                 "execute_do_item_item_action returned %d for correct allowed actions in indirect and direct, expected SUCCESS (0)", rc);
}

Test(item_item_actions, wrong_allowed_actions_direct)
{
    int rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy_allowed", ITEM_ITEM, "dummy", ITEM_ITEM);

    cr_assert_eq(rc, DIRECT_NOT_ALLOWED,
                 "execute_do_item_item_action returned %d for incorrect allowed actions name in direct, expected DIRECT_NOT_ALLOWED (2)", rc);
}

Test(item_item_actions, wrong_allowed_actions_indirect)
{
    int rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy", ITEM_ITEM, "dummy_allowed", ITEM_ITEM);

    cr_assert_eq(rc, INDIRECT_NOT_ALLOWED,
                 "execute_do_item_item_action returned %d for incorrect allowed actions name in indirect, expected INDIRECT_NOT_ALLOWED (3)", rc);
}

Test(item_item_actions, wrong_allowed_actions)
{
    int rc = execute_do_item_item_action("dummy", ITEM_ITEM, "dummy_allowed", ITEM_ITEM, "dummy_allowed", ITEM_ITEM);

    cr_assert_eq(rc, DIRECT_NOT_ALLOWED,
                 "execute_do_item_item_action returned %d for incorrect allowed actions name in indirect and direct, expected DIRECT_NOT_ALLOWED (2)", rc);
}

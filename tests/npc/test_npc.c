#include <criterion/criterion.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "npc/npc.h"
#include "game-state/item.h"


/* Checks that npc_new() properly mallocs and inits a new npc struct */
Test (npc, new) 
{
    npc_t *npc; 

    npc = npc_new("npc_22");

    cr_assert_not_null(npc, "npc_new() failed");

    cr_assert_eq(strncmp(npc->npc_id, "npc_22", MAX_ID_LEN), 0, 
                 "npc_new didn't set npc_id"); 
}

/* Checks that npc_init() initialized the fields in the new npc struct */
Test (npc, init) 
{ 
    npc_t *npc; 
    int res;

    npc = npc_new("test");
    res = npc_init(npc, "npc_22"); 

    cr_assert_eq(res, SUCCESS, "npc_init() failed"); 

    cr_assert_eq(strncmp(npc->npc_id, "npc_22", MAX_ID_LEN), 0,
                 "npc_22", "npc_init didn't set npc_id");
}

/* Checks that npc_free() frees the given npc struct from memory */
Test (npc, free)
{ 
    npc_t *npc;
    int res; 

    npc = npc_new("npc_22");

    cr_assert_not_null(npc, "npc_new() failed"); 

    res = npc_free(npc); 

    cr_assert_eq(res, SUCCESS, "npc_free() failed"); 
}


/* Checks that get_inventory() returns the npc's inventory */
Test(npc, add_to_and_get_inventory)
{
    npc_t *npc1, *npc2;
    item_t *new_item;
    item_hash_t *hash1, *hash2;
    item_list_t *list1, *list2;

    npc1 = npc_new("npc_1");
    npc2 = npc_new("npc_2");
    new_item = item_new("test_item", "item for npc testing",
                        "item for testing get_npc_inventory()");
    add_item_to_npc(npc2, new_item);

    hash1 = get_npc_inv_hash(npc1);
    hash2 = get_npc_inv_hash(npc2);

    list1 = get_npc_inv_list(npc1);
    list2 = get_npc_inv_list(npc2);

    // Assert everything was made and malloced
    cr_assert_not_null(npc1, "npc_new() failed on npc1");
    cr_assert_not_null(npc2, "npc_new() failed on npc2");
    cr_assert_not_null(new_item, "item_new() failed");

    // Check that inventory with added item is not NULL
    cr_assert_not_null(npc2->inventory,
                       "add_item_to_npc() failed to add item");

    // Check inventory hashes
    cr_assert_eq(hash1, npc1->inventory, "get_npc_inv_hash() failed to "
                 "return NULL for empty inventory");
    cr_assert_eq(hash2, npc2->inventory,
                 "get_npc_inv_hash() failed to return inventory");

    // Check inventory lists
    cr_assert_eq(list1, NULL, "get_npc_inv_list() failed to "
                 "return NULL for empty inventory");
    cr_assert_not_null(list2, "get_npc_inventory() failed to "
                       "return inventory list");

    // Check that added item has correct fields
    cr_assert_eq(strcmp(new_item->item_id, list2->item->item_id), 0,
                 "add_item_to_npc() failed to add item (id is wrong)");
    cr_assert_eq(strcmp(new_item->short_desc, list2->item->short_desc), 0,
                 "add_item_to_npc() failed to add item (short desc is wrong)");
    cr_assert_eq(strcmp(new_item->long_desc, list2->item->long_desc), 0,
                 "add_item_to_npc() failed to add item (long desc is wrong)");
}

/* Checks that add_item_to_npc adds item to the npc struct's inventory 
   by not returning NULL */
Test(npc, add_item_to_npc)
{
    npc_t *npc = npc_new("1");
    item_t *new_item = item_new("test_item", "item for npc testing",
                                "item for testing add_item_to_npc");
    add_item_to_npc(npc, new_item);

    cr_assert_not_null(npc, "npc_new() failed");
    cr_assert_not_null(new_item, "item_new() failed");
    cr_assert_not_null(npc->inventory,
	                   "add_item_to_npc() failed to add item");
}

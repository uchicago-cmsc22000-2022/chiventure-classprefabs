#ifndef BATTLE_STRUCTS_H
#define BATTLE_STRUCTS_H

#include <stdbool.h>
#include <string.h>
#include "playerclass/class_structs.h"


/* items stub */
typedef struct item {
    int id;
    int quantity;
    int durability;
    char* description;
    bool battle;
    int attack;
    int defense;
    int hp;
    struct item *next;
    struct item *prev;
} item_t;

/* moves stub */
typedef struct move {
    item_t *item;
    bool attack;
    int damage;
    int defense;
    struct move *next;
    struct move *prev;
} move_t;

/* stats stub */
typedef struct stat {
    int speed;
    int strength;
    int dexterity;
    int hp;
    int max_hp;
    int xp;
} stat_t;


#endif

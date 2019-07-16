#include "load_wdl.h"

 /*
 * load_wdl, function that loads a wdl into all the game-state structs
 *
 * takes a path to a yaml file, automatically load yaml found in FILE_PATH
 *
 *
 */
game_t *load_wdl(char *path_to_yaml)
{
    obj_t *big_document = get_doc_obj(path_to_yaml);
    game_t *game = create_game(big_document);

     // call functions that parse items, actions, rooms, and game attributes
    // into a game pointer
    int rooms = add_rooms_to_game(big_document, game);
    int connections =  add_connections_to_rooms(big_document, game);
    obj_t *game_document = obj_get_attr(big_document, "GAME.0", false);
    char *start_room = obj_get_str(game_document, "start");
    game_set_start(start_room, game);
    obj_t *end = obj_get_attr(game_document, "end.0", false);
    char *end_room = obj_get_str(end, "in_room");
    room_t *final_room = find_room_from_game(game, end_room);
    game->final_room = final_room;
    int items = load_items(big_document, game);

     if(rooms+connections == 0){
      if(items != -1){
	return game;
      }
    }
    return NULL;
}
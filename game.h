#ifndef GAME_
#define GAME_
#include "maps.h"
#include <ncurses.h>
struct coordonates{
	int x, y;
};

int mark(int **map, int x, int y);
int choose(int **pcMap, int **emptyMap, int ** myMap, TPowers *puteri, int mutari);
int end(int **myMap);
void startGame(int **myMap, int **pcMap, int **emptyMap, TPowers *puteri, int mutari, char **top);
void new_game(char **maps, int noMaps, char **top);
void startMenu(char **maps, int noMaps);
void resume_game(char **top);
#endif
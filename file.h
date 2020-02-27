#ifndef FILE_
#define FILE_
#include "game.h"

void resetTop(char **top);
void addTop(char *name, int mutari, char **top);
void deleteTop(char **top);
void writeTop(char **top);
void showTop(char **top);
void readTop(char **top);
void write_files(int **myMap, int **emptyMap, int **pcMap, TPowers puteri, int mutari);
void delete_files();

#endif
#ifndef POWER_
#define POWER_


void randomize_map(int **myMap, int **emptyMap, int **pcMap, TPowers *puteri);
void destroy_in_advance(int **myMap, int **emptyMap, int **pcMap, TPowers *puteri, int mutari);
int hit_random(int **map, int **originalMap);
void destroy_random_ship(int **myMap, int **emptyMap, int **pcMap, TPowers *puteri, int mutari);

#endif
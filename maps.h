#ifndef MAPS_
#define MAPS_
#define DIM 10

typedef struct powers{
	int dia; //destroy in advance
	int rm;//randomize map
	int drs; //destroy random ship
}TPowers;


int **generateMap();
void print_map(int **map, int i);
void draw_maps(int **firstMap, int **secondMap, TPowers* puteri, int mutari);

#endif
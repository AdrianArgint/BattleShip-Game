#include<ncurses.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include "maps.h"
#include "game.h"
#include "file.h"
#include "power.h"


void randomize_map(int **myMap, int **emptyMap, int **pcMap, TPowers *puteri){

}

void destroy_in_advance(int **myMap, int **emptyMap, int **pcMap, TPowers *puteri, int mutari){
	int i;
	for (i = 0; i < 10; ++i)
	{
		hit_random(emptyMap, pcMap);
		draw_maps(myMap, emptyMap, puteri, mutari);
        sleep(1);
		hit_random(myMap,myMap);
		draw_maps(myMap, emptyMap, puteri, mutari);
        sleep(1);	   
    }
}



int hit_random(int **map, int **originalMap){
	int x = rand() % 10;
    int y = rand() % 10;
    while(originalMap[x][y] > 1){
        x = rand() % 10;
        y = rand() % 10;

    }
    if(mark(originalMap, x, y) == 1){
        map[x][y] = 2;
        originalMap[x][y] = 2;
	    refresh();
	    //sleep(3);
    	return 1;
    }
    else{
        map[x][y] = 3;
        originalMap[x][y] = 3;
		refresh();
		//sleep(3);
        return 0;
    }
}


void destroy_random_ship(int **myMap, int **emptyMap, int **pcMap, TPowers *puteri, int mutari){
	//prima oara o nava a jucatorului
	//se cauta un spatiu ocupat(valoare 1)
	int x = rand() % 10;
    int y = rand() % 10;

    while(myMap[x][y] != 1){
    	x = rand() % 10;
        y = rand() % 10;
    }

    myMap[x][y] = 2;

    int copyX = x;
    int copyY = y;
    while(copyX <= 9 &&  myMap[copyX + 1][copyY] == 1){
    	myMap[copyX + 1][copyY] = 2;
    	copyX++;
    }
    copyX = x;
    copyY = y;
    while(copyY <= 9 && myMap[copyX][copyY + 1] == 1){
    	myMap[copyX][copyY + 1] = 2;
    	copyY++;
    }

    copyX = x;
    copyY = y;

    while(copyX >= 0 && myMap[copyX - 1][copyY] == 1){
    	myMap[copyX - 1][copyY] = 2;
    	copyX--;
    }

    copyX = x;
    copyY = y;
    while(copyY >= 0 && myMap[copyX][copyY - 1] == 1){
    	myMap[copyX][copyY - 1] = 2;
    	copyY--;
	}

	//nava din harta calculatorului
	while(pcMap[x][y] != 1){
    	x = rand() % 10;
        y = rand() % 10;
    }

    pcMap[x][y] = 2;
    emptyMap[copyX][copyY] = 2;

    while(copyX < 9 &&  pcMap[copyX + 1][copyY] == 1){
    	pcMap[copyX + 1][copyY] = 2;
    	emptyMap[copyX + 1][copyY] = 2;
    	copyX++;
    }
    copyX = x;
    copyY = y;
    while(copyY < 9 && pcMap[copyX][copyY + 1] == 1){
    	pcMap[copyX][copyY + 1] = 2;
    	emptyMap[copyX][copyY + 1] = 2;
    	copyY++;
    }

    copyX = x;
    copyY = y;

    while(copyX > 0 && pcMap[copyX - 1][copyY] == 1){
    	pcMap[copyX - 1][copyY] = 2;
    	emptyMap[copyX - 1][copyY] = 2;
    	copyX--;
    }

    copyX = x;
    copyY = y;
    while(copyY > 0 && pcMap[copyX][copyY - 1] == 1){
    	pcMap[copyX][copyY - 1] = 2;
    	emptyMap[copyX][copyY - 1] = 2;
    	copyY--;
	}
	draw_maps(myMap, emptyMap, puteri, mutari);
	refresh();
}
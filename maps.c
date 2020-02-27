
#include "game.h"
#include "maps.h"
#include <time.h>
#include<stdlib.h>
#include<ncurses.h>
#include "file.h"
#include "power.h"

void draw_maps(int **firstMap, int **secondMap, TPowers* puteri, int mutari){
    clear();
    noecho();
    print_map(firstMap, 0);
    printw("\n");

    print_map(secondMap, 40);



    move(41,0);
    printw("Numar de mutari efectuate: %d\n", mutari);
	printw("TASTA	PUTERE			DISPONIBILITATE\n");

	if(puteri->drs)
		printw("  B 	Destroy Random Ship	DA\n");
	else
		printw("  B 	Destroy Random Ship	NU\n");

	if(puteri->rm)
		printw("  N 	Randomize Map		DA\n");
	else
		printw("  N 	Randomize Map		NU\n");

	if(puteri->dia)
		printw("  M 	Destroy in Advance	DA\n");
	else
		printw("  M 	Destroy in Advance	NU\n");


	
	refresh();

}

void print_map(int **map, int pos){
	int i, j;
	for (i = 0; i < DIM; ++i)
	{
		move(i, pos);
		printw("|");
		for (j = 0; j < DIM; ++j)
		{
			if(map[i][j] == 1){
				printw("X|");
			}
			else if(map[i][j] == 2){
				printw("H|");
			}
			else if(map[i][j] == 3){
				printw("O|");
			}
			else{
				printw(" |");
			}
		}
		printw("\n");
	}
}

int **generateMap(){
	srand ( time(NULL) );
	int **map = (int **)malloc(sizeof(int*) * DIM);
	int i, j;
	for (i = 0; i < DIM; ++i)
	{
		map[i] = (int*)calloc(sizeof(int), DIM);
	}

	int noShips = 1;
	int noCells = 4;

	while(noCells > 0){
		for(j = 0 ; j < noShips; j++){
			int ok = 1;
			while(ok){
				ok = 0;
				int direction = rand() % 2; // 0-vertical, 1-orizontal
				
				if(direction == 0){
					int column = rand() % 10; // coloana in care trebuie pozitionata
					int row = rand() % (10 - noCells); // coloana in care trebuie pozitionata
					//verificare disponibilitate
					for(i = 0; i < noCells; i++){
						if(map[row + i][column] == 1){
							ok = 1;
						}
					}
					//verificare dreapta fiind pe primul rand 
					if(column != 9){
						for (i = 0; i < noCells; ++i)
						{
							if(map[row + i][column + 1] == 1){
								ok = 1;
							}
						}
					}

					//verificare stanga fiind pe primul rand 
					if(column != 0){
						for (i = 0; i < noCells; ++i)
						{
							if(map[row + i][column - 1] == 1){
								ok = 1;
							}
						}
					}
					//verificare suplimentara pe diagonala si sus/jos
					//cazul in care suntem pe prima linie
					if(row == 0){
						//verificam sub
						if(map[row + noCells][column]) ok = 1;
						if(column == 0 && map[row + noCells][column + 1]) ok = 1;
						else if(column == 9 && map[row + noCells][column - 1]) ok = 1;
						else{
							if(map[row + noCells][column + 1] || map[row + noCells][column - 1])
								ok = 1;
						}
					}
					else if(row == 10 - noCells){
						//verificam deasupra
						if(map[row - 1][column]) ok = 1;

						if(column == 0 && map[row - 1][column + 1]) ok = 1;
						else if(column == 9 && map[row - 1][column - 1]) ok = 1;
						else{
							if(map[row - 1][column + 1] || map[row - 1][column - 1])
								ok = 1;
						}
					}
					else{
						if(map[row + noCells][column]) ok = 1;
						if(map[row + noCells][column + 1] || map[row + noCells][column - 1]) ok = 1;
						if(map[row - 1][column]) ok = 1;
						if(map[row - 1][column + 1] || map[row - 1][column - 1]) ok = 1;

					}


					if(ok == 0){
						for (i = 0; i < noCells; ++i)
						{
							map[row + i][column] = 1;
						}
					}
				}
				else{ //orizontala
					int column = rand() % (10 - noCells); // coloana in care trebuie pozitionata
					int row = rand() % 10; // coloana in care trebuie pozitionata
					//verificare disponibilitate
					for(i = 0; i < noCells; i++){
						if(map[row][column + i] == 1){
							ok = 1;
						}
					}
					
					if(row == 0){//jos
						for(i = 0; i <noCells; i++){
							if(map[row + 1][column + i]) ok = 1;
						}
					}
					else if(row == 9){ //sus
						for(i = 0; i <noCells; i++){
							if(map[row - 1][column + i]) ok = 1;
						}
					}
					else {//si sus si jos
						for(i = 0; i < noCells; i++){
							if(map[row - 1][column + i] || map[row + 1][column + i]) ok = 1;
						}
					}

					if(column + noCells != 9){//dreapta
						if(map[row][column + noCells]) ok = 1;
					}
					if(column != 0)//stanga
						if(map[row][column - 1]) ok = 1;


					if(column + noCells != 9 && row != 9){
						if(map[row + 1][column + noCells]) ok = 1;
					}

					if(column + noCells != 9 && row != 0){
						if(map[row - 1][column + noCells]) ok = 1;
					}

					if(row != 9 && column != 0)
						if(map[row + 1][column - 1]) ok = 1;

					if(row != 0 && column != 0){
						if(map[row - 1][column - 1]) ok = 1;
 					}

					if(ok == 0){
						for (i = 0; i < noCells; ++i)
						{
							map[row][column + i] = 1;
						}
					}
				}
			}
		}
		noCells--;
		noShips++;
		
	}
	return map;
}

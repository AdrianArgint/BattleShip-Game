#include<ncurses.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include "maps.h"
#include "game.h"
#include "file.h"
#include "power.h"



int main(int argc, char *argv[])
{

	if(argc == 1){
		printf("[Eroare]: Nu s-au dat argumente de comanda.\n");
		return 1;
	}


	int i;
	char **maps;
	maps = (char**)malloc(sizeof(char *) * (argc - 1));
	for (i = 0; i < argc - 1; ++i)
	{	
		maps[i] = (char *) malloc(sizeof(char) * 20);//maxim 20 de caractere
		strcpy(maps[i], argv[i + 1]);
	}

	for (i = 0; i < argc - 1; ++i)
	{
		if(access( maps[i], F_OK ) == -1 ){
			printf("[Eroare]: Fisierul %s nu poate fi deschis.\n", maps[i]);
			for (i = 0; i < argc - 1; ++i)
		    {	
		    	free(maps[i]);
		    }
		    free(maps);
			return 1;
		}
	}
	

	initscr(); // initialize Ncurses
	keypad(stdscr, TRUE);
	startMenu(maps, argc - 1);



    //free
    for (i = 0; i < argc - 1; ++i)
    {
    	free(maps[i]);
    }
    free(maps);
    endwin();
}	
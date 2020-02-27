#include<ncurses.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include "maps.h"
#include "game.h"
#include "file.h"
#include "power.h"

int mark(int **map, int x, int y){
    if(map[x][y] == 1){
        return 1;
    }
    return 0;
}



int choose(int **pcMap, int **emptyMap, int ** myMap, TPowers *puteri, int mutari){
    struct coordonates cursor;
    cursor.y = 0;
    cursor.x = 41;

    move(cursor.y, cursor.x);
    curs_set(1);
    refresh();
    int ch;
    while(1){
        ch = getch();
        switch(ch){
            case KEY_UP:
                if(cursor.y > 0) cursor.y -= 1;
                break;

            case KEY_DOWN:
                if(cursor.y < 9) cursor.y += 1;
                break;

            case KEY_RIGHT:
                if(cursor.x < 40 + 19) cursor.x += 2;
                break;

            case KEY_LEFT:
                if(cursor.x > 40 + 1) cursor.x -= 2;
                break;  

            case 10:
                if(pcMap[cursor.y ][(cursor.x-40) / 2] < 2){
                    // mark(emptyMap,cursor.y , (cursor.x-40) / 2);
                    if(mark(pcMap, cursor.y , (cursor.x-40) / 2) == 0){
                        pcMap[cursor.y ][(cursor.x-40) / 2] = 3;
                        emptyMap[cursor.y][(cursor.x - 40) / 2] = 3;
                        return 1;
                    }
                    else{
                        pcMap[cursor.y ][(cursor.x-40) / 2] = 2;
                        emptyMap[cursor.y][(cursor.x - 40) / 2] = 2;
                        draw_maps(myMap, emptyMap, puteri, mutari);
                    }
                }
                break;
            case 'q':
                return 0;
                break;

            case 'b':
                if(puteri->drs){
                    puteri->drs = 0;
                    destroy_random_ship(myMap, emptyMap, pcMap, puteri, mutari);
                }
                break;
            case 'm':
                if(puteri->dia){
                    puteri->dia = 0;
                    destroy_in_advance(myMap, emptyMap, pcMap, puteri, mutari);
                }
                break;

            case 'n':
                if(puteri->rm){
                    puteri->rm = 0;
                    randomize_map(myMap, emptyMap, pcMap, puteri);
                }
                break;
            default:
                break;
        }
        move(cursor.y, cursor.x);
        refresh();
    }
}


int end(int **myMap){
    int i,j;
    for(i = 0; i < DIM; i++){
        for (j = 0; j < DIM; ++j)
        {
            if(myMap[i][j] == 1)
                return 1;
        }
    }
    return 0;
}




void startGame(int **myMap, int **pcMap, int **emptyMap, TPowers *puteri, int mutari, char **top){
    int turn = 1;
    while(end(myMap) && end(pcMap)){
        draw_maps(myMap, emptyMap, puteri, mutari);
        refresh();
        if(turn == 1){
            if(!choose(pcMap, emptyMap, myMap, puteri, mutari)) break;
            mutari++;
        }
        else{
            sleep(3);
            if(hit_random(myMap, myMap)) 
                turn = !turn; 
        }
        turn = !turn;
    }
    move(25,0);
    if(!end(myMap)){
        printw("A castigat calculatorul!\n");
        refresh();
        sleep(5);
    }
    else if(!end(pcMap)){
        mvprintw(15, 0, "Ai castigat!\n");
        refresh();
        printw("Care este numele tau?\n");
        echo();
        refresh();
        char name[15];
        getstr(name);
        printw("Felicitari, %s!\n", name);
        refresh();
        //sleep(3);
        addTop(name, mutari, top);
    }
    clear();
    refresh();
}


void new_game(char **maps, int noMaps, char **top){
    echo();
    clear();
	int i;
	mvprintw(0, 1, "Which map would you like?(number)");
    refresh();
	for (i = 0; i < noMaps; ++i)
	{
		mvprintw(i + 1, 1, "%d. %s", i + 1, maps[i]);
	}
    refresh();
	mvprintw(noMaps +1 , 1, "I choose map number ");
    refresh();
	char *answer;
    answer = (char *) malloc(sizeof(char) * 100);
	getstr(answer);
	
	int **pcMap = generateMap();

    i = atoi(answer);
    free(answer);
    int j;
    while(i > noMaps){
        mvprintw(noMaps + 2, 1, "Map no %d does not exist.\nChoose another map ", i);
        getstr(answer);
        i = atoi(answer);
    }

    noecho();
    int **myMap = (int **)malloc(sizeof(int*) * DIM);
    FILE *in = fopen(maps[i - 1], "r");
    for (i = 0; i < DIM; ++i)
    {
        myMap[i] = (int*)malloc(sizeof(int) * DIM);
    }

    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            fscanf(in ,"%d", &myMap[i][j]);
        }
    }





    int **emptyMap = (int **)malloc(sizeof(int*) * DIM);
    for (i = 0; i < DIM; ++i)
    {
        emptyMap[i] = (int*)malloc(sizeof(int) * DIM);      
    }

    for ( i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            emptyMap[i][j] = 0;
        }
    }

    TPowers puteri;
    puteri.dia = 1;
    puteri.rm = 1;
    puteri.drs = 1;
    int mutari = 0;;
	startGame(myMap, pcMap, emptyMap, &puteri, mutari, top);

    if(end(myMap) && end(pcMap))
        write_files(myMap, emptyMap, pcMap, puteri, mutari);
}





void resume_game(char **top){
    if( access( "myMap.out", F_OK ) == -1 || access("emptyMap.out", F_OK ) == -1 
        || access("pcMap.out", F_OK) == -1 || access("powers.out", F_OK) == -1){
        printw("Mai intai trebuie sa joci!\n");
        refresh();
        sleep(2);
        return;
    }

    clear();
    refresh();
    int i, j;
    int **myMap = (int **)malloc(sizeof(int*) * DIM);
    FILE *inMyMap = fopen("myMap.out", "r");
    for (i = 0; i < DIM; ++i)
    {
        myMap[i] = (int*)malloc(sizeof(int) * DIM);
    }

    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            fscanf(inMyMap ,"%d", &myMap[i][j]);
        }
    }




    FILE *inEmptyMap = fopen("emptyMap.out", "r");
    int **emptyMap = (int **)malloc(sizeof(int*) * DIM);
    for (i = 0; i < DIM; ++i)
    {
        emptyMap[i] = (int*)malloc(sizeof(int) * DIM);      
    }

    for ( i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            fscanf(inEmptyMap ,"%d", &emptyMap[i][j]);
        }
    }

    FILE *inPcMap = fopen("pcMap.out", "r");
    int **pcMap = (int **)malloc(sizeof(int*) * DIM);
    for (i = 0; i < DIM; ++i)
    {
        pcMap[i] = (int*)malloc(sizeof(int) * DIM);      
    }

    for ( i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            fscanf(inPcMap ,"%d", &pcMap[i][j]);
        }
    }
    fclose(inMyMap);
    fclose(inPcMap);
    fclose(inEmptyMap);


    TPowers puteri;
    int mutari;
    FILE *inPuteri = fopen("powers.out", "r");
    fscanf(inPuteri, "%d %d %d %d", &puteri.dia, &puteri.rm, &puteri.drs, &mutari);



    startGame(myMap, pcMap, emptyMap, &puteri, mutari, top);

    if(end(myMap) && end(pcMap))
        write_files(myMap, emptyMap, pcMap, puteri, mutari);
    clear();
    refresh();

}


void startMenu(char **maps, int noMaps){
   //memoram topul din top.out
    char **top;
    int i;
    top = (char **)malloc(sizeof(char*) * 12);
    for (i = 0; i < 12; ++i)
    {
        top[i] = (char *)malloc(sizeof(char) * 35);
    }

    readTop(top);



    char list[5][15] = { "New Game", "Resume Game", "Top5", "Reset Top5",  "Quit"};
    char item[15];
    int ch;
// afiseaza optiunile si pune highlight pe cea curenta
    for(i = 0; i < 5; i++) {
        if( i == 0 ) 
            attron(A_STANDOUT); // highlight pe prima optiune
        else
            attroff(A_STANDOUT);
        sprintf(item, "%-14s", list[i]);
        mvprintw(i + 1, 2, "%s", item);
    }
 
    refresh();
 
    i = 0;
    noecho();
    curs_set(0);    

    mvprintw(7, 2, "Use arrow keys to go up and down. Press enter to select a choice"); 
    refresh();
    while(1){ 
    	 	ch = getch();
            sprintf(item, "%-14s",  list[i]); 
            mvprintw(i + 1, 2, "%s", item ); 
             switch(ch) {
                case KEY_UP:
                            i--;
                            i = (i < 0) ? 4 : i;
                            break;
                case KEY_DOWN:
                            i++;
                            i = (i > 4) ? 0 : i;
                            break;

            	case 10:
                    if(i == 3){
                        resetTop(top);
                    }
                    else if(i == 2){
                        showTop(top);
                    }
            		else if(i == 4){
                        writeTop(top);
                        deleteTop(top);
                        delete_files();
            			return;
            		}
                    else if(i == 1){
                        resume_game(top);
                    }
            		else if(i == 0){
            			new_game(maps, noMaps, top);
        	        }
                    clear();
                    for(i = 0; i < 5;i++) {
                        if( i == 0 ) 
                            attron(A_STANDOUT); 
                        else
                            attroff(A_STANDOUT);
                        sprintf(item, "%-14s", list[i]);
                        mvprintw(i + 1, 2, "%s", item );
                    }   
                    mvprintw( 7, 2, "Use arrow keys to go up and down. Press enter to select a choice"); 
                    refresh();
            		break;
            }
            attron(A_STANDOUT);
             
            sprintf(item, "%-14s", list[i]);
            mvprintw(i + 1, 2, "%s", item);
            attroff(A_STANDOUT);
    }

}

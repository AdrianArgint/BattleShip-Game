#include<ncurses.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include "maps.h"
#include "game.h"
#include "file.h"


void resetTop(char **top){
    strcpy(top[0], "TOP");
    strcpy(top[1], "5");
    int i;
    for (i = 1; i < 6; ++i)
    {
        strcpy(top[2 * i], "------");
        strcpy(top[2 * i + 1], "999");
    }
}


int findPosition(char **top, int mutari){
    int i, poz = 0;
    for (i = 1; i < 6; ++i)
    {
        if(atoi(top[2 * i + 1]) > mutari)
            return i;
    }
    return 0;
}

void addTop(char *name, int mutari, char **top){
    int poz = findPosition(top, mutari);

    if(!poz){
        printw("Din pacate, nu ai intrat in top\nPentru a vedea topul acceseaza sectiunea TOP 5 din meniu!\n ");
        refresh();
        sleep(2);
        return;
    }

    //mut scorurile mai in jos
    int j;
    for (j = 5; j > poz; --j)
    {
        strcpy(top[2 * j + 1],top[2 * (j - 1) + 1]);
        strcpy(top[2 * j], top[2 * (j - 1)]);
    }

    char moves[15];
    sprintf(moves, "%d", mutari);
    strcpy(top[2 * poz + 1], moves);
    strcpy(top[2 * poz], name);

    printw("Ocupi locul %d in top!\n", poz);
    refresh();
    sleep(3);
}


void deleteTop(char ** top){
    int i;
    for (i = 0; i < 12; ++i)
    {
        free(top[i]);
    }
    free(top);
}

void writeTop(char **top){
    int i;
    FILE *topFile = fopen("top.out", "w");
    for (i = 0; i < 12; ++i)
    {
        fprintf(topFile, "%s ", top[i++]);
        fprintf(topFile, "%s\n", top[i]);
    }
}




void showTop(char **top){
    int i;
    clear();
    move(1,0);
    for (i = 0; i < 12; ++i)
    {
        printw("%s %s\n", top[i++], top[i+1]);
    }
    refresh();
    sleep(5);
}

void readTop(char **top){
    char name[15];
    char score[15];
    int i;
    int j = 0;
    FILE *topFile = fopen("top.out", "r");
    for (i = 0; i < 6; ++i)
    {
        fscanf(topFile, "%s", name);
        fscanf(topFile, "%s", score);
        strcpy(top[j++], name);
        strcpy(top[j++], score);
    }
}


void write_files(int **myMap, int **emptyMap, int **pcMap, TPowers powers, int mutari){
    int i, j;
    FILE *write;
    write = fopen("myMap.out", "w");
    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            fprintf(write, "%d ", myMap[i][j]);
        }
        fprintf(write, "\n");
    }

    fclose(write);


    FILE *randomWrite;
    randomWrite = fopen("emptyMap.out", "w");

    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            fprintf(randomWrite, "%d ", emptyMap[i][j]);
        }
        fprintf(randomWrite, "\n");
    }
    fclose(randomWrite);


    FILE *pcMapWrite;
    pcMapWrite = fopen("pcMap.out", "w");

    for (i = 0; i < DIM; ++i)
    {
        for (j = 0; j < DIM; ++j)
        {
            fprintf(pcMapWrite, "%d ", pcMap[i][j]);
        }
        fprintf(pcMapWrite, "\n");
    }
    fclose(pcMapWrite);

    FILE *powersWrite;
    powersWrite = fopen("powers.out", "w");
    fprintf(powersWrite, "%d %d %d %d", powers.dia, powers.rm, powers.drs, mutari);
    fclose(powersWrite);


    for (i = 0; i < DIM; ++i)
    {
        free(myMap[i]);
        free(emptyMap[i]);
        free(pcMap[i]);
    }
    free(myMap);
    free(emptyMap);
    free(pcMap);

}


void delete_files(){
    remove("myMap.out");
    remove("pcMap.out");
    remove("emptyMap.out");
    remove("powers.out");
}

/*  Author: Kyle Zrinski
    Purpose: Assignment main for calling functions kicking off the program
    Date: 19 - 9 - 18*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lineFuncs.h"
#include "fileIO.h"
#include "macros.h"
#include "LL.h"

int startIO(char fileIn[],char logFile[], LinkedList* list);

int main(int argc, char** argv)
{
    /*Code babeyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy*/
    /*assinging heap memory for variables to be passed through program*/
    char* fileIn = (char*)malloc(LEN * sizeof(char));
    char logFile[] = "graphics.log";

    /*assigning memory to items relevant to drawing lines*/
    coords* currXY = (coords*)malloc(sizeof(coords));
    double* currAngle = (double*)malloc(sizeof(double));
    char* currPat = (char*)malloc(sizeof(char));

    /*creating the read in commands list and the debug log list*/
    LinkedList* inList = createList();
    LinkedList* logList = createList();

    /*assigning default values*/
    currXY->x = 0.0;
    currXY->y = 0.0;
    *currAngle = 0.0;
    *currPat = '+';

    if(argc == 2) /*if correct amount of command line args given*/
    {
        strcpy(fileIn, argv[1]); /*send 1st (excluding program name) argument to fileIn name*/
        if(startIO(fileIn,logFile,inList) == 0) /*kicking off reading file and ensuring no errors occur*/
        {
            /*CONTINUE WITH PROGRAM*/
            /*execute the read in commands and check for errors*/
            if(exCommand(inList,logList,currAngle,currXY,currPat) == 0)
            {
                writeFile(logFile,logList);
            }
            else /*error in executing commands*/
            {
                printf("###Error### Execution error quiting program\n");
            }
        }
        else /*error in reading commands*/
        {
            printf("###Error### Read error quitting program\n");
        }
    }
    else /*invalid command line arguments*/
    {
        perror("###ERROR### Invalid number of arguments");
    }

    /*FREEING MEMORY AND LISTS*/
    free(fileIn);
    fileIn = NULL;

    free(currXY);
    free(currAngle);
    free(currPat);

    currXY = NULL;
    currAngle = NULL;
    currPat = NULL;

    freeList(inList);
    inList = NULL;
    freeList(logList);
    logList = NULL;

    printf("\033[39;49m\n");/*Returning the terminal back to default colours
                           *bc themes are life*/

    return 0;
}

/*function to kick of fileIO and check for errors*/
int startIO(char fileIn[], char logFile[], LinkedList* list)
{
    int error = FALSE;

    DEBUG_FPRINTF(stderr,"%s###DEBUG### Beggining read\n","");
    if(readFile(fileIn,list) == 0) /*if no errors in readFile*/
    {
        /*do nothing*/
    }
    else
    {
        printf("###Error### IO Error\n");
        error = TRUE;
    }

    return error;
}


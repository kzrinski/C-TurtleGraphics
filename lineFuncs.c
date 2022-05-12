/*  Author: Kyle Zrinski
    Purpose: File for functions relating to use of the functions in effects.c 
    and or preparing them for use.
    Date: 6 - 10 - 18*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fileIO.h"
#include "lineFuncs.h"

/*#################################STATIC FUNCTIONS############################*/
static int rotate(double* currAngle, double newAngle);
static int move(LinkedList* logList,double* currAngle,double dist,coords* currXY);
static int draw(LinkedList* logList,double* currAngle,double dist,coords* currXY,char* c,PlotFunc plotter);
static int round(double in);
static double deg2rad(double deg);

/****| exCommand |**************************************************************
 * Purpose: Takes inList of all commands and values validates the values then
 * passes them to the relevant functions with other supporting pointers to be 
 * executed.
 *
 * Imports: LinkedList* inList & logList, double* currAngle, coords* currXY,
 *          char* c
 *          
 * Exports: int errror (return)
 *******************************************************************************/
int exCommand(LinkedList* inList,LinkedList* logList, double* currAngle, coords* currXY, char* c)
{
    PlotFunc plotPtr = plot;
    double newAngle = 0.0;
    double dist = 0.0;
    int FG = 7;
    int BG = 0;
    int temp = 0;
    int error = FALSE;
    dict* comp;
    LLNode* curr = NULL;
    if(inList->head != NULL) /*Checking the list is not empty*/
    {
        curr = inList->head;
        clearScreen(); /*Clears the screen in preperation for printing*/
        do /*WHILE CURRENT NODE NOT NULL AND NO ERRORS HAVE OCCURED*/
        {
            /*Changing forground and background colours prior to running each
             *command*/
            setFgColour(FG);
            setBgColour(BG);

            temp = 0;

            /*Reducing the amount of typecasting for comparisons*/
            comp = (dict*)curr->data;
            if(strcmp(comp->command,"ROTATE") == 0)
            {
                /*Rotate*/
                /* Checking that the value is actually a number (specifically)
                \* of a floating point type.*/
                temp = sscanf(comp->value,"%lf",&newAngle);
                if(temp == 1) /*if only 1 double is read in*/
                {
                    error = rotate(currAngle,newAngle);
                }
                else /*if nothing is read in or something that is not a double/float*/
                {
                    error = TRUE;
                    fprintf(stderr,"###ERROR### Invalid Angle");
                }
            }
            else if(strcmp(comp->command,"MOVE") == 0)
            {
                /*Move*/
                /* Checking that the value is actually a number (specifically)
                \* of a floating point type.*/
                temp = sscanf(comp->value,"%lf",&dist);
                if(temp == 1) /*if only 1 double is read in*/
                {
                    error = move(logList,currAngle,dist,currXY);
                }
                else /*if nothing is read in or something that is not a double/float*/
                {
                    error = TRUE;
                    fprintf(stderr,"###ERROR### Invalid distance");
                }
            }
            else if(strcmp(comp->command,"DRAW") == 0)
            {
                /*Draw*/
                /* Checking that the value is actually a number (specifically)
                \* of a floating point type.*/
                temp = sscanf(comp->value,"%lf",&dist);
                if(temp == 1) /*if only 1 double is read in*/
                {
                    /* To prevent lines being drawn 1 coordinate too many
                    ** distance is reduced by one then at the end of the
                    \* function this is corrected by moving one place*/
                    dist = dist - 1.0;
                    error = draw(logList,currAngle,dist,currXY,c,plotPtr);
                    error = move(logList,currAngle,1.0,currXY);
                }
                else /*if nothing is read in or something that is not a double/float*/
                {
                    error = TRUE;
                    fprintf(stderr,"###ERROR### Invalid distance");
                }
            }
            else if(strcmp(comp->command,"FG") == 0)
            {
                /*Foreground*/
                if(!SMP) /*if simple mode not active*/
                {
                    if(sscanf(comp->value,"%d",&temp) == 1) /*if fg an int*/
                    {    
                        if(temp >= 0 && temp <= 15)/*if FG in valid range*/
                        {
                            FG = temp;
                        }
                        else /*fg not in valid range*/
                        {
                            fprintf(stderr,"###ERROR### Invalid foreground colour");
                            error = TRUE;
                        }
                    }
                    else /*fg not an int*/
                    {
                        fprintf(stderr,"invalid FG argument");
                        error = TRUE;
                    }
                }
            }
            else if(strcmp(comp->command,"BG") == 0)
            {
                /*Background*/
                if(!SMP) /*if simple mode not active*/
                {
                    if(sscanf(comp->value,"%d",&temp) == 1) /*if bg is an int*/
                    {
                        if(temp >= 0 && temp <= 7) /*if BG in valid range*/
                        {
                            BG = temp;
                        }
                        else /*bg not in range*/
                        {
                            fprintf(stderr,"###ERROR### invalid background colour");
                            error = TRUE;
                        }
                    }
                    else /*bg not an int*/
                    {
                        fprintf(stderr,"invalid BG argument");
                        error = TRUE;
                    }
                }
            }
            else if(strcmp(comp->command,"PATTERN") == 0)
            {
                /*Pattern*/
                if(strlen(comp->value) == 1)/*check pattern is a single char*/
                {
                    /*Checking pattern is human readable char*/
                    if(*(comp->value) >= '!' && *(comp->value) <= '~')
                    {
                        *c = *(comp->value);
                    }
                    else /*pattern is not a human readable character*/
                    {
                        fprintf(stderr,"###ERROR### Invalid Pattern %c", *(comp->value));
                        error = TRUE;
                    }
                }
                else /*pattern is a string or empty*/
                {
                    printf("###ERROR### Invalid pattern: %s\n",comp->value);
                    error = TRUE;
                }
            }
            curr = curr->next;/*iterate to next node in list*/
        }
        while(curr != NULL && error == FALSE);
        penDown();
    }
    else /*Linked list empty*/
    {
        DEBUG_FPRINTF(stderr,"%s###Error### In List empty (lineFuncs.c)\n","");
        error = TRUE;
    }

    return error; /*return error code to calling function*/
}

/*PLOTTER FUNCTION*/
void plot(void* plotData)
{
    char *c = (char*)plotData;
    printf("%c",*c);
}

/***| ROTATE |******************************************************************
 * DESCRIPTION: Takes the current angle and the newAngle to be added/subtracted
 * from the original angle and ensures that currAngle is cyclic at 360 degrees.
 * IMPORTS: double* currAngle, double newAngle
 * EXPORTS: int error(return)
 * *****************************************************************************/
static int rotate(double* currAngle, double newAngle)
{
    int error = FALSE;
    if(newAngle != 0.0f) /*if newAngle is valid*/
    {
        *currAngle = *currAngle + newAngle; /*add new angle to current*/
        do /*while currAngle not within 0 and 360 degrees*/
        {
            if(*currAngle > 360.0) /*if greater than 360 take 360 degrees*/
            {
                *currAngle = *currAngle - 360.0;
            }
            else if(*currAngle < 0.0) /*if less than 0 add 360 degrees*/
            {
                *currAngle = *currAngle + 360.0;
            }
            else /*if angle valid after addition*/
            {
                DEBUG_FPRINTF(stderr,"###DEBUG### %3.3f is within 360 degrees\n",*currAngle);
            }
        }
        while((*currAngle > 360.0)||(*currAngle < 0.0));
    }
    else /*angle value invalid*/
    {
        perror("###Error### Angle value invalid");
        error = TRUE;
    }
    return error; /*return error code to calling function*/
}

/***| MOVE |********************************************************************
 * DESCRIPTION: moves the current "cursor". Using the current angle and distance
 * it calculates the end coordinates of a move based on triganometric functions.
 * IMPORTS: LinkedList* logList, double* currAngle, double dist, coords* currXY
 * EXPORTS: int error (return)
 * *****************************************************************************/
static int move(LinkedList* logList,double* currAngle,double dist,coords* currXY)
{
    LLNode* newDBG = createNode(); /*new node to be put into the log file list*/
    char* debug = (char*)malloc(LEN * (sizeof(char))); /*string for newDBG*/
    int error = FALSE;

    coords* start = currXY;
    coords end;
    end.x = 0.0;
    end.y = 0.0;

    if(dist >= 0.f) /*if distance valid*/
    {
        /*end.x = start->x + d*cos(theta) | where theta is in radians*/
        end.x = start->x + (dist)*(cos(deg2rad(*currAngle)));
        /*end.y = start->y - d*cos(theta) | where theta is in radians
         *the reason for start->y - gradient is because traditional coordinates
         *start at 0,0 in the bottom left however for this application they start
         *at the top left and thus y must be inverted.*/
        end.y = start->y - (dist)*(sin(deg2rad(*currAngle)));
        
        /* rounded to avoid negative 0 floats and is fine since the
         * coords will be rounded when actually drawn so no precision is lost
         * in this comparison*/
        if(round(end.x) >= 0 && round(end.y) >= 0)/*if x,y coords not negative*/
        {
            /*log file*/
            sprintf(debug,"MOVE (%3.3f,%3.3f)-(%3.3f,%3.3f)\n",start->x,start->y,end.x,end.y);
           
            if(DBG) /*if debug mode active*/
            {
                fprintf(stderr,"%s",debug); /*print log file info to screen*/
            }

            newDBG->data = debug;
            insertLast(logList,newDBG); /*add log file string to logList*/

            /*update end coordinates*/
            currXY->x = end.x;
            currXY->y = end.y;
        }
        else /*if x,y coords are negative*/
        {
            error = TRUE;
            perror("###Error### Invalid move to negative space");

            /*free memory that was unused*/
            free(debug);
            free(newDBG);
            debug = NULL;
            newDBG = NULL;
        }
    }
    else /*distance invalid*/
    {
        /*errror*/
        error = TRUE;
        perror("###Error### Invalid distance");

        /*free memory that was unused*/
        free(debug);
        free(newDBG);
        debug = NULL;
        newDBG = NULL;
    }

    return error;
 
}

/***| DRAW |********************************************************************
 * DESCRIPTION: Conceptually similar to move except it passes the start and end
 * coordinates to the line function to draw the line to screen using the 
 * plotter function.
 * IMPORTS: LinkedList* logList, double* currAngle, double dist, coords* currXY,
 * char* patt, PlotFunc.
 * EXPORTS: int error (return)
 * *****************************************************************************/
static int draw(LinkedList* logList,double* currAngle,double dist,coords* currXY,char* patt,PlotFunc plotter)
{
    LLNode* newDBG = createNode(); /*new node to be put into log file list*/
    char* debug = (char*)malloc(LEN * (sizeof(char))); /*string for newDBG*/
    int error = FALSE;

    coords* start = currXY;
    coords end;
    end.x = 0.0;
    end.y = 0.0;

    if(dist >= 0.f) /*if dist valid*/
    {
        /*starting x and y are rounded to improve the visuals of certain angled
         *shapes as the retained float precision caused certain coordinates to
         *be off resulting in a lines at 45 degrees not printing straight*/
         /*SEE MOVE FOR DETAILED EXPLENATION OF HOW END.X AND END.Y ARE CALCULATED*/
        end.x = round(start->x) + (dist)*(cos(deg2rad(*currAngle)));
        end.y = round(start->y) - (dist)*(sin(deg2rad(*currAngle)));
        
        /* rounded to avoid negative 0 floats and is fine since the
         * coords will be rounded when actually drawn so no precision is lost
         * in this comparison*/
        if(round(end.x) >= 0 && round(end.y) >= 0) /*if coords not negative*/
        {
            /*calls line function to print line to screen using plotter*/
            line(round(start->x),round(start->y),round(end.x),round(end.y),plotter,patt);
            /*formatting debug string with data*/
            sprintf(debug,"DRAW (%3.3f,%3.3f)-(%3.3f,%3.3f)\n",start->x,start->y,end.x,end.y);
          
            if(DBG) /*if debug mode active*/
            {
                fprintf(stderr,"%s",debug); /*print debug info to stderr*/
            }

            newDBG->data = debug;
            insertLast(logList,newDBG);

            /* updating coordinates, rounding for the same reason the starting
             * coords are rounded (line 321 for explenation)*/
            currXY->x = round(end.x);
            currXY->y = round(end.y);
        }
        else /* coords are negative*/
        {
            error = TRUE;
            perror("###Error### Invalid draw to negative space");

            /*free unused memory*/
            free(newDBG);
            free(debug);
            debug = NULL;
            newDBG = NULL;
        }
    }
    else /* dist invalid*/
    {
        /*errror*/
        error = TRUE;
        perror("###Error### invalid distance");

        /*free unused memory*/
        free(newDBG);
        free(debug);
        debug = NULL;
        newDBG = NULL;
    }

    return error; /*return error code to calling function*/
 
}

/********************************STATIC FUNCS**********************************/

/*function for converting between degrees and radians*/
static double deg2rad(double deg)
{
    return (deg * M_PI)/180.0;
}

/*function for truly rounding doubles to ints instead of truncation*/
static int round(double in)
{
    int ret;
    double comp = floor(in); 
    
    if((in - comp) >= 0.5f)
    {
        ret = (int)ceil(in); /*round up*/
    }
    else if((in - comp) < 0.5f)
    {
        ret = (int)floor(in); /*round down*/
    }
    else
    {
        perror("###ERROR### rounding error (this should never happen chief)");
    }

    return ret;
}



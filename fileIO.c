/*******************************************************************************  
 *  Author: Kyle Zrinski
 *  Purpose: To handle fileIO functions of the program
 *  Date: 19 - 9 - 18
 *******************************************************************************/

#include "fileIO.h"

/*###########STATIC FUNCTION DECLARATIONS###########################*/
static int readLine(FILE* file, LinkedList* inList);
static int split(char* line, LinkedList* inList);
static void strCase(char str[]);
static int cmdCheck(char cmd[]);
/*##################################################################*/

/*## NOTE string arrays and character arrays used are place holders *
 *## linked lists                                                   */

/*Takes in a file name and a LinkedList which will be filled with *
 *the tokenized strings retrieved from the file operations.         */
int readFile(char* fName, LinkedList* inList)
{
    FILE* file;
    int ch;
    int error = FALSE;
    file = fopen(fName,"r"); /*Opening file in read mode*/
    if(file != NULL) /* if file exists*/
    {
        /*FILE EXISTS FULL STEAM AHEAD*/
        ch = fgetc(file); /*check first character is not EOF aka empty file*/
        if(ch != EOF)/*ch is not EOF, split and read new lines*/
        {
            rewind(file); /*return file pointer to file start for readLine*/
            /*readLine handles tokeniser within*/
            if(readLine(file, inList) != 0) /*if error in readLine*/
            {
                error = TRUE;
                printf("###Error### Format Error in %s\n",fName);
            }
        }
        else /*file exists but is empty*/
        {
            error = TRUE;
            printf("###Error### %s exists however is empty", fName);
        }

        fclose(file); /*closing file once done*/
    }
    else
    {
        error = TRUE;
        perror("###Error### Error opening file"); /*Prints to stderr specific file error*/
    }

    return error; /*Return whether an error occured*/
}

/*writeFile takes in a file and a linked list with string "values" these string
 *values are the lines that will need to be printed to the log file and terminal
 *when debug is active.*/
int writeFile(char* fName, LinkedList* inList)
{   
    FILE* file;
    int error = FALSE; 
    LLNode* curr = NULL;
    file = fopen(fName,"a"); /*open file in append mode*/

    if(file != NULL)
    {
        fputs("---\n",file);

        if(inList->head != NULL) /*Making sure list has at least one element*/
        {
            curr = inList->head;
            do
            {
                fputs((char*)curr->data,file);
                fflush(stdout); /*Ensuring the line is printed to file each time*/
                curr = curr->next; 
            }
            while(curr != NULL);
        }
        else /*list has no elements*/
        {
            printf("###Error### Writing Error: List Empty\n");
            error = TRUE;
        }

        if(ferror(file)) /*Checking for file errors to print*/
        {
            perror("###Error### Error writing to file: ");
            fclose(file); /*Close file after error*/
            error = TRUE;
        }
        else /*Program succeeded "do nothing"*/
        {
            DEBUG_FPRINTF(stderr,"###DEBUG### Writing Success%s\n","");
        }

        fclose(file); /*close file after done writing*/
    }
    else
    {
        perror("Error opening log file: ");
    }

    return error; /*returns error code to calling function*/
}

/*#############################################################################*
 *#############################STATIC FUNCTIONS################################*
 *#############################################################################*/

static int readLine(FILE* file, LinkedList* inList)
{
    char* line = malloc(LEN * sizeof(char));
    int done = FALSE;
    int count = 1; /*Counter for debugging to find problem lines*/
    int error = FALSE;
    do
    {
        /*fgets() takes the line which is being pointed at by the file pointer *
         *as such it will still "get" a line when used inside a control        *
         *structure as below hence the strange nature of the if statement      */
        if(fgets(line,LEN,file) != NULL)
        {
            if(split(line, inList) == 0) /*If split runs without error*/
            {
                count++;
            }
            else /*print the line number where error occured and break the loop*/
            {
                printf("###Error### Format error at line: %d\n",count);
                done = TRUE;
                error = TRUE;
            }
        }
        else /*print the line where the program finished or error occurs*/
        {
            DEBUG_FPRINTF(stderr,"###DEBUG### Finished/Error at line: %d\n",count);
            done = TRUE;
        }
    }
    while(!done); /*loops through all lines in the file until null is reached*/

    if(ferror(file)) /*if there is an error in the file functions*/
    {
        perror("###Error### Error reading file\n");
        fclose(file);
    }
    else if(error != TRUE) /*if there isn't an error*/
    {
        DEBUG_FPRINTF(stderr,"###DEBUG### File Read Success%s\n","");
    }
    free(line);
    line = NULL;

    return error; /*return error code to calling function
    */
}

static int split(char* line, LinkedList* inList)
{
    char str1[LEN], str2[LEN],str3[LEN]; /*temporary strings for sscanf*/
    int numRead;
    int exit = FALSE;

    LLNode* newNode = createNode();
    dict* pair = (dict*)malloc(sizeof(dict));

    /* scanf's line for strings the third string being there to check that 2 and
     * only 2 arguments are provided simply catching extra ones and going straight
     * to an error.*/
    numRead = sscanf(line,"%s %s %s\n",str1,str2,str3);
    if(numRead == 2) /*if only 2 parameters are read by sscanf*/
    {
        strCase(str1); /*Ensuring the command is case insensitive*/

        if(cmdCheck(str1) == 1) /*checking the first argument is a valid command*/
        {
            strcpy(pair->command,str1);
            strcpy(pair->value,str2);
        
            newNode->data = pair;
            insertLast(inList,newNode); /*inserting last in the list so data
            can be retrieved the right way out*/
        }
        else
        {
            printf("###Error#### Invalid command/format\n");
            /*Freeing memory that isn't filled and cant be free'd by free
             *list or anywhere else*/
            free(pair);
            free(newNode);
            pair = NULL;
            newNode = NULL;

            exit = TRUE;
        }

    }
    else /*if incorrect number of parameters are read by sscanf*/
    {
        printf("###Error### Incorrect number of parameters\n");
        exit = TRUE;
        free(newNode);
        free(pair);

        newNode = NULL;
        pair = NULL;
    }

    return exit;
}

/* String case converter originally from UCP Prac 4 Written 5/9/18 */
static void strCase(char str[])
{
    int ii = 0;
    int caseDiff = 32; /*Numerical difference between uppercase and lower case*/
    char curr = *str;

    /*The loop below's statement allows the function to run without requiring
     *the length of the string to be passed instead looking for the string
     *null terminator to end the loop*/
    while(curr != '\0')
    {   
        /*Modifying the string's case directly without the use of array index 
         *notation. Instead dereferencing the string pointer to access characters
         *from the string and modiyfing them by caseDiff*/
        if(*(str + ii) >= 'a' && *(str + ii) <= 'z')
        {
            *(str + ii) = *(str+ii) - caseDiff;
        }
        ii++;
        curr = *(str + ii);
    }
}

/*checks that the input string matches at least one of the allowed strings and
 *returns a boolean true/false if it does/nt*/
static int cmdCheck(char cmd[])
{
    int ii;

    int valid = FALSE;
    char *list[] = {"ROTATE","MOVE","DRAW","FG","BG","PATTERN"};

    for(ii = 0; ii < 6; ii++)
    {
        if(strcmp(cmd,list[ii]) == 0)
        {
            valid = TRUE;
        }
    }

    if(valid) /*debug prints to inform the user whether commands are valid in debug mode*/
    {
        DEBUG_FPRINTF(stderr,"###DEBUG### %s does match valid commands\n",cmd);
    }
    else
    {
        DEBUG_FPRINTF(stderr,"###ERROR### %s does not match valid commands\n",cmd);
    }
 

    return valid;
}



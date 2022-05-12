/*Creates a print statement that only prints when the DEBUG flag is active.
 * Otherwise it removes the line, there is a version for print statements 
 * with and without format options*/
#ifdef DEBUG
# define DEBUG_FPRINT(frmt,prnt) printf(frmt,prnt) /*FPRINT = format print*/
# define DEBUG_PRINT(prnt) printf(prnt)
# define DEBUG_FPRINTF(file,frmt,prnt) fprintf(file,frmt,prnt)
# define DBG 1
#else
# define DEBUG_FPRINT(frmt,prnt)
# define DEBUG_PRINT(prnt)
# define DEBUG_FPRINTF(file,frmt,prnt)
# define DBG 0
#endif

/*defining SMP flag to perform tasks when SIMPLE has been defined by make*/
#ifdef SIMPLE
# define SMP 1
#else
# define SMP 0
#endif

/*Creating boolean operators for C*/
#define TRUE 1
#define FALSE 0

/*defining max array size */
#define LEN 50

/*Creating key-value pair data type called dictionary (called as such bc i 
 *remember it from python "\-(o_O)-/") */
#ifndef dictionary
# define dictionary
typedef struct{
    char command[LEN];
    char value[LEN];
}dict;
#endif

/*Struct to hold x,y values together instead of seperately*/
#ifndef coordinates
# define coordinates
typedef struct {
    double x;
    double y;
}coords;
#endif

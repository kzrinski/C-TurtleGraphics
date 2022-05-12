#include "macros.h"
#include "LL.h"
#include "effects.h"
#define M_PI 3.14159265358979323846

#ifdef SIMPLE
# define SMP 1
#else
# define SMP 0
#endif

int exCommand(LinkedList* inList,LinkedList* logList, double* currAngle, coords* currXY, char* c);
void plot(void* plotData);

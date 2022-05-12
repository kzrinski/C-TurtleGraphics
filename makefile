CFLAGS = -Wall -Werror -pedantic -ansi
CC = gcc
OBJ = main.o effects.o fileIO.o LL.o lineFuncs.o
EXEC = TurtleGraphics 
DBG = main.c fileIO.c LL.c lineFuncs.c
SMP = lineFuncs.c

$(EXEC) : $(OBJ)
		$(CC) $(OBJ) -o $(EXEC) -lm
main.o : main.c effects.c fileIO.c lineFuncs.c macros.h
		$(CC) -c main.c $(CFLAGS)
effects.o : effects.c effects.h
		$(CC) -c effects.c $(CFLAGS)
fileIO.o : fileIO.c fileIO.h macros.h
		$(CC) -c fileIO.c $(CFLAGS)
LL.o : LL.c LL.h macros.h
		$(CC) -c LL.c $(CFLAGS)
lineFuncs.o : lineFuncs.c lineFuncs.h macros.h
		$(CC) -c lineFuncs.c $(CFLAGS)
clean :
		rm -f $(EXEC) $(OBJ) TurtleGraphicsDebug TurtleGraphicsSimple
debug : $(OBJ)
		$(CC) -g -c $(DBG) $(CFLAGS) -D DEBUG=1
		$(CC) $(OBJ) -o TurtleGraphicsDebug -lm
simple : $(OBJ)
		$(CC) -c $(SMP) $(CFLAGS) -D SIMPLE=1
		$(CC) $(OBJ) -o TurtleGraphicsSimple -lm


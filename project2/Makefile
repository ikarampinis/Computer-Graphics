all: ask6

ask6: ask6.o
	gcc -Wall -g ask6.c -o ask6 -lGL -lGLU -lglut -lm -lncurses

ask6.o: ask6.c
	gcc -Wall -g -c ask6.c

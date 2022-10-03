all: ask1 ask2 ask4

ask1: ask1.o
	gcc -Wall -g ask1.c -o ask1 -lGL -lGLU -lglut -lm

ask2: ask2.o
	gcc -Wall -g ask2.c -o ask2 -lGL -lGLU -lglut -lm

ask4: ask4.o
	gcc -Wall -g ask4.c -o ask4 -lGL -lGLU -lglut -lm

ask1.o: ask1.c
	gcc -Wall -g -c ask1.c

ask2.o: ask2.c
	gcc -Wall -g -c ask2.c

ask4.o: ask4.c
	gcc -Wall -g -c ask4.c

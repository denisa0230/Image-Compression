build: quadtree

quadtree: quadtree.o functions.o
	gcc -g -Wall -std=c9x quadtree.o functions.o -o quadtree

quadtree.o: quadtree.c
	gcc -g -Wall -std=c9x -c quadtree.c

functions.o: functions.c
	gcc -g -Wall -std=c9x -c functions.c

clean:
	rm *.o quadtree
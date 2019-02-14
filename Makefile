build: main.o compress.o decompress.o mirror.o overlay.o auxiliary.o
	gcc -o quadtree $^ -g -lm

main.o: main.c compress.h decompress.h mirror.h overlay.h
	gcc -c main.c

compress.o: compress.c auxiliary.h
	gcc -c compress.c

decompress.o: decompress.c auxiliary.h
	gcc -c decompress.c

mirror.o: mirror.c auxiliary.h
	gcc -c mirror.c

overlay.o: overlay.c auxiliary.h
	gcc -c overlay.c

auxiliary.o: auxiliary.c auxiliary.h
	gcc -c auxiliary.c

clean:
	rm -f *.o quadtree 
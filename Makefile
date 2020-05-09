all: clear parseCsv.o main.o
	gcc -g parseCsv.o main.o -o main -Wall -lm

parseCsv.o:
	gcc -g -c parseCsv.c -o parseCsv.o

main.o:
	gcc -g -c main.c -o main.o

clear:
	rm -f *.o

run:
	./main

debug: clear parseCsv.o main.o
	gcc -g parseCsv.o main.o -o main -Wall -lm -fsanitize=address
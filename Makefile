all: clear dir register binarioNaTela main
	gcc -g build/register.o build/binarioNaTela.o build/main.o -o main -Wall -lm

dir:
	mkdir build/

binarioNaTela:
	gcc -g -c src/binarioNaTela.c -o build/binarioNaTela.o

register:
	gcc -g -c src/register.c -o build/register.o

main:
	gcc -g -c src/main.c -o build/main.o

clear:
	rm -rf build/

run:
	./main

debug: clear register binarioNaTela main
	gcc -g build/register.o build/binarioNaTela.o build/main.o -o main -Wall -lm -fsanitize=address
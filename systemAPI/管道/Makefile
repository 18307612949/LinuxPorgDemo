.PHONY:clean 
main:main.o
	g++ -Wall -g -pthread main.o -o main
main.o:main.c
	g++ -Wall -c main.c -o main.o
clean:
	rm -f main main.o

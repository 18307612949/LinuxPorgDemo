.PHONY:clean 
poll:poll.o
	gcc -Wall -g -pthread poll.o -o poll
poll.o:poll.c
	gcc -Wall -c poll.c -o poll.o
clean:
	rm -f poll poll.o

.PHONY:clean 
epoll:epoll.o
	g++ -Wall -g -pthread epoll.o -o epoll
epoll.o:epoll.c
	g++ -Wall -c epoll.c -o epoll.o
clean:
	rm -f epoll epoll.o

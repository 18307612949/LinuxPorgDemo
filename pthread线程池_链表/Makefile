.PHONY:clean 
thread8:thread8.o
	g++ -Wall -g -pthread thread8.o -o thread8
thread8.o:thread8.c
	g++ -Wall -c thread8.c -o thread8.o
clean:
	rm -f thread8 thread8.o

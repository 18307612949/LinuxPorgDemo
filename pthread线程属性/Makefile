.PHONY:clean 
thread4:thread4.o
	g++ -Wall -g -pthread thread4.o -o thread4
thread4.o:thread4.c
	g++ -Wall -c thread4.c -o thread4.o
clean:
	rm -f thread4 thread4.o

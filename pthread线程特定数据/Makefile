.PHONY:clean 
thread5:thread5.o
	g++ -Wall -g -pthread thread5.o -o thread5
thread5.o:thread5.c
	g++ -Wall -c thread5.c -o thread5.o
clean:
	rm -f thread5 thread5.o

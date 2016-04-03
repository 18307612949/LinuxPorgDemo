.PHONY:clean 
thread3:thread3.o
	g++ -Wall -g -pthread thread3.o -o thread3
thread3.o:thread3.c
	g++ -Wall -c thread3.c -o thread3.o
clean:
	rm -f thread3 thread3.o

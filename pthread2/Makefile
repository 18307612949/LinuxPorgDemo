.PHONY:clean 
thread2:thread2.o
	g++ -Wall -g -pthread thread2.o -o thread2
thread2.o:thread2.c
	g++ -Wall -c thread2.c -o thread2.o
clean:
	rm -f thread2 thread2.o

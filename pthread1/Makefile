.PHONY:clean 
thread1:thread1.o
	g++ -Wall -g -pthread thread1.o -o thread1
thread1.o:thread1.c
	g++ -Wall -c thread1.c -o thread1.o
clean:
	rm -f thread1 thread1.o

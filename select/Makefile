.PHONY:clean 
all:main client
main:test.o
	gcc -Wall -g -pthread test.o -o main
test.o:test.c
	gcc -Wall -c test.c -o test.o
client:client.o
	gcc -Wall -g -pthread client.o -o client
client.o:client.c
	gcc -Wall -c client.c -o client.o
clean:
	rm -f main test.o client client.o

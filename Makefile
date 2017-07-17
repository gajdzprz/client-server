CC = gcc

all: server.o client.o
	$(CC) server.o -L ../jsmn/ -l:libjsmn.a -o server
	$(CC) client.o -L ../jsmn/ -l:libjsmn.a -o client

client: client.o ../jsmn/jsmn.h
	$(CC) client.c -c -o client.o

server: server.o ../jsmn/jsmn.h
	$(CC) server.c -c -o server.o

clean:
	rm -f *.o server client

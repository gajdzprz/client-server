CC = gcc

all: libjsmn.a server.o client.o
	$(CC) server.o -L jsmn/ -l:libjsmn.a -o server
	$(CC) client.o -L jsmn/ -l:libjsmn.a -o client
	mkdir files_download

libjsmn.a: jsmn/jsmn.o
	$(AR) rc jsmn/libjsmn.a jsmn/jsmn.o

jsmn.o: jsmn/jsmn.c jsmn/jsmn.h
	$(CC) jsmn/jsmn.c -c -o jsmn/jsmn.o

client.o: client.c jsmn/jsmn.h
	$(CC) client.c -c -o client.o

server.o: server.c jsmn/jsmn.h
	$(CC) server.c -c -o server.o

clean:
	rm -f *.o server client
	rm -f jsmn/jsmn.o jsmn/libjsmn.a
	rm -rf files_download/

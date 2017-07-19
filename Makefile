CC = gcc

all: libjsmn.a parser.o logging.o helper.o menu.o server.o client.o
	$(CC) server.o parser.o logging.o helper.o menu.o -L jsmn/ -l:libjsmn.a -o server
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

parser.o: parser.c parser.h
	$(CC) parser.c -c -o parser.o

logging.o: logging.c logging.h
	$(CC) logging.c -c -o logging.o

helper.o: helper.c helper.h
	$(CC) helper.c -c -o helper.o

menu.o: menu.c menu.h
	$(CC) menu.c -c -o menu.o

clean:
	rm -f *.o server client
	rm -f jsmn/jsmn.o jsmn/libjsmn.a
	rm -rf files_download/

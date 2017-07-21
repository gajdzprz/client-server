CC = gcc

all: server client
	mkdir files_download

server: server.o parser.o logging.o helper.o menu.o libjsmn.a
	$(CC) server.o parser.o logging.o helper.o menu.o -L jsmn/ -ljsmn -o server

client: test_client.o parser.o helper.o libjsmn.a
	$(CC) test_client.o parser.o helper.o -L jsmn/ -ljsmn -o test_client

libjsmn.a: jsmn/jsmn.o
	$(AR) rc jsmn/libjsmn.a jsmn/jsmn.o

%.o: %.c
	$(CC) $< -c -o $@

clean:
	rm -f *.o server test_client
	rm -f jsmn/jsmn.o jsmn/libjsmn.a
	rm -rf files_download/

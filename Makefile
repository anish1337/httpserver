CC=clang
CFLAGS=-std=c99 -Wall -Werror

server: server.o
	$(CC) $(CFLAGS) -o server server.o
server.o: server.c
	$(CC) $(CFLAGS) -c server.c
debug: server.c
	$(CC) $(CFLAGS) -g -ggdb -c server.c
	$(CC) $(CFLAGS) -g -ggdb -o server server.o
clean:
	rm -f *~
	rm -f *#
	rm -f ./server
	rm -f *.o

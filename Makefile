CC=gcc
CFLAGS=-I.

client: client.c requests.c helpers.c parson.c
		$(CC) -w -o client client.c requests.c helpers.c parson.c
		
run: client
		./client

clean:
		rm -f *.o client
CFLAGS=-Wall -g

shell: shell.o
	gcc $(CFLAGS) shell.o -o shell

shell.o: shell.c
	gcc $(CFLAGS) -c shell.c

clean:
	rm shell *.o

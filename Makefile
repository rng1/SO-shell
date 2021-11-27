shell: p2.o mem_list.o
	gcc p2.o mem_list.o -o shell

p2.o: p2.c mem_list.h
	gcc -c p2.c

mem_list.o: mem_list.c mem_list.h types.h
	gcc -c mem_list.c
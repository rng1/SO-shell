OBJS	= main.o mem_list.o job_list.o p0.o p1.o p2.o p3.o
SOURCE	= main.c mem_list.c job_list.c p0.c p1.c p2.c p3.c
HEADER	= main.h mem_list.h job_list.h p0.h p1.h p2.h p3.h types.h color.h
FLAGS	= -c -Wall

all: $(OBJS)
	gcc $(OBJS) -o shell

main.o: main.c
	gcc $(FLAGS) main.c

mem_list.o: mem_list.c
	gcc $(FLAGS) mem_list.c

job_list.o: job_list.c
	gcc $(FLAGS) job_list.c

p0.o: p0.c
	gcc $(FLAGS) p0.c

p1.o: p1.c
	gcc $(FLAGS) p1.c

p2.o: p2.c
	gcc $(FLAGS) p2.c

p3.o: p3.c
	gcc $(FLAGS) p3.c


clean:
	rm -f $(OBJS) shell

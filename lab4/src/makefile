CC=gcc
CFLAGS=-I.


# EXECUTABLES
parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

zombies:
	$(CC) zombies.c -o zombies

memory :
	$(CC) process_memory.c -o memory

parsum : utils.o libarray_sum.so
	$(CC) parallel_sum.c utils.o array_sum.h -L. -l=pthread -l=array_sum -o parsum

# UTILS & LIBS
utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

libarray_sum.so :
	$(CC) --shared array_sum.c -o libarray_sum.so

# CLEAN
clean :
	rm -f *.o *.so parallel_min_max zombies memory parsum
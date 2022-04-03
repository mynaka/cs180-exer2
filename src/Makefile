CC = gcc
CFLAGS = -0

default: naive

naive:					#run naive program
	$(CC) -c naive.c
	$(CC) -o run naive.o
	./run
	$(RM) naive.o run

cells:					#run threading by cells program
	$(CC) -pthread -c threaded_cells.c
	$(CC) -pthread -o run threaded_cells.o
	./run
	$(RM) threaded_cells.o run

rows:					#run threading by rows naive program
	$(CC) -pthread -c threaded_rows.c
	$(CC) -pthread -o run threaded_rows.o
	./run
	$(RM) threaded_rows.o run

columns:					#run threading by column program
	$(CC) -pthread -c threaded_cols.c
	$(CC) -pthread -o run threaded_cols.o
	./run
	$(RM) threaded_cols.o run

rows_cached:					#run threading by rows(cached) program
	$(CC) -pthread -c threaded_rows_cached.c
	$(CC) -pthread -o run threaded_rows_cached.o
	./run
	$(RM) threaded_rows_cached.o run

own:					#run threading by rows(cached) program
	$(CC) -pthread -c threaded_own.c
	$(CC) -pthread -o run threaded_own.o
	./run
	$(RM) threaded_own.o run
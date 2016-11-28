CC = gcc
PROGRAMS = main Database Tuples
CFLAGS = -g

all: proj4

proj4: main.o Database.o Tuples.o
	$(CC) $(CFLAGS) -o proj4 main.o Database.o Tuples.o
	
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
	
Database.o: Database.c Database.h
	$(CC) $(CFLAGS) -c Database.c
	
Tuples.o: Tuples.c Tuples.h
	$(CC) $(CFLAGS) -c Tuples.c
	

clean:
	-rm $(PROGRAMS) *.o
	-rm -r *.dSYM
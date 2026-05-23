CC = gcc
CFLAGS = -Wall -Wextra

all: tarsau

tarsau: main.o archive.o extract.o utils.o
	$(CC) $(CFLAGS) -o tarsau main.o archive.o extract.o utils.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

archive.o: archive.c archive.h
	$(CC) $(CFLAGS) -c archive.c

extract.o: extract.c extract.h
	$(CC) $(CFLAGS) -c extract.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o tarsau
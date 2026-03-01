CC = gcc
CFLAGS = -Wall -Wextra

OBJ = main.o songs.o album.o playlist.o commands.o

all: cunplugged

cunplugged: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o cunplugged

main.o: main.c songs.h album.h playlist.h commands.h
	$(CC) $(CFLAGS) -c main.c

songs.o: songs.c songs.h
	$(CC) $(CFLAGS) -c songs.c

album.o: album.c album.h songs.h playlist.h
	$(CC) $(CFLAGS) -c album.c

playlist.o: playlist.c playlist.h songs.h album.h
	$(CC) $(CFLAGS) -c playlist.c

commands.o: commands.c commands.h
	$(CC) $(CFLAGS) -c commands.c

clean:
	rm -f *.o cunplugged

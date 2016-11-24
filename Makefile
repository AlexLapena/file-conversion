CC = gcc

all: liblistio.a

# Compiles Library
liblistio.a: listio.c
	$(CC) listio.c -g -o listio.o -c
	$(CC) db.c -g -o db.o -c
	ar cr liblistio.a listio.o
	$(CC) db.o -o db -lmysqlclient -L.
	$(CC) main.c -g -llistio -lmysqlclient -L. -o a4
	
lib: libTest.c
	$(CC) libTest.c -g -llistio -L. -o start
	
db: db.o
	$(CC) db.o -o db -lmysqlclient -L.
	
db.o: db.c
	$(CC) db.c -o db.o -c

clean:
	rm -rf *.o a4 db *.bin liblistio.a files/*.html start

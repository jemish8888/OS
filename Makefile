CC=gcc
CFLAGS=-Wall -ggdb

driver: driver.o loglib.o
	$(CC) $(CFLAGS) driver.o loglib.o -o driver

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

loglib.o: loglib.c loglib.h
	$(CC) $(CFLAGS) -c loglib.c

clean:
	rm -f driver driver.o loglib.o memcheck.log messages.log

memcheck: driver
	valgrind -v --tool=memcheck --leak-check=full \
				--show-reachable=yes \
				--track-origins=yes  \
				--log-file=memcheck.log ./driver

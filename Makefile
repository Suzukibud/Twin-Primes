
CC = g++
CFLAGS = -std=c++11 -pthread

%.0: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS) -fopenmp

ALL: main.o 
	$(CC) -o main  main.o -fopenmp $(CFLAGS) $(DEBUGFLAGS)

DOXY:
	doxygen Doxyfile

DEBUG: DEBUGFLAGS = -DDEBUG
DEBUG: ALL

CLEAN:
	rm *.o

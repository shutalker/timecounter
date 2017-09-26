CC=clang++
CFLAGS=-c -std=c++11 -Wall

all: timecounter

timecounter: timecounter.o
	$(CC) timecounter.o -o timecounter

timecounter.o: timecounter.cpp
	$(CC) $(CFLAGS) timecounter.cpp

clean:
	rm -rf *.o timecounter

CC=clang++
CFLAGS=-c -std=c++11 -Wall

all: timecounter

timecounter: main.o timecounter.o
	$(CC) main.o timecounter.o -o timecounter

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

timecounter.o: timecounter.cpp
	$(CC) $(CFLAGS) timecounter.cpp

clean:
	rm -rf *.o timecounter

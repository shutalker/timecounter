CC		 = clang++
CFLAGS	 = -c -std=c++11 -Wall
BUILDDIR = build
TARGET	 = timecounter
MV       = mv *.o $(TARGET) $(BUILDDIR)
.PHONY	 : all makebuild clean

all: makebuild

makebuild: $(TARGET)
	install -d $(BUILDDIR) \
	&& $(MV)

$(TARGET): timecounter.o main.o
	$(CC) timecounter.o main.o -o $(TARGET)

main.o: main.cpp
	$(CC) $(CFLAGS) $<

timecounter.o: timecounter.cpp
	$(CC) $(CFLAGS) $<

clean:
	rm -rf $(BUILDDIR)

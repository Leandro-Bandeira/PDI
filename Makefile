CC = g++

all:
	$(CC) -O3  *.cpp `pkg-config --cflags --libs opencv4` -o main

clean:
	rm cgprog

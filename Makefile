CC=clang
EXE=./run
RLFLAGS=`pkg-config --cflags --libs raylib`
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu

all: main

main: main.c
	$(CC) $(CFLAGS) $(RLFLAGS) main.c -o $(EXE) && $(EXE)

clean:
	rm -f $(EXE) *.o *.i *.asm
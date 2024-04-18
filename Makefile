CC=clang
EXE=./run
RLFLAGS=`pkg-config --cflags --libs raylib`
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu
LIBFLAGS=-Wall -Wextra -pedantic -Wconversion -Wunreachable-code -Wno-gnu
LIB=./lib/evaluate.o

.PHONY: run

all: clean main run

run:
	$(EXE)

main: evaluate.o main.c
	$(CC) $(CFLAGS) $(RLFLAGS) main.c -o $(EXE) $(LIB)

eval_test: evaluate.o main.c
	$(CC) $(CFLAGS) $(RLFLAGS) main.c -o $(EXE) $(LIB) -DREAD_TEST

evaluate.o: ./lib/evaluate.c
	$(CC) $(LIBFLAGS) -c ./lib/evaluate.c -o $(LIB)

clean:
	rm -rf $(EXE) *.o *.i *.asm

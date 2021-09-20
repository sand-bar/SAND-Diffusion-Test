CC=gcc
CFLAGS=-O3 -w
STYLE=-std=c99
EXE = bin_SAND_64128 bin_SAND_128128

all: $(EXE)

bin_SAND_128128: test_SAND_128128.c SAND128128.h
	$(CC) $(CFLAGS) test_SAND_128128.c -o $@

bin_SAND_64128: test_SAND_64128.c SAND64128.h
	$(CC) $(CFLAGS) test_SAND_64128.c -o $@

clean:
	@echo "cleaning..."
	@rm -r $(EXE) *.o 2> /dev/null || true

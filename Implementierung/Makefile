CFLAGS=-std=gnu17 -O3 -march=native -mavx2
LDFLAGS=-lm

.PHONY: all clean

all: main

main: main.c expo.c intArr.c bignum.c longKomma.c divisionTests.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f main
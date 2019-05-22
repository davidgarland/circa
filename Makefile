CC=gcc
CFLAGS=-pipe
LDFLAGS=-Isrc/h -I. -L.

CFLAGS_FAST= $(CFLAGS) -O3 -s -DNDEBUG
CFLAGS_SMALL=$(CFLAGS) -Os -s -DNDEBUG
CFLAGS_BUILD=$(CFLAGS) -O2    -fno-omit-frame-pointer
CFLAGS_DEBUG=$(CFLAGS) -Og -g -fno-omit-frame-pointer
CFLAGS_SANITIZED=$(CFLAGS_DEBUG) -fsanitize=undefined -fsanitize=address -fsanitize=leak

#
# Build Options
#

default: build

fast:
	$(CC) -c $(CFLAGS_FAST) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.dSYM *.o

small:
	$(CC) -c $(CFLAGS_SMALL) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.dSYM *.o

build:
	$(CC) -c $(CFLAGS_BUILD) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.dSYM *.o

debug:
	$(CC) -c $(CFLAGS_DEBUG) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.dSYM *.o

sanitized:
	$(CC) -c $(CFLAGS_SANITIZED) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.dSYM *.o

#
# Tests
#

examples: debug
	$(CC) $(CFLAGS_DEBUG) ex/seq.c -lcirca $(LDFLAGS)

examples_sanitized: sanitized
	$(CC) $(CFLAGS_SANITIZED) ex/seq.c -lcirca $(LDFLAGS)

examples_header:
	$(CC) $(CFLAGS_DEBUG) -DCIRCA_HEADER_ONLY ex/seq.c $(LDFLAGS)

test: debug
	$(CC) $(CFLAGS_DEBUG) tests/test.c -I. -L. -Ilib/snow -DSNOW_ENABLED -o test.o -lcirca -lm

test_sanitized: sanitized
	$(CC) $(CFLAGS_SANITIZED) tests/test.c -I. -L. -Ilib/snow -DSNOW_ENABLED -o test.o -lcirca -lm

test_header:
	$(CC) $(CFLAGS_DEBUG) -DCIRCA_HEADER_ONLY tests/test.c -I. -L. -Ilib/snow -DSNOW_ENABLED -o test.o -lm

#
# Cleanup
#

clean:
	-@rm -f $(BIN) *.o *.out *.a *.so



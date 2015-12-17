.PHONY: all clean format

CPPFLAGS += -isystem src

CFLAGS += -Wall -Wextra -pedantic -ansi -std=c11 -g -O2 -mmacosx-version-min=10.6

LDFLAGS += -mmacosx-version-min=10.6

all: test

test: test.o src/core.o

test.o: test.c src/yalog/logging.h src/yalog/core.h

src/core.o: src/core.c src/yalog/core.h src/yalog/logging.h

clean:
	$(RM) test test.o core.o

format:
	clang-format -i *.h *.c

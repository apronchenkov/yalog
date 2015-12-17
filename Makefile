.PHONY: all clean format

CPPFLAGS += -isystem src

CFLAGS += -Wall -Wextra -pedantic -ansi -std=c11 -g -O2 -mmacosx-version-min=10.6

LDFLAGS += -mmacosx-version-min=10.6

all: test

test: test.o yalog.a

test.o: test.c src/yalog/logging.h src/yalog/core.h src/yalog/backend.h

yalog.a: src/core.o src/plain_config.o src/stderr_sink.o
	ar rcs $@ $^
src/core.o: src/core.c src/spinlock.h src/yalog/core.h src/yalog/logging.h
src/plain_config.o: src/plain_config.c src/yalog/backend.h src/yalog/core.h
src/stderr_sink.o: src/stderr_sink.c src/yalog/backend.h src/yalog/core.h

clean:
	$(RM) test test.o core.o yalog.a

format:
	clang-format -i src/*.h src/*.c src/yalog/*.h

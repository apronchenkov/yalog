.PHONY: all clean format

PROJECT = github.com/apronchenkov/yalog
BIN_PATH = ${TG_PATH}/bin
PKG_PATH = ${TG_PATH}/pkg/${PROJECT}
SRC_PATH = ${TG_PATH}/src/${PROJECT}

OPTFLAGS = -g -O2
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	OPTFLAGS += -mmacosx-version-min=10.6
endif

CPPFLAGS += -isystem ${TG_PATH}/src
CFLAGS += -Wall -Wextra -pedantic -std=gnu11 ${OPTFLAGS}
LDFLAGS += -lpthread ${OPTFLAGS}

all: ${BIN_PATH}/sample ${PKG_PATH}/core.a ${PKG_PATH}/basic.a ${PKG_PATH}/logging.a

${PKG_PATH}/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	${CC} ${CPPFLAGS} ${CFLAGS} ${OPTFLAGS} -c -o $@ $^


${PKG_PATH}/core.a: ${PKG_PATH}/core.o
	ar rcs $@ $^

${PKG_PATH}/basic.a: ${PKG_PATH}/file_sink.o ${PKG_PATH}/plain_config.o ${PKG_PATH}/stderr_sink.o ${PKG_PATH}/syslog_sink.o
	ar rcs $@ $^

${PKG_PATH}/logging.a: ${PKG_PATH}/logging_printf.o
	ar rcs $@ $^

${BIN_PATH}/sample: ${PKG_PATH}/sample.o ${PKG_PATH}/core.a ${PKG_PATH}/basic.a ${PKG_PATH}/logging.a
	${CC} ${LDFLAGS} ${OPTFLAGS} -o $@ $^

clean:
	$(RM) ${BIN_PATH}/sample ${PKG_PATH}/*.a ${PKG_PATH}/*.o

format:
	clang-format -i *.h *.c public/*.h

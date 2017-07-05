PROJECT = github.com/apronchenkov/yalog
BIN_PATH = ${TG_PATH}/bin
PKG_PATH = ${TG_PATH}/pkg/${PROJECT}
SRC_PATH = ${TG_PATH}/src/${PROJECT}

OPTFLAGS = -g -O2
CPPFLAGS += -Wall -Wextra -pedantic -isystem ${TG_PATH}/src
CFLAGS += -std=c99
LDFLAGS += -lpthread ${OPTFLAGS}

.PHONY: all clean format

all: ${BIN_PATH}/sample ${PKG_PATH}/core.a ${PKG_PATH}/basic.a ${PKG_PATH}/logging.a

${PKG_PATH}/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} ${CPPFLAGS} ${OPTFLAGS} -c -o $@ $^

${PKG_PATH}/core.a: ${PKG_PATH}/core.o
	ar rcs $@ $^

${PKG_PATH}/basic.a: ${PKG_PATH}/file_sink.o ${PKG_PATH}/plain_config.o ${PKG_PATH}/stderr_sink.o ${PKG_PATH}/syslog_sink.o
	ar rcs $@ $^

${PKG_PATH}/logging.a: ${PKG_PATH}/logging_printf.o
	ar rcs $@ $^

${TG_PATH}/pkg/github.com/apronchenkov/syslog_client/syslog_client.a:
	make -f ${TG_PATH}/src/github.com/apronchenkov/syslog_client/syslog_client.a $^

${BIN_PATH}/sample: ${PKG_PATH}/sample.o ${PKG_PATH}/core.a ${PKG_PATH}/basic.a ${PKG_PATH}/logging.a ${TG_PATH}/pkg/github.com/apronchenkov/syslog_client/syslog_client.a
	${CC} ${LDFLAGS} ${OPTFLAGS} -o $@ $^

clean:
	$(RM) $g{BIN_PATH}/sample ${PKG_PATH}/*.a ${PKG_PATH}/*.o

format:
	clang-format -i *.h *.c public/*.h

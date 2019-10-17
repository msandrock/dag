BINARY  = dag
TEST_BINARY = $(BINARY)_test
SHELL   = /bin/sh
VERSION = 1.0.0
CC      = clang++		# Compiler name
CFLAGS  = -std=c++17 -Wall -g -DVERSION=\"$(VERSION)\" -Werror #-Wmissing-prototypes
#LFLAGS  = -v
# clang optimize: -O3

all: $(BINARY)

${BINARY}: src/*.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

${TEST_BINARY}: src/*.cpp
	$(CC) $(CFLAGS) -DTEST=1 -o $@ $^ $(LFLAGS)

clean:
	if [ -f ${BINARY} ] ; then rm ${BINARY} ; fi
	if [ -f ${TEST_BINARY} ] ; then rm ${TEST_BINARY} ; fi

cppcheck:
	cppcheck --enable=warning --inconclusive --force --std=c++11 src/* --error-exitcode=1

BINARY  = dag
TEST_BINARY = $(BINARY)_test
SHELL   = /bin/sh
VERSION = 1.1.1
CC      = clang++		# Compiler name
CFLAGS  = -std=c++17 -optimize: -O3 -Wall -DVERSION=\"$(VERSION)\" -Werror #-Wmissing-prototypes
#CFLAGS  = -std=c++17 -g -Wall -DVERSION=\"$(VERSION)\" -Werror #-Wmissing-prototypes
#LFLAGS  = -v

all: $(BINARY) $(TEST_BINARY)

${BINARY}: *.cpp
	$(CC) $(CFLAGS) -o bin/$@ $^ $(LFLAGS)

${TEST_BINARY}: *.cpp tests/*.cpp
	$(CC) $(CFLAGS) -DTEST=1 -o bin/$@ $^ $(LFLAGS)

clean:
	if [ -f ${BINARY} ] ; then rm ${BINARY} ; fi
	if [ -f ${TEST_BINARY} ] ; then rm ${TEST_BINARY} ; fi

cppcheck:
	cppcheck --enable=warning --inconclusive --force --std=c++11 *.?pp --error-exitcode=1

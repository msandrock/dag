BINARY  = dag
SHELL   = /bin/sh
VERSION = 1.0.0
CC      = clang++		# Compiler name
CFLAGS  = -std=c++14 -Wall -g -DVERSION=\"$(VERSION)\" # -Werror -Wmissing-prototypes
#LFLAGS  = -v
# clang optimize: -O3

all: $(BINARY)

${BINARY}: src/*.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	if [ -f ${BINARY} ] ; then rm ${BINARY} ; fi

cppcheck:
	cppcheck --enable=warning --inconclusive --force --std=c++11 src/* --error-exitcode=1
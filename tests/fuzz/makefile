.PHONY: clean all files

all: fuzz_me files

OS != uname
SANITIZE_FLAGS != if test ${OS} != "OpenBSD"; then echo -fsanitize=address,undefined,leak -fsanitize-address-use-after-scope -fstack-protector-all; fi

fuzz_me: fuzz_me.c ../../include/cnpy.h
	${CC} -std=c11 -pedantic -W -Wall -Werror -Wno-unused-function -Wfatal-errors -O3 -lm ${SANITIZE_FLAGS} ${CFLAGS} ${LDFLAGS} -I ../../include/ -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE fuzz_me.c -o fuzz_me;

files: make_files.py
	python3 make_files.py

clean:
	-rm fuzz_me
	-rm -rf files

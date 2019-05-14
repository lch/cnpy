.PHONY: run all clean

OS != uname
SANITIZE_FLAGS != if test ${OS} != "OpenBSD"; then echo -fsanitize=address,undefined,leak -fsanitize-address-use-after-scope -fstack-protector-all; fi

run: all
	for x in test1; do \
		echo $$x:; \
		cd $$x; \
		./test; \
		cd ..; \
	done

all: test1/test

test1/test: test1/test.c ../../include/cnpy.h
	cc -std=gnu11 -W -Wall -Wno-unused-function -Werror -Wfatal-errors -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE -I ../../include -lm $(SANITIZE_FLAGS) $(CFLAGS) $(LDFLAGS) test1/test.c -o test1/test

clean:
	-rm */test

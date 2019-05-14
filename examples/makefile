.PHONY: clean

print_npy: print_npy.c ../include/cnpy.h
	${CC} -std=c99 -pedantic -W -Wall -Werror -Wno-unused-function -Wfatal-errors -O3 -lm ${CFLAGS} ${LDFLAGS} -I ../include/ -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE print_npy.c -o print_npy;

clean:
	-rm print_npy

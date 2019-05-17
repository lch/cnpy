.PHONY: all clean

all: print_npy ex1 ex2

print_npy: print_npy.c ../include/cnpy.h
	${CC} -std=c99 -pedantic -W -Wall -Werror -Wno-unused-function -Wfatal-errors -O3 -lm ${CFLAGS} ${LDFLAGS} -I ../include/ -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE print_npy.c -o print_npy;

ex1: ex1.c ../include/cnpy.h
	${CC} -std=c99 -pedantic -W -Wall -Werror -Wno-unused-function -Wno-gnu-imaginary-constant -Wfatal-errors -O3 -lm ${CFLAGS} ${LDFLAGS} -I ../include/ -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE ex1.c -o ex1;

ex2: ex2.c ../include/cnpy.h
	${CC} -std=c99 -pedantic -W -Wall -Werror -Wno-unused-function -Wfatal-errors -O3 -lm ${CFLAGS} ${LDFLAGS} -I ../include/ -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE ex2.c -o ex2;

clean:
	-rm print_npy ex1 ex2

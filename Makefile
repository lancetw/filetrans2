CC=gcc
CFLAGS=-O2 -Wall -C99

filetrans2: filetrans2.c client2.c server2.c
	${CC} -o filetrans2 ${CFLAGS} filetrans2.c client2.c server2.c        

debug: filetrans2_debug

filetrans2_debug: filetrans2.c client2.c server2.c
	${CC} -o filetrans2_d -D_DEBUG -g ${CFLAGS} filetrans2.c client2.c server2.c

clean:
	rm -rf filetrans2 filetrans2_d


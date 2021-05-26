CC=gcc
CFLAGS=-Wall -pedantic -Werror -std=c89 -g
APP=laserTank

$(APP): main.c linkedlist.o
	${CC} ${CFLAGS} -o $@ $^

linkedlist.o: linkedlist.c linkedlist.h
	${CC} ${CFLAGS} -c $<

clean:
	rm -rf *.o ${APP}


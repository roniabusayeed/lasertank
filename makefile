CC=gcc
CFLAGS=-Wall -pedantic -Werror -std=c89
APP=laserTank

${APP}: main.c linkedlist.o sleep.o utils.o
	${CC} ${CFLAGS} -o $@ $^

linkedlist.o: linkedlist.c linkedlist.h
	${CC} ${CFLAGS} -c $<

sleep.o: sleep.c sleep.h
	${CC} ${CFLAGS} -c $<

utils.o: utils.c utils.h
	${CC} ${CFLAGS} -c $<

clean:
	rm -rf *.o ${APP}


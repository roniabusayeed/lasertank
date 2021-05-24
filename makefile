CC=gcc
CFLAGS=-Wall -pedantic -Werror -std=c89 -g
APP=laserTank

$(APP): main.c
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -rf *.o ${APP}


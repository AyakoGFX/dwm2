# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

all: mydwm  # Renamed to mydwm

.c.o:
	${CC} -c ${CFLAGS} $<

mydwm: ${OBJ}  # Renamed target to mydwm
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f mydwm ${OBJ} mydwm-${VERSION}.tar.gz

dist: clean
	mkdir -p mydwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		mydwm.1 drw.h util.h ${SRC} mydwm.png transient.c mydwm-${VERSION}
	tar -cf mydwm-${VERSION}.tar mydwm-${VERSION}
	gzip mydwm-${VERSION}.tar
	rm -rf mydwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f mydwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/mydwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < mydwm.1 > ${DESTDIR}${MANPREFIX}/man1/mydwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/mydwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/mydwm\
		${DESTDIR}${MANPREFIX}/man1/mydwm.1

.PHONY: all clean dist install uninstall

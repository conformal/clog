# $clog$

.PATH:		${.CURDIR}/..

WANTLINT=
LIB= clog
SRCS= clog.c
DEBUG+= -ggdb3 
CFLAGS+= -Wall -Werror
HDRS= clog.h
NOMAN=

includes:
	@cd ${.CURDIR}; for i in ${HDRS}; do \
	cmp -s $$i ${DESTDIR}/usr/include/$$i || \
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) $$i \
	${DESTDIR}/usr/include; done

.include <bsd.own.mk>
.include <bsd.lib.mk>

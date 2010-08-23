# $clog$

PREFIX?=/usr/local
BINDIR=${PREFIX}/bin
LIBDIR=${PREFIX}/lib

#WANTLINT=
LIB= clog
SRCS= clog.c
.if defined(${COMPILER_VERSION})  &&  ${COMPILER_VERSION:L} == "gcc4"
CFLAGS+= -fdiagnostics-show-option -Wall -Werror
.else
CFLAGS+= -Wall -Werror
.endif
CFLAGS+= -ggdb3

MAN= clog.3
MANDIR= ${PREFIX}/man/cat
HDRS= clog.h

includes:
	@cd ${.CURDIR}; for i in ${HDRS}; do \
	cmp -s $$i ${PREFIX}/include/$$i || \
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) $$i \
	${PREFIX}/include; done

.include <bsd.own.mk>
.include <bsd.lib.mk>

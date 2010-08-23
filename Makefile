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
MLINKS+=clog.3 clog_init.3
MLINKS+=clog.3 clog_set_flags.3
MLINKS+=clog.3 clog_set_mask.3
MLINKS+=clog.3 CNINFO.3
MLINKS+=clog.3 CINFO.3
MLINKS+=clog.3 CNDBG.3
MLINKS+=clog.3 CDBG.3
MLINKS+=clog.3 CNWARNX.3
MLINKS+=clog.3 CWARNX.3
MLINKS+=clog.3 CNWARN.3
MLINKS+=clog.3 CWARN.3
MLINKS+=clog.3 CNFATALX.3
MLINKS+=clog.3 CFATALX.3
MLINKS+=clog.3 CNFATAL.3
MLINKS+=clog.3 CFATAL.3
HDRS= clog.h

includes:
	@cd ${.CURDIR}; for i in ${HDRS}; do \
	cmp -s $$i ${PREFIX}/include/$$i || \
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) $$i ${PREFIX}/include; \
	echo ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) $$i ${PREFIX}/include;\
	done

.include <bsd.own.mk>
.include <bsd.lib.mk>

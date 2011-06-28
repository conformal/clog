# $clog$

SYSTEM != uname -s
.if exists(${.CURDIR}/config/Makefile.$(SYSTEM:L))
.  include "${.CURDIR}/config/Makefile.$(SYSTEM:L)"
.endif

LOCALBASE?=/usr/local
BINDIR?=${LOCALBASE}/bin
LIBDIR?=${LOCALBASE}/lib
INCDIR?=${LOCALBASE}/include

#WANTLINT=
LIB= clog
SRCS= clog.c
HDRS= clog.h
MAN= clog.3
MLINKS+=clog.3 clog_version.3
MLINKS+=clog.3 clog_init.3
MLINKS+=clog.3 clog_set_flags.3
MLINKS+=clog.3 clog_set_logfile.3
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
MLINKS+=clog.3 CNABORTX.3
MLINKS+=clog.3 CABORTX.3
MLINKS+=clog.3 CNABORT.3
MLINKS+=clog.3 CABORT.3

CLEANFILES+=clog.cat3
CFLAGS+= -Wall -Werror
CFLAGS+= -ggdb3 -I${.CURDIR} -I${INCDIR}
#LDADD+= 

beforeinstall:
	${INSTALL} -m 0755 -o ${LIBOWN} -g ${LIBGRP} -d ${DESTDIR}${LIBDIR}/
	${INSTALL} -m 0755 -o ${BINOWN} -g ${BINGRP} -d ${DESTDIR}${INCDIR}/
	${INSTALL} -m 0755 -o ${MANOWN} -g ${MANGRP} -d ${DESTDIR}${MANDIR}3/

afterinstall:
	@cd ${.CURDIR}; for i in ${HDRS}; do \
	cmp -s $$i ${DESTDIR}${INCDIR}/$$i || \
	${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) $$i ${DESTDIR}${INCDIR}; \
	echo ${INSTALL} ${INSTALL_COPY} -m 444 -o $(BINOWN) -g $(BINGRP) $$i ${DESTDIR}${INCDIR};\
	done

uninstall:
	@for i in $(HDRS); do \
	echo rm -f ${DESTDIR}${INCDIR}/$$i ;\
	rm -f ${DESTDIR}${INCDIR}/$$i; \
	done

	@for i in $(_LIBS); do \
	echo rm -f ${DESTDIR}${LIBDIR}/$$i ;\
	rm -f ${DESTDIR}${LIBDIR}/$$i; \
	done
# XXX: Remove man pages

.include <bsd.own.mk>
.include <bsd.lib.mk>

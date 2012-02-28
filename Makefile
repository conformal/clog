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
MLINKS+=clog.3 CABORT.3
MLINKS+=clog.3 CABORTX.3
MLINKS+=clog.3 CALERT.3
MLINKS+=clog.3 CALERTX.3
MLINKS+=clog.3 CCRIT.3
MLINKS+=clog.3 CCRITX.3
MLINKS+=clog.3 CDBG.3
MLINKS+=clog.3 CEMERG.3
MLINKS+=clog.3 CEMERGX.3
MLINKS+=clog.3 CERR.3
MLINKS+=clog.3 CERRX.3
MLINKS+=clog.3 CFATAL.3
MLINKS+=clog.3 CFATALX.3
MLINKS+=clog.3 CINFO.3
MLINKS+=clog.3 CNDBG.3
MLINKS+=clog.3 CNINFO.3
MLINKS+=clog.3 CNNOTICE.3
MLINKS+=clog.3 CNOTICE.3
MLINKS+=clog.3 CNWARN.3
MLINKS+=clog.3 CNWARNX.3
MLINKS+=clog.3 CWARN.3
MLINKS+=clog.3 CWARNX.3

BUILDVERSION != sh "${.CURDIR}/buildver.sh"

CLEANFILES+=clog.cat3
CFLAGS+= -Wall -Werror
CFLAGS+= -ggdb3 -I${.CURDIR} -I${INCDIR}
.if !${BUILDVERSION} == ""
CPPFLAGS+= -DBUILDSTR=\"$(BUILDVERSION)\"
.endif
#LDADD+=

beforeinstall:
	@if [ ! -d ${DESTDIR}${LIBDIR} ]; then \
	echo ${INSTALL} -m 0755 -o ${LIBOWN} -g ${LIBGRP} -d ${DESTDIR}${LIBDIR}/; \
	${INSTALL} -m 0755 -o ${LIBOWN} -g ${LIBGRP} -d ${DESTDIR}${LIBDIR}/; \
	fi
	@if [ ! -d ${DESTDIR}${INCDIR} ]; then \
	echo ${INSTALL} -m 0755 -o ${BINOWN} -g ${BINGRP} -d ${DESTDIR}${INCDIR}/; \
	${INSTALL} -m 0755 -o ${BINOWN} -g ${BINGRP} -d ${DESTDIR}${INCDIR}/; \
	fi
	@if [ ! -d ${DESTDIR}${MANDIR} ]; then \
	echo ${INSTALL} -m 0755 -o ${MANOWN} -g ${MANGRP} -d ${DESTDIR}${MANDIR}3/; \
	${INSTALL} -m 0755 -o ${MANOWN} -g ${MANGRP} -d ${DESTDIR}${MANDIR}3/; \
	fi

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

# $clog$

SYSTEM != uname -s
.if exists(${.CURDIR}/../config/Makefile.$(SYSTEM:L))
.  include "${.CURDIR}/../config/Makefile.$(SYSTEM:L)"
.endif

LOCALBASE?=/usr/local
BINDIR?=${LOCALBASE}/bin
LIBDIR?=${LOCALBASE}/lib
INCDIR?=${LOCALBASE}/include

PROG= testclog 
NOMAN=
NO_MAN=

.if exists(${.CURDIR}/../obj)
LDADD+=	-L${.CURDIR}/../obj
DPADD+= ${.CURDIR}/../obj/libclog.a
.else
LDADD+=	-L${.CURDIR}/..
DPADD+= ${.CURDIR}/../libclog.a
.endif

SRCS= testclog.c
COPT+= -O2
CFLAGS+= -Wall -Werror -ggdb3
LDADD+= -L${LOCALBASE}/lib -lclog

CFLAGS+= -I${.CURDIR}/..

.include <bsd.prog.mk>


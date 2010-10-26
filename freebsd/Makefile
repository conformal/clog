#LIB=            z
#SHLIBDIR?=      /lib
#MAN=            zlib.3
#
##CFLAGS+=       -DMAX_WBITS=14 -DMAX_MEM_LEVEL=7
##CFLAGS+=       -g -DDEBUG
##CFLAGS+=       -Wall -Wwrite-strings -Wpointer-arith -Wconversion \
##               -Wstrict-prototypes -Wmissing-prototypes
#
#CFLAGS+=        -DHAS_snprintf -DHAS_vsnprintf
#
#CLEANFILES+=    example.o example foo.gz minigzip.o minigzip
#
#SRCS = adler32.c compress.c crc32.c gzio.c uncompr.c deflate.c trees.c \
#       zutil.c inflate.c inftrees.c inffast.c zopen.c infback.c
#INCS=           zconf.h zlib.h
#
#minigzip:       all minigzip.o
#        $(CC) -o minigzip minigzip.o -L. -lz
#
#example:        all example.o
#        $(CC) -o example example.o -L. -lz
#
#test: example minigzip
#        (export LD_LIBRARY_PATH=. ; ./example )
#        (export LD_LIBRARY_PATH=. ; \
#                echo hello world | ./minigzip | ./minigzip -d )
#
#.include <bsd.lib.mk>

PREFIX?=/usr/local
#BINDIR=${DESTDIR}${PREFIX}/bin
#LIBDIR=${DESTDIR}${PREFIX}/lib
LIBDIR?= ${PREFIX}/lib

.include <${.CURDIR}/../shlib_version>
VPATH=${.CURDIR}/..
SHLIB_MAJOR=${major}

LIB= clog
MAN= ../clog.3
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

CFLAGS+= -fdiagnostics-show-option -Wall -Werror
CFLAGS+= -ggdb3
CPPFLAGS+=-I${.CURDIR}

CLEANFILES+= clog.c clog.h clog.3

SRCS= clog.c

.include <bsd.lib.mk>

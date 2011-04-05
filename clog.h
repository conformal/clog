/* $clog$ */
/*
 * Copyright (c) 2009, 2010 Marco Peereboom <marco@peereboom.us>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef CLOG_H
#define CLOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <syslog.h>
#include <errno.h>
#include <stdarg.h>

#include <sys/socket.h>
#include <sys/time.h>

#include "conformal_lens.h"

#define CLOG_F_FILE		(1<<0)
#define CLOG_F_FUNC		(1<<1)
#define CLOG_F_LINE		(1<<2)
#define CLOG_F_DATE		(1<<26)
#define CLOG_F_DTIME		(1<<27)
#define CLOG_F_STDERR		(1<<28)
#define CLOG_F_SYSLOG		(1<<29)
#define CLOG_F_ENABLE		(1<<30)
#define CLOG_F_DBGENABLE	(1<<31)

#define CLOG_F_ALLFLAGS							\
	(CLOG_F_FILE | CLOG_F_FUNC | CLOG_F_LINE |			\
	CLOG_F_DATE | CLOG_F_DTIME |					\
	CLOG_F_STDERR | CLOG_F_SYSLOG | CLOG_F_ENABLE | CLOG_F_DBGENABLE)

int	clog_set_flags(uint32_t);
void	clog_set_mask(uint64_t);
void	clog_dbg_internal(int, int, uint64_t, const char *, const char *, int,
	    const char *, ...) __attribute__((__format__ (printf, 7, 8)));

/* no errno */
#define CNINFO(a, b...)		do { clog_dbg_internal(LOG_INFO, 0, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CINFO(b...)		do { clog_dbg_internal(LOG_INFO, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CNDBG(a, b...)		do { clog_dbg_internal(LOG_DEBUG, 0, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CDBG(b...)		do { clog_dbg_internal(LOG_DEBUG, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)

/* errno */
#define CNWARNX(a, b...)	do { clog_dbg_internal(LOG_WARNING, 0, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CWARNX(b...)		do { clog_dbg_internal(LOG_WARNING, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CNWARN(a, b...)		do { clog_dbg_internal(LOG_WARNING, 1, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CWARN(b...)		do { clog_dbg_internal(LOG_WARNING, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)

#define CNFATALX(a, b...)	do { clog_dbg_internal(LOG_CRIT, 0, a, __FILE__, __FUNCTION__, __LINE__, b); exit(1); } while (0)
#define CFATALX(b...)		do { clog_dbg_internal(LOG_CRIT, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); exit(1); } while (0)
#define CNFATAL(a, b...)	do { clog_dbg_internal(LOG_CRIT, 1, a, __FILE__, __FUNCTION__, __LINE__, b); exit(1); } while (0)
#define CFATAL(b...)		do { clog_dbg_internal(LOG_CRIT, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); exit(1); } while (0)

/* old interface */
void		 clog_init(int);
void		 clog_warn(const char *, ...);
void		 clog_warnx(const char *, ...);
void		 clog_info(const char *, ...);
void		 clog_debug(const char *, ...);
__dead void	 clog_fatal(const char *);
__dead void	 clog_fatalx(const char *, ...);

#endif /* CLOG_H */

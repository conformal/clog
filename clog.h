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
#include <err.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <regex.h>
#include <netdb.h>
#include <syslog.h>
#include <errno.h>

#include <sys/stdarg.h>
#include <sys/socket.h>

#define CLOG_F_FILE	(1<<0)
#define CLOG_F_FUNC	(1<<1)
#define CLOG_F_LINE	(1<<2)
#define CLOG_F_TIME	(1<<28)
#define CLOG_F_STDERR	(1<<29)
#define CLOG_F_SYSLOG	(1<<30)
#define CLOG_F_ENABLED	(1<<31)

#define CLOG_F_ALLFLAGS							\
	(CLOG_F_FILE | CLOG_F_FUNC | CLOG_F_LINE |			\
	CLOG_F_TIME | CLOG_F_STDERR | CLOG_F_SYSLOG | CLOG_F_ENABLED)

int	clog_set_flags(u_int32_t);
void	clog_set_mask(u_int64_t);
void	clog_dbg_internal(int, u_int64_t, const char *, const char *, int,
	    const char *, ...);

#define CNDBG(a, b...)	do { clog_dbg_internal(LOG_DEBUG, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CDBG(b...)	do { clog_dbg_internal(LOG_DEBUG, -1, __FILE__, __FUNCTION__, __LINE__, b); } while (0)

/* old interface */
void		 clog_init(int);
void		 clog_warn(const char *, ...);
void		 clog_warnx(const char *, ...);
void		 clog_info(const char *, ...);
void		 clog_debug(const char *, ...);
__dead void	 clog_fatal(const char *);
__dead void	 clog_fatalx(const char *, ...);

#endif /* CLOG_H */

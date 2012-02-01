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

#ifdef NEED_LIBCLENS
#include <clens.h>
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <syslog.h>

/* versioning */
#define CLOG_STRINGIFY(x)	#x
#define CLOG_STR(x)		CLOG_STRINGIFY(x)
#define CLOG_VERSION_MAJOR	0
#define CLOG_VERSION_MINOR	5
#define CLOG_VERSION_PATCH	0
#define CLOG_VERSION		CLOG_STR(CLOG_VERSION_MAJOR) "." \
				CLOG_STR(CLOG_VERSION_MINOR) "." \
				CLOG_STR(CLOG_VERSION_PATCH)

const char	*clog_verstring(void);
void		 clog_version(int *major, int *minor, int *patch);


#define CLOG_F_FILE		(1<<0)
#define CLOG_F_FUNC		(1<<1)
#define CLOG_F_LINE		(1<<2)
#define CLOG_F_FILOG		(1<<24)
#define CLOG_F_PID		(1<<25)
#define CLOG_F_DATE		(1<<26)
#define CLOG_F_DTIME		(1<<27)
#define CLOG_F_STDERR		(1<<28)
#define CLOG_F_SYSLOG		(1<<29)
#define CLOG_F_ENABLE		(1<<30)
#define CLOG_F_DBGENABLE	(1<<31)

#define CLOG_F_ALLFLAGS							\
	(CLOG_F_FILE | CLOG_F_FUNC | CLOG_F_LINE |			\
	CLOG_F_PID | CLOG_F_DATE | CLOG_F_DTIME |			\
	CLOG_F_FILOG | CLOG_F_STDERR | CLOG_F_SYSLOG |			\
	CLOG_F_ENABLE | CLOG_F_DBGENABLE)

int		 clog_set_flags(uint32_t);
uint32_t	 clog_get_flags(void);
int		 clog_set_logfile(const char *);
const char	*clog_get_logfile(void);
void		 clog_set_log_callback(void (*)(void *, int, int, const char *,
		    va_list), void *);
void		 clog_default_log_callback(void *, int, int, const char *,
		    va_list);
void		 clog_set_mask(uint64_t);
uint64_t	 clog_get_mask(void);
int		 clog_mask_is_set(uint64_t);
void		 clog_end(void);
void		 clog_dbg_internal(int, int, uint64_t, const char *,
		     const char *, int, const char *, ...)
		     __attribute__((__format__ (printf, 7, 8)));

#define CEMERG(b...)		do { clog_dbg_internal(LOG_EMERG, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CEMERGX(b...)		do { clog_dbg_internal(LOG_EMERG, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CALERT(b...)		do { clog_dbg_internal(LOG_ALERT, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CALERTX(b...)		do { clog_dbg_internal(LOG_ALERT, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CCRIT(b...)		do { clog_dbg_internal(LOG_CRIT, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CCRITX(b...)		do { clog_dbg_internal(LOG_CRIT, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CERR(b...)		do { clog_dbg_internal(LOG_ERR, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CERRX(b...)		do { clog_dbg_internal(LOG_ERR, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CWARN(b...)		do { clog_dbg_internal(LOG_WARNING, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CNWARN(a, b...)		do { clog_dbg_internal(LOG_WARNING, 1, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CWARNX(b...)		do { clog_dbg_internal(LOG_WARNING, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CNWARNX(a, b...)	do { clog_dbg_internal(LOG_WARNING, 0, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CNNOTICE(a, b...)	do { clog_dbg_internal(LOG_NOTICE, 0, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CNOTICE(b...)		do { clog_dbg_internal(LOG_NOTICE, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CNINFO(a, b...)		do { clog_dbg_internal(LOG_INFO, 0, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CINFO(b...)		do { clog_dbg_internal(LOG_INFO, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CNDBG(a, b...)		do { clog_dbg_internal(LOG_DEBUG, 0, a, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CDBG(b...)		do { clog_dbg_internal(LOG_DEBUG, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); } while (0)
#define CFATAL(b...)		do { clog_dbg_internal(LOG_EMERG, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); exit(1); } while (0)
#define CFATALX(b...)		do { clog_dbg_internal(LOG_EMERG, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); exit(1); } while (0)
#define CABORT(b...)		do { clog_dbg_internal(LOG_EMERG, 1, -1llu, __FILE__, __FUNCTION__, __LINE__, b); abort(); } while (0)
#define CABORTX(b...)		do { clog_dbg_internal(LOG_EMERG, 0, -1llu, __FILE__, __FUNCTION__, __LINE__, b); abort(); } while (0)

/* old interface */
void		 clog_init(int);
void		 clog_warn(const char *, ...);
void		 clog_warnx(const char *, ...);
void		 clog_info(const char *, ...);
void		 clog_debug(const char *, ...);
__dead void	 clog_fatal(const char *);
__dead void	 clog_fatalx(const char *, ...);

#endif /* CLOG_H */

/* $OpenBSD: log.c,v 1.8 2007/08/22 21:04:30 ckuethe Exp $ */
/*
 * Copyright (c) 2010 Marco Peereboom <marco@peereboom.us>
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "clog.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <inttypes.h>
#include <limits.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#ifdef BUILDSTR
static const char *vertag = CLOG_VERSION " " BUILDSTR;
#else
static const char *vertag = CLOG_VERSION;
#endif

extern char		*__progname;

static int		clog_initialized;
static uint64_t		clog_ext_mask;
static uint32_t		clog_flags;
static struct timeval	clog_start_of_day;
static char		clog_logfile[PATH_MAX] = "";
static void		(*clog_log_callback)(void *, int, int, const char *,
			    va_list) = clog_default_log_callback;
static void		*clog_log_state;

int
clog_set_logfile(const char *logfile)
{
	if (strlcpy(clog_logfile, logfile, sizeof(clog_logfile)) >=
		sizeof(clog_logfile))
		return (1);
	return (0);
}

const char *
clog_get_logfile(void)
{
	return (clog_logfile);
}

const char *
clog_verstring(void)
{
	return (vertag);
}

void
clog_version(int *major, int *minor, int *patch)
{
	*major = CLOG_VERSION_MAJOR;
	*minor = CLOG_VERSION_MINOR;
	*patch = CLOG_VERSION_PATCH;
}

void
clog_opensyslog(void)
{
	openlog(__progname, LOG_PID | LOG_NDELAY | LOG_CONS, LOG_DAEMON);
}

void
clog_initialize(int log_syslog)
{
	clog_ext_mask = 1llu<<63;
	gettimeofday(&clog_start_of_day, NULL);

	if (log_syslog)
		clog_opensyslog();

	tzset();

	clog_initialized = 1;
}

void
clog_end(void)
{
	if (clog_flags & CLOG_F_SYSLOG)
		closelog();
	clog_initialized = 0;
}

int
clog_set_flags(uint32_t f)
{
	if ((f & ~CLOG_F_ALLFLAGS) != 0)
		return (1);

	if ((f & CLOG_F_FILOG) != 0 && clog_logfile[0] == '\0')
		return (1);

	if ((f & CLOG_F_SYSLOG) != (clog_flags & CLOG_F_SYSLOG)) {
		/* syslog toggle */
		if ((clog_flags & CLOG_F_SYSLOG))
			closelog();
		else
			clog_opensyslog();
	}

	clog_flags = f;

	return (0);
}

uint32_t
clog_get_flags(void)
{
	return (clog_flags);
}

void
clog_set_mask(uint64_t f)
{
	clog_ext_mask = f | (1llu<<63);
}

uint64_t
clog_get_mask(void)
{
	return (clog_ext_mask & ~(1llu<<63));
}

int
clog_mask_is_set(uint64_t bit)
{
	return (clog_ext_mask & bit);
}

void
clog_set_log_callback(void (*func)(void *, int, int, const char *, va_list),
    void *st)
{
	clog_log_callback = func;
	clog_log_state = st;

}

void
clog_default_log_callback(void *state, int flags, int pri, const char *message,
    va_list args)
{
	const char		*lmessage;
	FILE			*stream;
	va_list			 fargs, largs, sargs;

	if ((CLOG_F_FILOG & flags) != 0) {
		stream = fopen(clog_logfile, "a");
		if (stream) {
			va_copy(fargs, args);
			vfprintf(stream, message, fargs);
			fclose(stream);
		}
	}

	if ((CLOG_F_STDERR & flags) != 0) {
		va_copy(sargs, args);
		vfprintf(stderr, message, sargs);
		fflush(stderr);
	}

	if ((CLOG_F_SYSLOG & flags) != 0) {
		va_copy(largs, args);
		if ((CLOG_F_DATE & flags) != 0)
			lmessage = message + 16;
		else
			lmessage = message;

		if (clog_initialized == 0)
			clog_initialize(1);

		vsyslog(pri, lmessage, largs);
	}
}

void
clog_print(int pri, int do_errno, const char *file, const char * func, int line,
    const char *fmt, va_list ap)
{
	const char		*fi = "", *fu = "", *li = "";
	const char		*pi2 = "", *fi2 = "", *fu2 = "", *li2 = "";
	char			li_buf[16], *start = "", *end = "";
	char			*s = NULL, *ts = "", *ts2 = "" , delta[32];
	char			*er = "", *er2 = "", *pi = "";
	int			got_some = 0, free_pi = 0;
	struct timeval		now, elapsed;
	time_t			tnow;

	delta[0] = '\0';
	if ((CLOG_F_DTIME & clog_flags) != 0) {
		if (clog_initialized == 0)
			clog_initialize(0);

		if (gettimeofday(&now, NULL) != -1) {
			timersub(&now, &clog_start_of_day, &elapsed);
			snprintf(delta, sizeof delta, "%" PRId64 ".%.6ld: ",
			    (int64_t) elapsed.tv_sec, (long) elapsed.tv_usec);
		}
	}

	if ((CLOG_F_DATE & clog_flags) != 0) {
		time(&tnow);
		ts = ctime(&tnow) + 4;
		ts[15] = '\0';
		if ((CLOG_F_DTIME & clog_flags) == 0)
			ts2 = ": ";
		else
			ts2 = " ";
	}

	if ((CLOG_F_PID & clog_flags) != 0) {
		if (asprintf(&pi, "%s(%d)", __progname, getpid()) != -1) {
			pi2 = " ";
			got_some = 1;
			free_pi = 1;
		} else {
			pi = "";
		}
	}

	if ((CLOG_F_FILE & clog_flags) != 0) {
		fi = file;
		fi2 = " ";
		got_some = 1;
	}
	if ((CLOG_F_FUNC & clog_flags) != 0) {
		fu = func;
		fu2 = " ";
		got_some = 1;
	}
	if ((CLOG_F_LINE & clog_flags) != 0) {
		snprintf(li_buf, sizeof li_buf, "%d", line);
		li = li_buf;
		li2 = " ";
		got_some = 1;
	}

	if (got_some) {
		start = "< ";
		end = "> ";
	}

	if (do_errno) {
		er = ": ";
		er2 = strerror(errno);
	}

	if (asprintf(&s, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
	    ts, ts2, delta, start, pi, pi2, fi, fi2, fu, fu2,
	    li, li2, end, fmt, er, er2) == -1) {
		/* out of memory */
		abort();
	}

	(*clog_log_callback)(clog_log_state, clog_flags, pri, s, ap);

	free(s);
	if (free_pi)
		free(pi);
}

void
clog_dbg_internal(int pri, int do_errno, uint64_t mask, const char *file,
    const char * func, int line, const char *fmt, ...)
{
	va_list			ap;

	if ((mask & clog_ext_mask) == 0 || (clog_flags & CLOG_F_ENABLE) == 0)
		return;
	if (pri == LOG_DEBUG && !(clog_flags & CLOG_F_DBGENABLE))
		return;

	va_start(ap, fmt);
	clog_print(pri, do_errno, file, func, line, fmt, ap);
	va_end(ap);
}

/* old interface */
int		logdebug;
int		debugsyslog;

void
vlog(int pri, const char *fmt, va_list ap)
{
	char	*nfmt;

	if (logdebug) {
		/* best effort in out of mem situations */
		if (asprintf(&nfmt, "%s\n", fmt) == -1) {
			vfprintf(stderr, fmt, ap);
			fprintf(stderr, "\n");
		} else {
			vfprintf(stderr, nfmt, ap);
			free(nfmt);
		}
		fflush(stderr);
	} else
		vsyslog(pri, fmt, ap);
}

void
logit(int pri, const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	vlog(pri, fmt, ap);
	va_end(ap);
}

void
clog_init(int n_debug)
{
	logdebug = n_debug;

	if (!logdebug)
		openlog(__progname, LOG_PID | LOG_NDELAY | LOG_CONS,
		    LOG_DAEMON);
	tzset();

	clog_initialize(!logdebug); /* initialize new interface too */
}

void
clog_warn(const char *emsg, ...)
{
	char	*nfmt;
	va_list	 ap;

	/* best effort to even work in out of memory situations */
	if (emsg == NULL)
		logit(LOG_CRIT, "%s", strerror(errno));
	else {
		va_start(ap, emsg);

		if (asprintf(&nfmt, "%s: %s", emsg, strerror(errno)) == -1) {
			/* we tried it... */
			vlog(LOG_CRIT, emsg, ap);
			logit(LOG_CRIT, "%s", strerror(errno));
		} else {
			vlog(LOG_CRIT, nfmt, ap);
			free(nfmt);
		}
		va_end(ap);
	}
}

void
clog_warnx(const char *emsg, ...)
{
	va_list	 ap;

	va_start(ap, emsg);
	vlog(LOG_CRIT, emsg, ap);
	va_end(ap);
}

__dead void
clog_fatal(const char *emsg)
{
	if (emsg == NULL)
		logit(LOG_CRIT, "fatal: %s", strerror(errno));
	else
		if (errno)
			logit(LOG_CRIT, "fatal: %s: %s",
			    emsg, strerror(errno));
		else
			logit(LOG_CRIT, "fatal: %s", emsg);

	exit(1);
}

__dead void
clog_fatalx(const char *emsg, ...)
{
	va_list  ap;

	va_start(ap, emsg);
	vlog(LOG_CRIT, emsg, ap);
	va_end(ap);

	exit(1);
}

void
clog_info(const char *emsg, ...)
{
	va_list	 ap;

	va_start(ap, emsg);
	vlog(LOG_INFO, emsg, ap);
	va_end(ap);
}

void
clog_debug(const char *emsg, ...)
{
	va_list	 ap;

	if (logdebug || debugsyslog) {
		va_start(ap, emsg);
		vlog(LOG_DEBUG, emsg, ap);
		va_end(ap);
	}
}

const char *
clog_sockaddr(struct sockaddr *sa)
{
	static char	buf[NI_MAXHOST];

	if (getnameinfo(sa, SA_LEN(sa), buf, sizeof(buf), NULL, 0,
	    NI_NUMERICHOST))
		return ("(unknown)");
	else
		return (buf);
}

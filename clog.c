/* $clog$ */
/*	$OpenBSD: log.c,v 1.8 2007/08/22 21:04:30 ckuethe Exp $ */
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

#include <time.h>

extern char		*__progname;

static int		clog_initialized;
static uint64_t	clog_ext_mask;
static uint32_t	clog_flags;
static struct timeval	clog_start_of_day;

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

int
clog_set_flags(uint32_t f)
{
	if ((f & ~CLOG_F_ALLFLAGS) != 0)
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

void
clog_set_mask(uint64_t f)
{
	clog_ext_mask = f | (1llu<<63);
}

void
clog_print(int pri, int do_errno, const char *file, const char * func, int line,
    const char *fmt, va_list ap)
{
	const char		*fi = "", *fu = "", *li = "";
	const char		*fi2 = "", *fu2 = "", *li2 = "";
	char			li_buf[16], *start = "", *end = "";
	char			*s = NULL, *ts = "", *ts2 = "" , delta[32];
	char			*sl = NULL, *er = "", *er2 = "";
	int			got_some = 0;
	struct timeval		now, elapsed;
	time_t			tnow;
	va_list			sap;

	delta[0] = '\0';
	if ((CLOG_F_DTIME & clog_flags) != 0) {
		if (clog_initialized == 0)
			clog_initialize(0);

		if (gettimeofday(&now, NULL) != -1) {
			timersub(&now, &clog_start_of_day, &elapsed);
			snprintf(delta, sizeof delta, "%ld.%.6ld: ",
			    elapsed.tv_sec, elapsed.tv_usec);
		}
	}

	if ((CLOG_F_DATE & clog_flags) != 0) {
		time(&tnow);
		ts = ctime(&tnow) + 4;
		ts[12] = '\0';
		if ((CLOG_F_DTIME & clog_flags) == 0)
			ts2 = ": ";
		else
			ts2 = " ";
	}

	if ((CLOG_F_FILE & clog_flags) != 0) {
		fi = file;
		fi2 = " ";
		got_some = 1;
	}
	if ((CLOG_F_FUNC & clog_flags) != 0) {
		fu = func;
		fu2 = " ";
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

	if (asprintf(&s, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
	    ts, ts2, delta, start, fi, fi2, fu, fu2, li, li2, end, fmt, er, er2) == -1) {
		/* out of memory */
		abort();
	}

	va_copy(sap, ap);

	if ((CLOG_F_STDERR & clog_flags) != 0) {
		vfprintf(stderr, s, ap);
		fflush(stderr);
	}

	if ((CLOG_F_SYSLOG & clog_flags) != 0) {
		if ((CLOG_F_DATE & clog_flags) != 0)
			sl = s + 13;
		else
			sl = s;

		if (clog_initialized == 0)
			clog_initialize(1);

		vsyslog(pri, sl, sap);
	}

	free(s);
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

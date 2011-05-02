#include "clog.h"

#include <err.h>

#define CLOGTEST_F_LEVEL1	(1<<0)
#define CLOGTEST_F_LEVEL2	(1<<1)
#define CLOGTEST_F_LEVEL3	(1<<2) /* not printed */

int
main(int argc, char *argv[])
{
	clog_init(1);

	if (clog_set_flags(CLOG_F_FILE | CLOG_F_FUNC | CLOG_F_LINE |
	    CLOG_F_STDERR | CLOG_F_ENABLE | CLOG_F_DTIME))
		errx(1, "can't set clog flags");
	clog_set_mask(CLOGTEST_F_LEVEL1 | CLOGTEST_F_LEVEL2);

	CNDBG(CLOGTEST_F_LEVEL3, "not printed moo %s", "meh");
	CNDBG(CLOGTEST_F_LEVEL1, "printed moo %s", "meh");
	CDBG("dbg too %s", "coo");

	if (clog_set_flags(CLOG_F_STDERR | CLOG_F_ENABLE | CLOG_F_DTIME | CLOG_F_DATE))
		errx(1, "can't clear clog flags");

	CNDBG(1, "moo %s", "meh");
	CDBG("dbg2 too %s", "coo");

	if (clog_set_flags(CLOG_F_STDERR | CLOG_F_ENABLE | CLOG_F_DATE | CLOG_F_SYSLOG | CLOG_F_DBGENABLE))
		errx(1, "can't clear clog flags");

	CNDBG(1, "moo %s", "meh");
	CDBG("dbg 3 too %s", "coo");

	CWARNX("OMG WARNING");
	CWARN("OMG WARNING with errno");
	CNFATAL(CLOGTEST_F_LEVEL3, "OMG FATAL");

	/* not reached */
	CWARNX("OMG WARNING not reached");

	return (0);
}

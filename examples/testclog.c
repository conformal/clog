#include "clog.h"

#define CLOGTEST_F_LEVEL1	(1<<0)
#define CLOGTEST_F_LEVEL2	(1<<1)

int
main(int argc, char *argv[])
{
	if (clog_set_flags(CLOG_F_FILE | CLOG_F_FUNC | CLOG_F_LINE |
	    CLOG_F_STDERR | CLOG_F_ENABLED | CLOG_F_TIME))
		errx(1, "can't set clog flags");
	clog_set_mask(CLOGTEST_F_LEVEL1 | CLOGTEST_F_LEVEL2);

	CNDBG(1, "moo %s", "meh");
	CDBG("too %s", "coo");

	if (clog_set_flags(CLOG_F_STDERR | CLOG_F_ENABLED | CLOG_F_TIME))
		errx(1, "can't clear clog flags");

	CNDBG(1, "moo %s", "meh");
	CDBG("too %s", "coo");

	return (0);
}

# $clog$

# Attempt to include platform specific makefile.
# OSNAME may be passed in.
OSNAME ?= $(shell uname -s)
OSNAME := $(shell echo $(OSNAME) | tr A-Z a-z)
-include config/Makefile.$(OSNAME)

# Default paths.
LOCALBASE ?= /usr/local
BINDIR ?= ${LOCALBASE}/bin
LIBDIR ?= ${LOCALBASE}/lib
INCDIR ?= ${LOCALBASE}/include
MANDIR ?= $(LOCALBASE)/share/man

# Use obj directory if it exists.
OBJPREFIX ?= obj/
ifeq "$(wildcard $(OBJPREFIX))" ""
	OBJPREFIX =	
endif

# Get shared library version.
-include shlib_version
SO_MAJOR = $(major)
SO_MINOR = $(minor)

# System utils.
AR ?= ar
CC ?= gcc
INSTALL ?= install
LN ?= ln
LNFLAGS ?= -sf
MKDIR ?= mkdir
RM ?= rm -f

# Compiler and linker flags.
CPPFLAGS += -DNEED_LIBCLENS
INCFLAGS += -I $(INCDIR)/clens
WARNFLAGS ?= -Wall -Werror
DEBUG += -g
CFLAGS += $(INCFLAGS) $(WARNFLAGS) $(DEBUG)
LDFLAGS += -lclens
SHARED_OBJ_EXT ?= o

LIB.NAME = clog
LIB.SRCS = clog.c
LIB.HEADERS = clog.h
LIB.MANPAGES = clog.3
LIB.MLINKS  = clog.3 clog_version.3
LIB.MLINKS += clog.3 clog_init.3
LIB.MLINKS += clog.3 clog_set_flags.3
LIB.MLINKS += clog.3 clog_set_logfile.3
LIB.MLINKS += clog.3 clog_set_mask.3
LIB.MLINKS += clog.3 CNINFO.3
LIB.MLINKS += clog.3 CINFO.3
LIB.MLINKS += clog.3 CNDBG.3
LIB.MLINKS += clog.3 CDBG.3
LIB.MLINKS += clog.3 CNWARNX.3
LIB.MLINKS += clog.3 CWARNX.3
LIB.MLINKS += clog.3 CNWARN.3
LIB.MLINKS += clog.3 CWARN.3
LIB.MLINKS += clog.3 CNFATALX.3
LIB.MLINKS += clog.3 CFATALX.3
LIB.MLINKS += clog.3 CNFATAL.3
LIB.MLINKS += clog.3 CFATAL.3
LIB.MLINKS += clog.3 CNABORTX.3
LIB.MLINKS += clog.3 CABORTX.3
LIB.MLINKS += clog.3 CNABORT.3
LIB.MLINKS += clog.3 CABORT.3
LIB.OBJS = $(addprefix $(OBJPREFIX), $(LIB.SRCS:.c=.o))
LIB.SOBJS = $(addprefix $(OBJPREFIX), $(LIB.SRCS:.c=.$(SHARED_OBJ_EXT)))
LIB.DEPS = $(addsuffix .depend, $(LIB.OBJS))
ifneq "$(LIB.OBJS)" "$(LIB.SOBJS)"
	LIB.DEPS += $(addsuffix .depend, $(LIB.SOBJS))
endif
LIB.MDIRS = $(foreach page, $(LIB.MANPAGES), $(subst ., man, $(suffix $(page)))) 
LIB.MLINKS := $(foreach page, $(LIB.MLINKS), $(subst ., man, $(suffix $(page)))/$(page)) 
LIB.LDFLAGS = $(LDFLAGS.EXTRA) $(LDFLAGS) 

all: $(OBJPREFIX)$(LIB.SHARED) $(OBJPREFIX)$(LIB.STATIC)

obj:
	-$(MKDIR) obj

$(OBJPREFIX)$(LIB.SHARED): $(LIB.SOBJS)
	$(CC) $(LDFLAGS.SO) $^ $(LIB.LDFLAGS) -o $@

$(OBJPREFIX)$(LIB.STATIC): $(LIB.OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJPREFIX)%.$(SHARED_OBJ_EXT): %.c
	@echo "Generating $@.depend"
	@$(CC) $(INCFLAGS) -MM $(CPPFLAGS) $< | \
	sed 's,$*\.o[ :]*,$@ $@.depend : ,g' > $@.depend
	$(CC) $(CFLAGS) $(PICFLAG) $(CPPFLAGS) -o $@ -c $<

$(OBJPREFIX)%.o: %.c
	@echo "Generating $@.depend"
	@$(CC) $(INCFLAGS) -MM $(CPPFLAGS) $< | \
	sed 's,$*\.o[ :]*,$@ $@.depend : ,g' >> $@.depend
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $< 

depend: 
	@echo "Dependencies are automatically generated.  This target is not necessary."	

install:
	$(INSTALL) -m 0755 -d $(LIBDIR)/
	$(INSTALL) -m 0644 $(OBJPREFIX)$(LIB.SHARED) $(LIBDIR)/
	$(LN) $(LNFLAGS) $(LIB.SHARED) $(LIBDIR)/$(LIB.SONAME)
	$(LN) $(LNFLAGS) $(LIB.SHARED) $(LIBDIR)/$(LIB.DEVLNK)
	$(INSTALL) -m 0644 $(OBJPREFIX)$(LIB.STATIC) $(LIBDIR)/
	$(INSTALL) -m 0755 -d $(INCDIR)/
	$(INSTALL) -m 0644 $(LIB.HEADERS) $(INCDIR)/
	$(INSTALL) -m 0755 -d $(addprefix $(MANDIR)/, $(LIB.MDIRS))
	$(foreach page, $(LIB.MANPAGES), \
		$(INSTALL) -m 0444 $(page) $(addprefix $(MANDIR)/, \
		$(subst ., man, $(suffix $(page))))/; \
	)
	@set $(addprefix $(MANDIR)/, $(LIB.MLINKS)); \
	while : ; do \
		case $$# in \
			0) break;; \
			1) echo "Warning: Unbalanced MLINK: $$1"; break;; \
		esac; \
		page=$$1; shift; link=$$1; shift; \
		echo $(LN) $(LNFLAGS) $$page $$link; \
		$(LN) $(LNFLAGS) $$page $$link; \
	done
	
uninstall:
	$(RM) $(LIBDIR)/$(LIB.DEVLNK)
	$(RM) $(LIBDIR)/$(LIB.SONAME)
	$(RM) $(LIBDIR)/$(LIB.SHARED)
	$(RM) $(LIBDIR)/$(LIB.STATIC)
	$(RM) $(addprefix $(INCDIR)/, $(LIB.HEADERS))
	@set $(addprefix $(MANDIR)/, $(LIB.MLINKS)); \
	while : ; do \
		case $$# in \
			0) break;; \
			1) echo "Warning: Unbalanced MLINK: $$1"; break;; \
		esac; \
		page=$$1; shift; link=$$1; shift; \
		echo $(RM) $$link; \
		$(RM) $$link; \
	done
	$(foreach page, $(LIB.MANPAGES), \
		$(RM) $(addprefix $(MANDIR)/, \
		$(subst ., man, $(suffix $(page))))/$(page); \
	)
	
clean:
	$(RM) $(LIB.SOBJS)
	$(RM) $(OBJPREFIX)$(LIB.SHARED)
	$(RM) $(OBJPREFIX)/$(LIB.SONAME)
	$(RM) $(OBJPREFIX)/$(LIB.DEVLNK)
	$(RM) $(LIB.OBJS)
	$(RM) $(OBJPREFIX)$(LIB.STATIC)
	$(RM) $(LIB.DEPS)

-include $(LIB.DEPS)

.PHONY: clean depend install uninstall


#
# (C) Copyright 1992, ..., 2014 the "DOSEMU-Development-Team".
#
# for details see file COPYING.DOSEMU in the DOSEMU distribution
#

top_builddir=../../..
include ../plugins.mak
$(eval $(makefile_conf))

ALL_CFLAGS+=$(DL_CFLAGS) $(LIBAOCFL)
CFILES=snd_o_ao.c snd_o_ao_file.c ao_init.c
ifdef USE_DL_PLUGINS
all: $(BINPATH)/bin/libplugin_libao.so
ALL_CFLAGS += -fPIC
endif

include $(REALTOPDIR)/src/Makefile.common

ifdef USE_DL_PLUGINS
$(BINPATH)/bin/libplugin_libao.so: $(OBJS) | $(BINPATH)/bin
	$(CC) $(ALL_LDFLAGS) -shared -o $@ $^ $(LIBAOLIB)
endif

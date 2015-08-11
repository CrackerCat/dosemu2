# Makefile for DOSEMU
#

SHELL=/bin/bash

all: default configure src/include/config.h

srcdir=.
top_builddir=.
SUBDIR:=.
-include Makefile.conf

configure: configure.ac
	autoreconf -v

Makefile.conf: $(srcdir)/Makefile.conf.in $(srcdir)/configure $(srcdir)/default-configure
	@echo "Running $(srcdir)/default-configure ..."
	$(srcdir)/default-configure

default clean realclean install: config.status
	@$(MAKE) -C src $@

dosbin:
	@$(MAKE) SUBDIR:=commands -C src/commands dosbin

docs:
	@$(MAKE) SUBDIR:=doc -C src/doc all

docsinstall:
	@$(MAKE) SUBDIR:=doc -C src/doc install

docsclean:
	@$(MAKE) SUBDIR:=doc -C src/doc clean

$(PACKAGE_NAME).spec: $(PACKAGE_NAME).spec.in
	@$(MAKE) -C src ../$@

GIT_REV := .git/$(shell git rev-parse --symbolic-full-name HEAD)

$(PACKETNAME).tar.gz: $(GIT_REV) $(PACKAGE_NAME).spec
	rm -f $(PACKETNAME).tar.gz
	git archive -o $(PACKETNAME).tar --prefix=$(PACKETNAME)/ HEAD
	tar rf $(PACKETNAME).tar --add-file=$(PACKAGE_NAME).spec
	gzip $(PACKETNAME).tar

dist: $(PACKETNAME).tar.gz

rpm: $(PACKETNAME).tar.gz
	rpmbuild -tb $(PACKETNAME).tar.gz
	rm -f $(PACKETNAME).tar.gz

pristine distclean mrproper:  docsclean
	@$(MAKE) -C src pristine
	rm -f Makefile.conf $(PACKAGE_NAME).spec
	rm -f $(PACKETNAME).tar.gz
	rm -f core `find . -name config.cache`
	rm -f core `find . -name config.status`
	rm -f core `find . -name config.log`
	rm -f core `find . -name configure.lineno`
	rm -f src/include/config.h
	rm -f src/include/confpath.h
	rm -f src/include/version.h
	rm -f src/include/plugin_*.h
	rm -f core `find . -name '*~'`
	rm -f core `find . -name '*[\.]o'`
	rm -f core `find . -name '*.d'`
	rm -f core `find . -name '*[\.]orig'`
	rm -f core `find . -name '*[\.]rej'`
	rm -f core gen*.log
	rm -f man/dosemu.1 man/dosemu.bin.1 man/ru/dosemu.1 man/ru/dosemu.bin.1
	rm -rf autom4te*.cache
	rm -f config.sub config.guess
	$(srcdir)/mkpluginhooks clean

tar: distclean
	VERSION=`cat VERSION` && cd .. && tar czvf dosemu-$$VERSION.tgz dosemu-$$VERSION

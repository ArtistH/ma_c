# Top level makefile, the real shit is at src/Makefile

default: all

.DEFAULT:
	cd src && $(MAKE) $@

install:
	cd src && $(MAKE) $@

GTAGS:
	find . -type f -name "*.[ch|cpp]" > gtags.files
	gtags

.PHONY: install GTAGS

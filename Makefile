CFLAGS += -Wall -Wextra -Wno-unused-parameter -O3
LDFLAGS += -lconfig
__NAME__ = lariza
__NAME_UPPERCASE__ = `echo $(__NAME__) | sed 's/.*/\U&/'`
__NAME_CAPITALIZED__ = `echo $(__NAME__) | sed 's/^./\U&\E/'`

INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

DESTDIR = /usr/local
bindir = $(DESTDIR)/bin
datadir = $(DESTDIR)/share/$(__NAME__)
mandir = $(DESTDIR)/man


.PHONY: all clean install installdirs

all: $(__NAME__) we_adblock.so

$(__NAME__): browser.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-D__NAME__=\"$(__NAME__)\" \
		-D__NAME_UPPERCASE__=\"$(__NAME_UPPERCASE__)\" \
		-D__NAME_CAPITALIZED__=\"$(__NAME_CAPITALIZED__)\" \
		-o $@ $< \
		`pkg-config --cflags --libs gtk+-3.0 glib-2.0 webkit2gtk-4.0 libconfig`

we_adblock.so: we_adblock.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-D__NAME__=\"$(__NAME__)\" \
		-D__NAME_UPPERCASE__=\"$(__NAME_UPPERCASE__)\" \
		-D__NAME_CAPITALIZED__=\"$(__NAME_CAPITALIZED__)\" \
		-shared -o $@ -fPIC $< \
		`pkg-config --cflags --libs glib-2.0 webkit2gtk-4.0`

install: all installdirs
	$(INSTALL_PROGRAM) $(__NAME__) $(bindir)/$(__NAME__)
	$(INSTALL_PROGRAM) we_adblock.so $(datadir)/we_adblock.so
	$(INSTALL_DATA) $(__NAME__).cfg $(datadir)/$(__NAME__).cfg.example
	$(INSTALL_DATA) man1/$(__NAME__).1 $(mandir)/man1/$(__NAME__).1
	$(INSTALL_DATA) man1/$(__NAME__).usage.1 $(mandir)/man1/$(__NAME__).usage.1
	$(INSTALL_DATA) man1/$(__NAME__).cfg.5 $(mandir)/man5/$(__NAME__).cfg.5

installdirs:
	mkdir -p $(bindir) $(datadir) $(mandir)/man1 $(mandir)/man5

clean:
	rm -f $(__NAME__) we_adblock.so

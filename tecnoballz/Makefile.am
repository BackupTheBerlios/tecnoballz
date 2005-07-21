
gamesdir = $(prefix)/games
scoredir = /var/games/tecnoballz
score = tecnoballz.hi

SUBDIRS = src 

pkgdatadir = $(mandir)/man6
dist_pkgdata_DATA = tecnoballz.6

install-data-hook:
	-chown root:games "$(DESTDIR)/$(gamesdir)/tecnoballz"
	-chmod 2755 "$(DESTDIR)/$(gamesdir)/tecnoballz"
	$(INSTALL) -d $(DESTDIR)/$(scoredir)
	$(INSTALL) $(score) $(DESTDIR)/$(scoredir) 
	-chown root:games "$(DESTDIR)/$(scoredir)/$(score)"
	-chmod 664 "$(DESTDIR)/$(scoredir)/$(score)"


CC       = gcc
CXX      = g++
#CXXFLAGS = -O3 -Wall -s
CXXFLAGS = -O3 -Wall
PROG     = tecnoballz
PREFIX   = /usr/local
LIBPATH  = -L/usr/X11R6/lib -L.
TMPDIR   = /var/tmp/tecnoballz.dist   
ifeq ($(PREFIX),)
EXEDIR   = $(HOME)/bin
MANDIR   = $(HOME)/man
DATADIR  = $(HOME)/.tecnoballz
SCOREDIR = $(DATADIR)
else
EXEDIR   = $(PREFIX)/games
MANDIR   = $(PREFIX)/share/man
DATADIR  = $(PREFIX)/share/games/tecnoballz
SCOREDIR = /var/games
endif

MODC =	audiomixer \
	ballDirect \
	barreScore \
	BOB_killer \
	briqueCote \
	clavierMac \
	ecran_hard \
	ejectBalls \
	escapeMenu \
	fond_ecran \
	gard_tecno \
	GFX_bitmap \
	GIF_bitMap \
	giga_blitz \
	head_anima \
	joueurData \
	lastScroll \
	lesBriques \
	level_data \
	liste_BOBs \
	main \
	mentatCode \
	menu_tecno \
	mousePoint \
	print_menu \
	printmoney \
	print_text \
	RAM_killer \
	ressources \
	runGestion \
	score_over \
	scoretable \
	scrolledit \
	shop_tecno \
	tableaux_Z \
	technoBall \
	techno_eye \
	techno_gem \
	tecno_boui \
	tecno_bump \
	tecno_caps \
	tecnoDefil \
	tecno_fire \
	tecno_gads \
	tecno_gard \
	tecno_miss \
	tecno_text \
	zeBouiBoui \
	zeCapsules \
	zeCongBall \
	zeDefilTxt \
	zeFireBump \
	ze_gadgets \
	zeGameOver \
	zeGemstone \
	zeGigaBlit \
	zeguardian \
	ze_magneye \
	zeMiniMess \
	zeMissiles \
	zeMoveText \
	zeNewBalls \
	zeRaquette \
	zexplosion

MODULES = $(MODC) 
OBJ     = $(addsuffix .o,   $(MODULES))
SRCC    = $(addsuffix , $(MODC))

all: $(PROG)

.o: 
	$(CXX) $(CXXFLAGS) -DPROG=\"$(PROG)\" -o $@ -c $(DEFINES) $<


$(PROG): $(OBJ)
	$(CXX) $(LIBPATH) $(OBJ) $(PROFLAG) -lm \
		-lSDL -lSDL_mixer -lmikmod -o $@

clean:
	rm -f $(OBJ)  nomfichierscore.o

cleanall:
	rm -f $(PROG) $(OBJ)  nomfichierscore




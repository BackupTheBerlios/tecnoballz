
NULL =
gamesdir = $(prefix)/games
scoredir = /var/games/tecnoballz
score = tecnoballz.hi

SUBDIRS = TecnoballZ 

games_PROGRAMS = tecnoballz
tecnoballz_SOURCES = $(SOURCES_MAIN) 
tecnoballz_CXXFLAGS = -DDATADIR=\"$(datadir)/tecnoballz\" -DSCOREFILE=\"$(scoredir)/$(score)\" @XLIB_CFLAGS@ @SDL_CFLAGS@ @MIKMOD_FLAGS@ 
tecnoballz_LDADD = @XLIB_LIBS@ @SDL_LIBS@ @MM_LIBS@ @MIKMOD_LIBS@ -L.

SOURCES_MAIN = \
	ballDirect.cc \
	barreScore.cc \
	briqueCote.cc \
	configfile.cc \
	ejectBalls.cc \
	escapeMenu.cc \
	fond_ecran.cc \
	gard_tecno.cc \
	GFX_bitmap.cc \
	GIF_bitMap.cc \
  handler_audio.cc \
  handler_display.cc \
	handler_keyboard.cc \
	handler_memory.cc \
	giga_blitz.cc \
	head_anima.cc \
	joueurData.cc \
	lastScroll.cc \
	lesBriques.cc \
	level_data.cc \
	lispreader.cc \
  list_sprites.cc \
	main.cc \
	mentatCode.cc \
	menu_tecno.cc \
	mousePoint.cc \
	print_menu.cc \
	printmoney.cc \
	print_text.cc \
	ressources.cc \
	runGestion.cc \
	score_over.cc \
	scoretable.cc \
	scrolledit.cc \
	shop_tecno.cc \
	sprite_object.cc \
	tableaux_Z.cc \
	technoBall.cc \
	techno_eye.cc \
	techno_gem.cc \
	tecno_boui.cc \
	tecno_bump.cc \
	tecno_caps.cc \
	tecnoDefil.cc \
	tecno_fire.cc \
	tecno_gads.cc \
	tecno_gard.cc \
	tecno_miss.cc \
	tecno_text.cc \
	zeBouiBoui.cc \
	zeCapsules.cc \
	zeCongBall.cc \
	zeDefilTxt.cc \
	zeFireBump.cc \
	ze_gadgets.cc \
	zeGameOver.cc \
	zeGemstone.cc \
	zeGigaBlit.cc \
	zeguardian.cc \
	ze_magneye.cc \
	zeMiniMess.cc \
	zeMissiles.cc \
	zeMoveText.cc \
	zeNewBalls.cc \
	zeRaquette.cc \
	zexplosion.cc \
	$(NULL)


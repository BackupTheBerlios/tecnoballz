
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
	bitmap_data.cc \
	briqueCote.cc \
	configfile.cc \
	controller_balls.cc \
	controller_bricks.cc \
	ejectBalls.cc \
  handler_audio.cc \
  handler_display.cc \
	handler_keyboard.cc \
	handler_memory.cc \
	handler_popup_menu.cc \
	handler_resources.cc \
	giga_blitz.cc \
	head_anima.cc \
	joueurData.cc \
	lastScroll.cc \
	level_data.cc \
	lispreader.cc \
  list_sprites.cc \
	main.cc \
	mousePoint.cc \
	offscreen_surface.cc \
	print_menu.cc \
	printmoney.cc \
	print_text.cc \
	score_over.cc \
	scoretable.cc \
	surface_sdl.cc \
	sprite_object.cc \
	sprite_ball.cc \
  sprite_projectile.cc \
  sprite_paddle.cc \
  sprite_ship.cc \
  supervisor.c \
  supervisor_bricks_level.cc \
  supervisor_main_menu.cc \
  supervisor_map_editor.cc \
  supervisor_guards_level.cc \
  supervisor_shop.cc \
	techno_eye.cc \
	techno_gem.cc \
	tecno_caps.cc \
	tecnoDefil.cc \
	tecno_gads.cc \
	tecno_gard.cc \
	tecno_miss.cc \
	tecno_text.cc \
  tecnoballz.cc \
	tiles_background.cc \
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
	zeRaquette.cc \
	zexplosion.cc \
	$(NULL)


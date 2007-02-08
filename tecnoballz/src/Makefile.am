
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
	barreScore.cc \
	bitmap_data.cc \
	briqueCote.cc \
	configfile.cc \
	controller_balls.cc \
	controller_bricks.cc \
  controller_bullets.cc \
  controller_capsules.cc \
  controller_explosions.cc \
  controller_game_over.cc \
  controller_gigablitz.cc \
	controller_guardians.cc \
  controller_paddles.cc \
  controller_projectiles.cc \
	controller_viewfinders.cc \
	ejectBalls.cc \
  handler_audio.cc \
  handler_display.cc \
	handler_keyboard.cc \
	handler_memory.cc \
	handler_players.cc \
	handler_popup_menu.cc \
	handler_resources.cc \
	head_anima.cc \
	level_data.cc \
	lispreader.cc \
  list_sprites.cc \
	main.cc \
	offscreen_surface.cc \
	print_menu.cc \
	printmoney.cc \
	print_text.cc \
	score_over.cc \
	scoretable.cc \
	surface_sdl.cc \
	sprite_ball.cc \
	sprite_bullet.cc \
	sprite_capsule.cc \
  sprite_eye.cc \
	sprite_fonte_game.cc \
  sprite_fonte_menu.cc \
  sprite_gem.cc \
  sprite_gigablitz.cc \
	sprite_guardian.cc \
  sprite_money.cc \
  sprite_mouse_pointer.cc \
	sprite_object.cc \
  sprite_paddle.cc \
  sprite_projectile.cc \
  sprite_ship.cc \
  supervisor.c \
  supervisor_bricks_level.cc \
  supervisor_main_menu.cc \
  supervisor_map_editor.cc \
  supervisor_guards_level.cc \
  supervisor_shop.cc \
  tecnoballz.cc \
	tiles_background.cc \
	tilesmap_scrolling.cc \
	zeBouiBoui.cc \
	zeCapsules.cc \
	zeCongBall.cc \
	zeDefilTxt.cc \
	zeGemstone.cc \
	ze_magneye.cc \
  zeMiniMess.cc \
	zeMoveText.cc \
	$(NULL)



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
  bitmap_data.cc \
  configfile.cc \
  controller_balls.cc \
  controller_bricks.cc \
  controller_bullets.cc \
  controller_capsules.cc \
  controller_ejectors.cc \
  controller_explosions.cc \
  controller_fontes_game.cc \
  controller_fontes_menu.cc \
  controller_game_over.cc \
  controller_gems.cc \
  controller_gigablitz.cc \
  controller_guardians.cc \
  controller_indicators.cc \
  controller_magnetic_eyes.cc \
  controller_moneys.cc \
  controller_paddles.cc \
  controller_projectiles.cc \
  controller_ships.cc \
  controller_sides_bricks.cc \
  controller_spheres.cc \
  controller_viewfinders.cc \
  display_text_bitmap.cc \
  handler_audio.cc \
  handler_display.cc \
	handler_high_score.cc \
  handler_keyboard.cc \
  handler_memory.cc \
  handler_players.cc \
  handler_popup_menu.cc \
  handler_resources.cc \
  head_animation.cc \
  handler_levels.cc \
  lispreader.cc \
  list_sprites.cc \
  main.cc \
  offscreen_surface.cc \
  right_panel_score.cc \
  short_info_messages.cc \
  sprite_ball.cc \
  sprite_bullet.cc \
  sprite_capsule.cc \
  sprite_display_menu.cc \
  sprite_display_scores.cc \
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
  surface_sdl.cc \
  tecnoballz.cc \
  tiles_background.cc \
  tilesmap_scrolling.cc \
  tinyxml.cc \
  tinyxmlerror.cc \
  tinyxmlparser.cc \
  tinystr.cc
  $(NULL)



AC_PREREQ(2.50)
dnl AC_INIT(src/technoBall.cc)
AC_INIT(tecnoballz, 0.91-cvs20050828, tecnoballz@tlk.fr)
AC_CONFIG_AUX_DIR(autotools)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(tecnoballz,0.91-cvs20050828)
AM_CONFIG_HEADER(config.h)

dnl  Check our build tools
AC_PROG_CC
AC_PROG_CXX

dnl  Default flags
CXXFLAGS="-O3 -Wall"

dnl  Check for X
AC_PATH_X
x_includes="/usr/X11R6/includes"
x_libraries="/usr/X11R6/lib"
XLIB_CFLAGS="-I${x_includes}"
XLIB_LIBS="-L${x_libraries} -lXext -lXxf86dga -lXxf86vm"
AC_SUBST(XLIB_CFLAGS)
AC_SUBST(XLIB_LIBS)

dnl  Check for SDL
AM_PATH_SDL

dnl  Check for SDL_mixer
LDFLAGS_save="${LDFLAGS} ${SDL_LIBS}"
LDFLAGS="${LDFLAGS} ${SDL_LIBS}"
AC_CHECK_LIB(SDL_mixer, Mix_GetChunk, [
  SDL_LIBS="-lSDL_mixer ${SDL_LIBS}"
],[
  AC_MSG_ERROR(Could not find -lSDL_mixer)
])
LDFLAGS_="${LDFLAGS_save}"

AC_CHECK_LIB(vorbisfile, ov_raw_seek, [
	MM_LIBS="${MM_LIBS} -lvorbisfile"
],[
	AC_MSG_ERROR(Could not find the vorbisfile lib : -lvorbisfile)
])
AC_CHECK_LIB(ogg, ogg_stream_packetin, [
	MM_LIBS="${MM_LIBS} -logg"
],[
	AC_MSG_ERROR(Could not find the ogg lib : -logg)
])
AC_CHECK_LIB(smpeg, SMPEG_play, [
	MM_LIBS="${MM_LIBS} -lsmpeg"
],[
	AC_MSG_ERROR(Could not find the smpeg lib : -lsmpeg)
])
AC_SUBST(MM_LIBS)

AC_CHECK_LIB(mikmod, Player_Stop, [
	MIKMOD_LIBS="-lmikmod"
	MIKMOD_FLAGS="-pthread -D_REENTRANT"
],[
	AC_MSG_ERROR(Could not find the Mikmod library : -lmikmod)
])
AC_SUBST(MIKMOD_FLAGS)
AC_SUBST(MIKMOD_LIBS)

dnl  Output makefiles
AC_OUTPUT(Makefile src/Makefile src/TecnoballZ/Makefile src/TecnoballZ/hires/Makefile src/TecnoballZ/lores/Makefile src/TecnoballZ/textures/Makefile src/TecnoballZ/musics/Makefile src/TecnoballZ/sounds/Makefile)


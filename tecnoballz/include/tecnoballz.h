/** 
 * @file tecnoballz.h
 * @brief Base of all classes, and main static methods of the game 
 * @created 2002-08-18
 * @date 2007-03-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.20 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tecnoballz.h,v 1.20 2007/03/06 17:42:43 gurumeditation Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef WIN32
#include <io.h>
#include "SDL/SDL.h"
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <pthread.h>
#include <unistd.h>
#include "SDL/SDL.h"
#endif
#include "../include/configfile.h"
#ifndef __TECNOBALLZ__
#define __TECNOBALLZ__
#define TECNOBALLZ_VERSION	"TECNOBALLZ 0.91+ (2007-03-06)"
/** To define only under development  */
#define UNDER_DEVELOPMENT
/** Force bytes copy (SPARC unaligned memory access) */
#define BYTES_COPY
#ifndef SCOREFILE
#define SCOREFILE "tecnoball"
#endif
#ifndef PREFIX
#define PREFIX ""
#endif

class tecnoballz;
class configfile;
class scoretable;
class handler_resources;
class handler_levels;
class handler_memory;
class handler_display;
class handler_keyboard;
#ifndef SOUNDISOFF
class handler_audio;
#endif
class list_sprites;
class handler_players;
class supervisor_bricks_level;
class supervisor_shop;
class supervisor_guards_level;
class supervisor_main_menu;
class supervisor_map_editor;
class bitmap_data;
class offscreen_surface;

const Sint32 SINUS_MASK = 511;
const Sint32 SINUS_DECA = 7;
class tecnoballz
{
public:

  /** Different phases of the game */
  typedef enum
  {
    BRICKS_LEVEL = 1,
    SHOP,
    GUARDS_LEVEL,
    MAIN_MENU,
    MAP_EDITOR,
    LEAVE_TECNOBALLZ,
  }
  GAME_PHASE;

  typedef enum 
  {
    DIFFICULTY_EASY = 1,
    DIFFICULTY_NORMAL,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD,
  }
  DIFFICULTIES_ENUM;

public:
  /** 4 levels of difficulty */
  static const Uint32 NDIFFICULT = 4;
  static const Sint32 MAX_PLAYER = 6;

public:
  /** True if verbose enable */
  static bool is_verbose;
  static Sint32 bg4_colors;     //1 = force 4 colors background
  static Sint32 resolution;     //1:320*240 or 2:640*480 pixels
  static bool bob_ground;       //1 = background is sprites 
  static Uint32 double_mem;     //double memory size of all allocations 
  static Sint32 arg_jumper;

protected:
  Sint32 numero_obj;          //object number
  Sint32 erreur_num;            //error code

  /*
   * static members
   */

  /** Current number of objects */
  static Uint32 objects_counter;
  /** Counter the number of frames */
  static Uint32 frame_counter;
  static Sint32 num_erreur;     //error number
  /** Random counting variable */
  static Sint32 random_counter;
  /** Current phase of the game
   * BRICKS_LEVEL, SHOP, GUARDS_LEVEL,
   * MAIN_MENU, or MAP_EDITOR */
  static Uint32 super_jump;
  /** Difficulty with which an average player may complete a game
   * 1: easy; 2:normal 3:medium; 4:hard */
  static Sint32 difficulty_level;
  /** If true then leaves the game definitively */
  static bool is_exit_game;

  static Uint32 cheat_flag;     //E + T + B + Return (into shop)
  static Uint32 birth_flag;     //1 = all names is 040670
  /** Number of initial lifes */
  static Sint32 initial_num_of_lifes;
  /** Number of players from 1 to 6 */
  static Sint32 number_of_players;
  static const char nomprefix[];

  static scoretable *ptScoreTab;
  /** Handler of the files resources */
  static handler_resources *resources;
  static handler_levels *ptLev_data;
  static handler_memory *memory;
#ifndef SOUNDISOFF
  static handler_audio *audio;
#endif
  static handler_display *display;
  static handler_keyboard *keyboard;
  static list_sprites *sprites;
  static handler_players *current_player;

  static Sint16 *table_cosL;
  static Sint16 *table_sinL;
  static supervisor_bricks_level *bricks_level;
  static supervisor_shop *shop;
  static supervisor_guards_level *guards_level;
  static supervisor_main_menu *main_menu;
  static supervisor_map_editor *map_editor;
  /** A bitmap containing the current images of sprites */
  static bitmap_data *sprites_bitmap;

  static offscreen_surface *game_screen;
  static offscreen_surface *background_screen;
  static configfile *config_file;

public:
  static Sint32 first_init (configfile * pConf);
  static void game_begin ();
  static void release_objects ();
  static void release_all_objects (configfile * pConf);

  static void ecritLog (char *f_nom, char *chain);
  static void integer_to_ascii (Sint32 value, Uint32 padding, char *str); 
  static void intToASCII (Sint32 value, char *strng, Uint32 reste);

    tecnoballz ();
   ~tecnoballz ();
  void object_init ();
  void object_free ();
  Sint32 retour_err ();
  void error_init (Sint32 error);
  Sint32 get_number ();
  Sint32 chaine_cmp (char *srcPT, char *desPT, Sint32 taille);
  Sint32 littleWord (char *memPT);
  void bigendianw (Uint32 *, Uint32 *);
  void bigendianr (Uint32 *, Uint32 *);
};
#endif

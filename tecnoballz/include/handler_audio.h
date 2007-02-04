/** 
 * @file handler_audio.h
 * @brief Handler of the sound and music
 * @created 2004-03-22
 * @date 2007-01-30
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_audio.h,v 1.3 2007/02/04 17:10:16 gurumeditation Exp $
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
//#define SOUNDISOFF if define, don't compile handler_audio.cc
#ifndef SOUNDISOFF
#ifndef __AUDIOMIXER__
#define __AUDIOMIXER__
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <mikmod.h>
class handler_audio;
#include "tecnoballz.h"

/* numbers of the music */
typedef enum
{
  MUSICAREA1,
  MUSICAREA2,
  MUSICAREA3,
  MUSICAREA4,
  MUSICAREA5,
  MUSICGUARD,
  MUSICSCORE,
  MUSICGOVER,
  MUSICINTRO,
  MUSICCONGR
} music_code;

// module's positions in a area music
typedef struct
{
  Uint32 pos_music1;
  Uint32 loopmusic1;
  Uint32 pos_music2;
  Uint32 loopmusic2;
  Uint32 pos_winner;
  Uint32 pos_losing;
  Uint32 pos_zeshop;
} musics_pos;

// numbers of the sounds
typedef enum
{ S_ENLEVVIE,                   //lose a life
  S_RAKEXPLO,                   //explosion of the bumper
  S_AJOUTVIE,                   //extra life
  S_MONNAIES,                   //collect money
  S_TILTALAR,                   //alarm 
  S_GADGETGO,                   //collect option (extra balls, power balls)
  S_TECNOBAL,                   //"tecno" sound
  S_ATOMAPPA,
  S_TRANSFOR,                   //bumper transformation (expand, glue)
  S_TIR_GARD,                   //guard's fire
  S_RAK_TIRS,                   //bumper's fire
  S_BIGEXPLO,                   //guard's explosion
  S_CASSEIND,                   //break a indestructible brick
  S_ATOM_EXP,                   //
  S_GARDIENT,                   //hit the guard
  S_TO_ATOMS,                   //
  S_TOINDES1,                   //hit the indestructible brick 1
  S_TOINDES2,                   //hit the indestructible brick 2
  S_TOUCHRAK,
  S_BRICOTES,
  S_TOUBRIK1,
  S_TOUBRIK2,
  S_TOUBRIK3,
  S_TOUBRIK4,
  S_TOUBRIK5,
  S_COINEJEC,
  S_COINASPI,
  NUM_OF_SOUNDS
} sound_code;

class handler_audio:public virtual tecnoballz
{

public:
/* differents portions in a area music */
typedef enum
{ MUSIC_UNDIVIDED,
  GAME_PORTION,
  WIN_PORTION,
  LOST_PORTION,
  SHOP_PORTION,
  MUSIC_IS_OFF
} MUSIC_PORTIONS;

public:
  /* true if SDL_mixer is enable */
  static bool is_audio_enable;

private:
    handler_audio ();
public:
   ~handler_audio ();
  static handler_audio *get_instance ();
  void stop_music ();
  void play_music (Uint32 music_id);
  void run ();
  Sint32 play_level_music (Uint32 area_num, Uint32 level_num);
  Sint32 play_shop_music (Uint32 area_num);
  void play_win_music ();
  void play_lost_music ();
  void stop_lost_music ();
  bool is_win_music_finished ();
  Uint32 get_portion_music_played ();
  void play_sound (Uint32 sound_num);
  void disable_sound ();
  void enable_sound ();

private:
  void query_spec ();
  Uint32 area_music (Uint32);
  void initialize ();
  void control_music_position ();
  void play_requested_sounds ();

private:
  static handler_audio *audio_singleton;
  static char sounds_play[NUM_OF_SOUNDS];
  static Mix_Chunk *sound_list[];
  static const musics_pos ptMusicpos[];

private:
  /** Current area number (1 to 5) */
  Uint32 aera_number;
  /** Current level number (1 to 12) */
  Uint32 level_number;

  Sint32 position_1;
  Sint32 modposloop;
  Sint32 position_2;
  /** */
  Uint32 current_portion_music;            //normal / level / shop / ...

  /** True if only the music must be played,
   * and not the sound effects, during the phases of intro,
   * shop and game over */
  bool is_only_music;
  /* True if the music is enabled
   * [Ctrl] + [D] or [Ctrl +[S] keys toggle the music on and off */
  bool is_music_enable;
  /** True if sound effect are enabled. [Ctrl] + [F] or
   * [Ctrl] + [S] keys toggle the sound effects on and off */
  bool is_sound_enable;

  Uint32 music_volume;
  /** Identifier of the current music loaded and played */
  Sint32 current_music_id;
  Mix_Music *current_music;
  /** Amiga song module */
  MODULE *song_module;
  Uint32 waves_size;            //size fo all waves
  Sint32 song_pos;
  Sint32 lastpatpos;

};
#endif
#endif

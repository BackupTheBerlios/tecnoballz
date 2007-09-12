/** 
 * @file handler_audio.cc 
 * @brief Handler of the sound and music
 * @created 2004-03-22
 * @date 2007-03-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_audio.cc,v 1.7 2007/09/12 06:32:48 gurumeditation Exp $
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include "../include/handler_audio.h"
#ifndef SOUNDISOFF
#include "../include/handler_resources.h"
#include "../include/handler_keyboard.h"

handler_audio * handler_audio::audio_singleton = NULL;
bool handler_audio::is_audio_enable = true;

/** Positions in music modules */
const musics_pos
  handler_audio::ptMusicpos[] = { {0,   // first music of a "bricks level"
                                   2,   // restart first music

                                   11,  // second music of a "bricks level"
                                   11,  // restart second music

                                   23,  // "bricks level" completed
                                   24,  // lost ball in "bricks level"
                                   25   // shop music
                                   },
{0, 0, 15, 15, 22, 23, 24},
{0, 0, 15, 15, 28, 29, 30},
{0, 0, 11, 11, 18, 19, 20},
{0, 0, 15, 15, 30, 31, 32}
};

//######################################################################
// list of sounds pointers
//######################################################################
Mix_Chunk * handler_audio::sound_list[NUM_OF_SOUNDS];
char handler_audio::sounds_play[NUM_OF_SOUNDS];

/**
 * Create the handler_audoi singleton object, and clear members
 */
handler_audio::handler_audio ()
{
  object_init ();
  initialize ();
  aera_number = 0;
  level_number = 0;
  song_module = NULL;
  is_only_music = true;
  is_music_enable = true;
  is_sound_enable = true;
}

/**
 * Release the handler_keyboard singleton object
 */
handler_audio::~handler_audio ()
{
  if (NULL != song_module)
    {
      Mix_HaltMusic ();
      Mix_FreeMusic (current_music);
      song_module = NULL;
    }

  /* release the samples */
  for (Sint32 i = 0; i < NUM_OF_SOUNDS; i++)
    {
      if (sound_list[i])
        {
          Mix_FreeChunk (sound_list[i]);
          sound_list[i] = NULL;
        }
      sounds_play[i] = false;
    }

  if (is_audio_enable)
    {
      Mix_CloseAudio ();
      SDL_Quit ();
    }
  audio_singleton = NULL;
}

/**
 * Get the object instance
 * handler_audio is a singleton
 * @return the handler_audoi object 
 */
handler_audio *
handler_audio::get_instance ()
{
  if (NULL == audio_singleton)
    {
      audio_singleton = new handler_audio ();
    }
  return audio_singleton;
}

/**
 *  Initialize SDL audio and load files waves in memory 
 */
void
handler_audio::initialize ()
{
  if (!is_audio_enable)
    {
      if (is_verbose)
        {
          std::cout << "handler_audio::initialize() " <<
            "audio disable!" << std::endl;
        }
        return;
    }
  if (SDL_Init (SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
    {
      std::cerr << "handler_audio::initialize() " <<
        "SDL_Init() return " << SDL_GetError () << std::endl;
      is_audio_enable = false;
      return;
    }
  if (Mix_OpenAudio (44100, AUDIO_S16, 2, 4096))
    {
      std::cerr << "handler_audio::initialize() " <<
        "Mix_OpenAudio() return " << SDL_GetError () << std::endl;
      is_audio_enable = false;
      SDL_Quit ();
      return;
    }
  //query_spec();
  music_volume = Mix_VolumeMusic (-1);
  Mix_AllocateChannels (8);

  /* load files waves in memory */
  for (Sint32 i = 0; i < NUM_OF_SOUNDS; i++)
    {
      sound_list[i] = NULL;
    }
  waves_size = 0;
  for (Sint32 i = 0; i < NUM_OF_SOUNDS; i++)
    {
      char *pathname = resources->get_sound_filename (i);
      if (NULL == pathname)
        {
          fprintf (stderr,
                   "handler_audio::initialize() file %i not found\n", i);
          is_audio_enable = false;
          return;
        }

      Mix_Chunk *ptWav = Mix_LoadWAV (pathname);
      if (NULL == ptWav)
        {
          std::cerr << "handler_audio::initialize() " << 
            "Mix_LoadWAV(" << pathname << ") return " <<
            SDL_GetError () << std::endl;
          is_audio_enable = false;
          return;
        }
      Mix_VolumeChunk (ptWav, 32);
      sound_list[i] = ptWav;
      waves_size += ptWav->alen;
    }
  is_audio_enable = true;
  is_only_music = false;
  if (is_verbose)
    {
      std::cout << "handler_audio::initialize() initialize succeded" 
        << std::endl;
    }
}

//------------------------------------------------------------------------------
// display some infos
//------------------------------------------------------------------------------
void
handler_audio::query_spec ()
{
  Sint32 nopen, frequ, nchan;
  Uint16 fmNum;
  nopen = Mix_QuerySpec (&frequ, &fmNum, &nchan);
  if (!nopen)
    fprintf (stderr, "Mix_QuerySpec: %s\n", Mix_GetError ());
  else
    {
      char *fmStr = "Unknown";
      switch (fmNum)
        {
        case AUDIO_U8:
          fmStr = "U8";
          break;
        case AUDIO_S8:
          fmStr = "S8";
          break;
        case AUDIO_U16LSB:
          fmStr = "U16LSB";
          break;
        case AUDIO_S16LSB:
          fmStr = "S16LSB";
          break;
        case AUDIO_U16MSB:
          fmStr = "U16MSB";
          break;
        case AUDIO_S16MSB:
          fmStr = "S16MSB";
          break;
        }
      printf ("opened=%d times frequency=%dHz format=%s channels=%d",
              nopen, frequ, fmStr, nchan);
    }
}

/**
 * Play the music of a bricks level
 * @param aera_num area number 1 to 5
 * @param level level number 1 to 12
 * @return error code, 0 if no error
 */
void
handler_audio::play_level_music (Uint32 aera_num, Uint32 level)
{
  if (!is_audio_enable)
    {
      return;
    }
  aera_number = aera_num;
  level_number = level;
  Uint32 music = area_music (aera_num);
  Uint32 paire = level & 0x1;
  if ((level <= 5 && paire) || (level > 5 && !paire))
    {
      modposloop = ptMusicpos[music].loopmusic1;
      position_1 = ptMusicpos[music].pos_music1;
      position_2 = ptMusicpos[music].pos_music2 - 1;
    }
  else
    {
      modposloop = ptMusicpos[music].loopmusic2;
      position_1 = ptMusicpos[music].pos_music2;
      position_2 = ptMusicpos[music].pos_winner - 1;
    }
  play_music (music);
  Mix_SetMusicPosition (position_1);
  current_portion_music = GAME_PORTION;
  Player_Stop ();
}

/**
 * Play the music of the shop
 * @param aera_num area number
 */
void
handler_audio::play_shop_music (Uint32 aera_num)
{
  if (!is_audio_enable || NULL == song_module)
    {
      return;
    }
  aera_number = aera_num;
  Uint32 music = area_music (aera_num);
  position_1 = ptMusicpos[music].pos_zeshop;
  position_2 = song_module->numpos - 1;
  modposloop = position_1;
  play_music (music);
  Mix_SetMusicPosition (position_1);
  current_portion_music = SHOP_PORTION;
}

/**
 * Play the music of the victory, when the player finished a level
 */
void
handler_audio::play_win_music ()
{
  if (!is_audio_enable)
    {
      return;
    }
  Uint32 music = area_music (aera_number);
  position_1 = ptMusicpos[music].pos_winner;
  position_2 = ptMusicpos[music].pos_losing - 1;
  modposloop = position_1;
  Mix_SetMusicPosition (position_1);
  current_portion_music = WIN_PORTION;
}

/**
 * Play the music of the defeat, when the player loses a life
 */
void
handler_audio::play_lost_music ()
{
  if (!is_audio_enable || current_portion_music == LOST_PORTION)
    {
      return;
    }
  Uint32 music = area_music (aera_number);
  position_1 = ptMusicpos[music].pos_losing;
  position_2 = ptMusicpos[music].pos_zeshop - 1;
  modposloop = position_1;
  Mix_SetMusicPosition (position_1);
  current_portion_music = LOST_PORTION;
}

/**
 * Stop the music of the defeat, when the player loses a life
 */
void
handler_audio::stop_lost_music ()
{
  if (!is_audio_enable || current_portion_music != LOST_PORTION)
    {
      return;
    }
  play_level_music (aera_number, level_number);
}

/**
 * Check if the music of the victory is finished
 * @return true if the music of the victory is finished
 */
bool
handler_audio::is_win_music_finished ()
{
  if (!is_audio_enable || NULL == song_module)
    {
      return true;
    }
  if (WIN_PORTION == current_portion_music)
    {
      return false;
    }
  else
    {
      return true;
    }
}

/**
 * If a music is played, it is stopped
 */
void
handler_audio::stop_music ()
{
  if (!is_audio_enable || NULL == song_module)
    {
      return;
    }
  Mix_HaltMusic ();
  Mix_FreeMusic (current_music);
  song_module = NULL;
  current_music_id = -1;
}

/** 
 * Play a music module
 * @param music_id resource identifier of the music module
 */
void
handler_audio::play_music (Uint32 music_id)
{
  if (!is_audio_enable)
    {
      return;
    }

  /* if a music is played, it is stopped */
  if (NULL != song_module )
    {
      if (current_music_id == (Sint32) music_id)
        {
          return;
        }
      else
        {
          stop_music ();
        }
    }
  char *pathname = resources->get_music_filename (music_id);
  if (is_verbose)
    {
       std::cout << "handler_audio::play_music() " <<
         "try load pathname '" << pathname << "'" << std::endl;
    }

  /* load the music in memory */
  current_music = Mix_LoadMUS (pathname);
  if (NULL == current_music)
    {
       std::cerr << "handler_audio::play_music() " <<
         "Mix_LoadMUS return " << SDL_GetError () << std::endl;
      return;
    }
  /* Ugly way to access sdl-mixer's internal structures */
  union
  {
    int i;
    void *p;
  } dummy;
  memcpy (&song_module, (Uint8 *) current_music + sizeof (dummy),
          sizeof (void *));

  /* start the music */ 
  if (Mix_PlayMusic (current_music, -1) == -1)
    {
      std::cerr << "(!)handler_audio::play_music() " <<
        SDL_GetError () << std::endl;
      return;
    }

  current_portion_music = MUSIC_UNDIVIDED;
  song_pos = -1;
  lastpatpos = -1;
  current_music_id = music_id;
  if (is_verbose)
    {
      std::cout << "handler_audio::play_music() module " << 
        current_music_id << " playing!" << std::endl;
    }
  if (is_music_enable)
    {
      Mix_VolumeMusic (music_volume);
    }
  else
    {
      Mix_VolumeMusic (0);
    }
  return;
}

/**
 * Return the portion of the music currently played
 * @return identifier of the portion played
 */
Uint32
handler_audio::get_portion_music_played ()
{
  return current_portion_music;
}

/**
 * Handler of toggle keys, played sounds, and portions
 * of the music played 
 */
void
handler_audio::run ()
{
  if (!is_audio_enable)
    {
      return;
    }

  /* 
   * [Ctrl] + [S]: enable/disable sfx - mfx
   */
  if (keyboard->command_is_pressed (handler_keyboard::MFXSFXFLAG, true))
    {
      if (!is_music_enable || !is_sound_enable)
        {
          is_music_enable = true;
          is_sound_enable = true;
          Mix_VolumeMusic (music_volume);
        }
      else
        {
          is_music_enable = false;
          is_sound_enable = false;
          Mix_VolumeMusic (0);
        }
    }
  else
    { 
      /* [Ctrl] + [F]: enable/disable sfx */
      if (keyboard->command_is_pressed (handler_keyboard::SOUND_FLAG, true))
        {
          is_sound_enable = is_sound_enable ? false : true; 
          if (!is_sound_enable)
            {
               for (Sint32 i = 0; i < NUM_OF_SOUNDS; i++)
                {
                  sounds_play[i] = false;
                }
            }
        }
      /* [Ctrl] + [D]: enable/disable mfx */
      else
        {
          if (keyboard->
              command_is_pressed (handler_keyboard::MUSIC_FLAG, true))
            {
              is_music_enable = is_music_enable ? false : true;
              if (is_music_enable)
                {
                  Mix_VolumeMusic (music_volume);
                }
              else
                {
                  Mix_VolumeMusic (0);
                }
            }
        }
    }
  control_music_position ();
  play_requested_sounds ();
}

/**
 * Play all requested sounds 
 */
void
handler_audio::play_requested_sounds ()
{
  if (is_only_music || !is_sound_enable)
    {
      return;
    }
  for (Sint32 i = 0; i < NUM_OF_SOUNDS; i++)
    {
      if (sounds_play[i])
        {
          sounds_play[i] = false;
          Mix_PlayChannel (-1, sound_list[i], 0);
        }
    }
}

/**
 * Request to play sound effect
 * @param sound_num sound number
 */
void
handler_audio::play_sound (Uint32 sound_num)
{
  if (!is_only_music)
    {
      sounds_play[sound_num] = true;
    }
}

/**
 * Control of the portion of the played musics
 */
void
handler_audio::control_music_position ()
{
  if (NULL == song_module)
    {
      return;
    }
  if (song_pos != song_module->sngpos)
    {
      song_pos = song_module->sngpos;
    }
  Sint32 posgo = -1;
  switch (current_portion_music)
    {
    case GAME_PORTION:
    case SHOP_PORTION:
      if (song_module->sngpos < position_1)
        {
          posgo = position_1;
        }
      if (song_module->sngpos > position_2)
        {
          posgo = modposloop;
        }
      if (song_module->patpos >= 63 && song_module->sngpos == position_2)
        {
          posgo = modposloop;
        }
      if (posgo >= 0)
        {
          if (is_verbose)
            fprintf (stdout,
                     "handler_audio::execution1() - Player_SetPosition(%i)\n",
                     posgo);
          Mix_SetMusicPosition (posgo);
        }
      break;

    case LOST_PORTION:
      if (song_module->sngpos < position_1 ||
          song_module->sngpos > position_2 ||
          (song_module->patpos >= 63 && song_module->sngpos == position_2))
        {
          play_level_music (aera_number, level_number);
        }
      break;

    case WIN_PORTION:
      if (song_module->sngpos < position_1 ||
          song_module->sngpos > position_2 ||
          (song_module->patpos >= 63 && song_module->sngpos == position_2))
        {
          if (!Player_Paused ())
            {
              Player_TogglePause ();
            }
          current_portion_music = MUSIC_IS_OFF;
        }
      break;
    }
}

/**
  return music aera identifier
  @param narea area number, form 1 to 5
  @return music aera identifier
*/
Uint32
handler_audio::area_music (Uint32 narea)
{
  switch (narea)
    {
    case 2:
      return MUSICAREA2;
      break;
    case 3:
      return MUSICAREA3;
      break;
    case 4:
      return MUSICAREA4;
      break;
    case 5:
      return MUSICAREA5;
      break;
    default:
      return MUSICAREA1;
      break;
    }
  return MUSICAREA1;
}

/** 
 * Disable the sound effect
 */
void
handler_audio::disable_sound ()
{
  is_only_music = true;
  for (Sint32 i = 0; i < NUM_OF_SOUNDS; i++)
    {
      sounds_play[i] = false;
    }
}

/* 
 * Enable the sound effect
 */
void
handler_audio::enable_sound ()
{
  for (Sint32 i = 0; i < NUM_OF_SOUNDS; i++)
    {
      sounds_play[i] = false;
    }
  is_only_music = false;
}

#endif

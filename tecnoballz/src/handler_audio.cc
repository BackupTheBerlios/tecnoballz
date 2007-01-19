/** 
 * @file handler_audio.cc 
 * @brief Handler of the sound and music
 * @created 2004-03-22
 * @date 2007-01-19
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_audio.cc,v 1.1 2007/01/19 20:37:21 gurumeditation Exp $
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
#include "../include/handler_audio.h"
#ifndef SOUNDISOFF
#include "../include/ressources.h"
#include "../include/handler_keyboard.h"



handler_audio * handler_audio::audio_singleton = NULL;
bool handler_audio::is_audio_enable = true;

//######################################################################
// positions in music modules
//######################################################################
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
Mix_Chunk * handler_audio::sound_list[S_NOMBRETT];
char handler_audio::soundtable[S_NOMBRETT];

/**
 * Create the handler_audoi singleton object, and clear members
 */
handler_audio::handler_audio ()
{
  mentatInit ();
  initialize ();
  aera_number = 0;
  level_number = 0;
  ptModAmiga = NULL;
  is_sound_enable = false;
  music_flag = 0;
  sound_flag = 0;
  music_flag = ~music_flag;
  sound_flag = ~sound_flag;
  lastModule = MUSICINTRO;

}

/**
 * Release the handler_keyboard singleton object
 */
handler_audio::~handler_audio ()
{
  if (NULL != ptModAmiga)
    {
      Mix_HaltMusic ();
      Mix_FreeMusic (pMixmodule);
      ptModAmiga = NULL;
    }

  /* release the samples */
  for (Sint32 i = 0; i < S_NOMBRETT; i++)
    {
      if (sound_list[i])
        {
          Mix_FreeChunk (sound_list[i]);
          sound_list[i] = NULL;
        }
      soundtable[i] = 0;
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
 *  Initializes SDL audio and load files waves in memory 
 */
Sint32
handler_audio::initialize ()
{
  if (!is_audio_enable)
    {
      if (is_verbose)
        fprintf (stdout, "handler_audio::initialize() audio disable\n");
      return E_NO_ERROR;
    }
  if (SDL_Init (SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
    {
      fprintf (stderr, "handler_audio::initialize(): SDL_Init() failed: %s\n",
               SDL_GetError ());
      is_audio_enable = 0;
      return E_NO_ERROR;
    }
  if (Mix_OpenAudio (44100, AUDIO_S16, 2, 4096))
    {
      fprintf (stderr, "handler_audio::initialize(): %s\n", SDL_GetError ());
      is_audio_enable = 0;
      SDL_Quit ();
      return E_NO_ERROR;
    }
  //query_spec();
  musicVolum = Mix_VolumeMusic (-1);
  Mix_AllocateChannels (8);

  /* load files waves in memory */
  for (Sint32 i = 0; i < S_NOMBRETT; i++)
    {
      sound_list[i] = NULL;
    }
  waves_size = 0;
  for (Sint32 i = 0; i < S_NOMBRETT; i++)
    {
      char *pathname = pRessource->getSndFile (i);
      if (NULL == pathname)
        {
          fprintf (stderr,
                   "handler_audio::initialize() file %i not found\n", i);
          is_audio_enable = 0;
          return (E_NO_ERROR);
        }

      Mix_Chunk *ptWav = Mix_LoadWAV (pathname);
      if (!ptWav)
        {
          fprintf (stderr, "handler_audio::initialize() "
                   "Mix_LoadWAV(%s): %s\n", pathname, SDL_GetError ());
          is_audio_enable = 0;
          return (E_NO_ERROR);
        }
      Mix_VolumeChunk (ptWav, 32);
      sound_list[i] = ptWav;
      waves_size += ptWav->alen;
    }
  is_audio_enable = 1;
  is_sound_enable = true;
  if (is_verbose)
    fprintf (stdout, "handler_audio::initialize() initialize succeded\n");
  return E_NO_ERROR;
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
Sint32
handler_audio::play_level_music (Uint32 aera_num, Uint32 level)
{
  if (!is_audio_enable)
    {
      return E_NO_ERROR;
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
  musicphase = PHASE_GAME;
  Player_Stop ();
  return (E_NO_ERROR);
}

/**
 * Play the music of the shop
 * @param aera_num area number
 */
Sint32
handler_audio::play_shop_music (Uint32 aera_num)
{
  if (!is_audio_enable || NULL == ptModAmiga)
    {
      return E_NO_ERROR;
    }
  aera_number = aera_num;
  Uint32 music = area_music (aera_num);
  position_1 = ptMusicpos[music].pos_zeshop;
  position_2 = ptModAmiga->numpos - 1;
  modposloop = position_1;
  play_music (music);
  Mix_SetMusicPosition (position_1);
  musicphase = PHASE_SHOP;
  return (E_NO_ERROR);
}

/**
 * Play the music of the victory, when the player finished a level
 */
Sint32
handler_audio::play_win_music ()
{
  if (!is_audio_enable)
    {
      return E_NO_ERROR;
    }
  Uint32 music = area_music (aera_number);
  position_1 = ptMusicpos[music].pos_winner;
  position_2 = ptMusicpos[music].pos_losing - 1;
  modposloop = position_1;
  Mix_SetMusicPosition (position_1);
  musicphase = PHASE_WINN;
  return E_NO_ERROR;
}

/**
 * Play the music of the defeat, when the player loses a life
 */
Sint32
handler_audio::play_lost_music ()
{
  if (!is_audio_enable || musicphase == PHASE_LOST)
    {
      return E_NO_ERROR;
    }
  Uint32 music = area_music (aera_number);
  position_1 = ptMusicpos[music].pos_losing;
  position_2 = ptMusicpos[music].pos_zeshop - 1;
  modposloop = position_1;
  Mix_SetMusicPosition (position_1);
  musicphase = PHASE_LOST;
  return (E_NO_ERROR);
}

//------------------------------------------------------------------------------
// stop the music lost
//------------------------------------------------------------------------------
Sint32
handler_audio::lostm_stop ()
{
  if (!is_audio_enable)
    return E_NO_ERROR;
  if (musicphase != PHASE_LOST)
    return (E_NO_ERROR);
  play_level_music (aera_number, level_number);
  return (E_NO_ERROR);
}

//------------------------------------------------------------------------------
// bricks level end music is finish? 
//------------------------------------------------------------------------------
Uint32
handler_audio::winn_isend ()
{
  if (!is_audio_enable)
    return 1;
  if (musicphase == PHASE_WINN)
    return 0;
  else
    return 1;
}

//------------------------------------------------------------------------------
//      if a music is played, it is stopped
//------------------------------------------------------------------------------
void
handler_audio::stopModule ()
{
  if (!is_audio_enable)
    return;
  if (ptModAmiga)
    {
      Mix_HaltMusic ();
      Mix_FreeMusic (pMixmodule);
      ptModAmiga = NULL;
      nummodule1 = -1;
    }
}

//------------------------------------------------------------------------------
// play a module
// input => audio       : 1 = sound actif
// ouput <= error 
//-----------------------------------------------------------------------------
Sint32
handler_audio::play_music (Uint32 modnu)
{
  //return E_NO_ERROR;    //test only
  if (!is_audio_enable)
    return E_NO_ERROR;
  //if (!music_flag) return E_NO_ERROR;


  //###################################################################
  //      if a music is played, it is stopped
  //###################################################################
  if (ptModAmiga)
    {
      if (nummodule1 == (Sint32) modnu)
        return E_NO_ERROR;
      else
        stopModule ();
    }
  char *pathname = pRessource->getMusFile (modnu);
  if (is_verbose)
    printf ("handler_audio::play_music() try load pathname %s\n", pathname);
  if (!pathname)
    {
      fprintf (stderr,
               "handler_audio::play_music () modnu %i %s has no file \n",
               modnu, pathname);
      return (erreur_num = E_SDLMIXER);
    }

  //###################################################################
  // load the music in memory
  //###################################################################
  pMixmodule = Mix_LoadMUS (pathname);
  if (!pMixmodule)
    {
      fprintf (stderr, "handler_audio::charge_mod () erreur %s\n",
               SDL_GetError ());
      return (erreur_num = E_SDLMIXER);
    }
  // Ugly way to access sdl-mixer's internal structures.
  union
  {
    int i;
    void *p;
  } dummy;
  memcpy (&ptModAmiga, (Uint8 *) pMixmodule + sizeof (dummy),
          sizeof (void *));


  //###################################################################
  // play the music 
  //###################################################################
  if (Mix_PlayMusic (pMixmodule, -1) == -1)
    {
      fprintf (stderr, "! handler_audio::play_music() %s\n", SDL_GetError ());
      return (erreur_num = E_SDLMIXER);
    }


  musicphase = PHASE_NORM;
  lastsngpos = -1;
  lastpatpos = -1;
  nummodule1 = modnu;
  if (is_verbose)
    fprintf (stdout, "handler_audio::play_music() module %i playing\n",
             nummodule1);
  lastModule = modnu;
  if (music_flag)
    Mix_VolumeMusic (musicVolum);
  else
    Mix_VolumeMusic (0);

  return (E_NO_ERROR);
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
Uint32
handler_audio::get_mphase ()
{
  return musicphase;
}

//------------------------------------------------------------------------------
// management of the music and sounds
//-----------------------------------------------------------------------------
void
handler_audio::execution1 ()
{
  if (!is_audio_enable)
    return;

  // [CTRL] + [S]: enable/disable sfx - mfx
  if (keyboard->command_is_pressed (handler_keyboard::MFXSFXFLAG, true))
    {
      if (!music_flag || !sound_flag)
        {
          if (!music_flag)
            music_flag = ~music_flag;
          if (!sound_flag)
            sound_flag = ~sound_flag;
          Mix_VolumeMusic (musicVolum);
          //printf("[CTRL] + [S] : enable sound and music\n");
        }
      else
        {
          music_flag = 0;
          sound_flag = 0;
          Mix_VolumeMusic (0);
          //printf("[CTRL] + [S] : disable sound and music\n");
        }
    }
  else
    {                           // [CTRL] + [F]: enable/disable sfx
      if (keyboard->command_is_pressed (handler_keyboard::SOUND_FLAG, true))
        {
          sound_flag = ~sound_flag;
          //printf("[CTRL] + [F] : %i\n", sound_flag);
        }
      // [CTRL] + [D]: enable/disable mfx
      else
        {
          if (keyboard->
              command_is_pressed (handler_keyboard::MUSIC_FLAG, true))
            {
              music_flag = ~music_flag;
              //printf("[CTRL] + [D] : %i %i\n", music_flag, musicVolum);
              if (music_flag)
                Mix_VolumeMusic (musicVolum);
              else
                Mix_VolumeMusic (0);
            }
        }
    }
  mus_handle ();
  snd_handle ();
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void
handler_audio::snd_handle ()
{
  if (!is_sound_enable)
    return;
  if (!sound_flag)
    return;
  for (Sint32 i = 0; i < S_NOMBRETT; i++)
    {
      if (soundtable[i])
        {
          soundtable[i] = 0;
          Mix_PlayChannel (-1, sound_list[i], 0);
        }
    }
}

/**
 * Request of a sound
 * @param sound_num sound number
 */
void
handler_audio::play_sound (Uint32 sound_num)
{
  if (is_sound_enable)
    {
      soundtable[sound_num] = 1;
    }
}

//------------------------------------------------------------------------------
// management of the music
//-----------------------------------------------------------------------------
void
handler_audio::mus_handle ()
{
  if (!ptModAmiga)
    return;

  if (lastsngpos != ptModAmiga->sngpos)
    lastsngpos = ptModAmiga->sngpos;

  Sint32 posgo = -1;
  switch (musicphase)
    {
    case PHASE_GAME:
    case PHASE_SHOP:
      if (ptModAmiga->sngpos < position_1)
        posgo = position_1;
      if (ptModAmiga->sngpos > position_2)
        posgo = modposloop;
      if (ptModAmiga->patpos >= 63 && ptModAmiga->sngpos == position_2)
        posgo = modposloop;
      if (posgo >= 0)
        {
          if (is_verbose)
            fprintf (stdout,
                     "handler_audio::execution1() - Player_SetPosition(%i)\n",
                     posgo);
          Mix_SetMusicPosition (posgo);
        }
      break;

    case PHASE_LOST:
      if (ptModAmiga->sngpos < position_1 ||
          ptModAmiga->sngpos > position_2 ||
          (ptModAmiga->patpos >= 63 && ptModAmiga->sngpos == position_2))
        play_level_music (aera_number, level_number);
      break;

    case PHASE_WINN:
      if (ptModAmiga->sngpos < position_1 ||
          ptModAmiga->sngpos > position_2 ||
          (ptModAmiga->patpos >= 63 && ptModAmiga->sngpos == position_2))
        {                       //Mix_PauseMusic();
          if (!Player_Paused ())
            Player_TogglePause ();
          musicphase = PHASE_MOFF;
        }
      break;


    }
}

//------------------------------------------------------------------------------
// return the number of the music aera
// input => narea       : number of the area (1 to 5)
// ouput <= narea       : number of the music area (0 to 5) 
//-----------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// disable sound
//------------------------------------------------------------------------------
void
handler_audio::stop_sound ()
{
  is_sound_enable = false;
  for (Sint32 i = 0; i < S_NOMBRETT; i++)
    {
      soundtable[i] = 0;
    }
}

//------------------------------------------------------------------------------
// enable sound
//------------------------------------------------------------------------------
void
handler_audio::startSound ()
{
  for (Sint32 i = 0; i < S_NOMBRETT; i++)
    {
      soundtable[i] = 0;
    }
  is_sound_enable = true;
}


#endif

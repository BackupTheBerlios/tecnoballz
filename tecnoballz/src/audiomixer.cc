//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "audiomixer.cc"
// created	: 2004-03-22
// updates	: 2005-01-15
// functions	: handler music and sound
// id		: $Id: audiomixer.cc,v 1.2 2005/01/15 19:59:58 gurumeditation Exp $
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//*****************************************************************************
#include "../include/audiomixer.h"
#ifndef SOUNDISOFF
#include "../include/ressources.h"
#include "../include/clavierMac.h"



Sint32 audiomixer::audioactif = 1;

//######################################################################
// positions in music modules
//######################################################################
const musics_pos audiomixer::ptMusicpos[] =
{	{	0,		// first music of a "bricks level"
		2,		// restart first music

		11,		// second music of a "bricks level"
		11,		// restart second music
	
		23,		// "bricks level" completed
		24,		// lost ball in "bricks level"
		25		// shop music
	},
	{0, 0, 15, 15, 22, 23, 24},
	{0, 0, 15, 15, 28, 29, 30},
	{0, 0, 11, 11, 18, 19, 20},
	{0, 0, 15, 15, 30, 31, 32}
};

//######################################################################
// list of sounds pointers
//######################################################################
Mix_Chunk* audiomixer::sound_list[S_NOMBRETT];
char audiomixer::soundtable[S_NOMBRETT];

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
audiomixer::audiomixer()
{
	mentatInit();
	initialise();
	areaNumber = 0;
	levelTecno = 0;
	ptModAmiga = NULL;
	flag_sound = 0;
	music_flag = 0;	
	sound_flag = 0;
	music_flag = ~music_flag;	
	sound_flag = ~sound_flag;
	lastModule = MUSICINTRO;
	
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
audiomixer::~audiomixer()
{
	if (ptModAmiga) 
	{	Mix_HaltMusic();
		Mix_FreeMusic(pMixmodule);
		ptModAmiga = NULL;
	}

	// release the samples
	for(Sint32 i = 0; i < S_NOMBRETT; i++)
	{	if (sound_list[i])
		{	Mix_FreeChunk(sound_list[i]);
			sound_list[i] = NULL;
		}
		soundtable[i] = 0;
	}
	
	if (audioactif)
	{	Mix_CloseAudio();
		SDL_Quit();
	}
}

//------------------------------------------------------------------------------
// perform some initializations
//------------------------------------------------------------------------------
Sint32 audiomixer::initialise()
{
	if (!audioactif)
	{	if(is_verbose)
			fprintf (stdout,
				"audiomixer::initialise() audio disable\n");
		return E_NO_ERROR;
	}
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
	{	fprintf (stderr, "audiomixer::initialise(): SDL_Init() failed: %s\n",
			SDL_GetError());
			audioactif = 0;
			return E_NO_ERROR;
	}
	if(Mix_OpenAudio(44100, AUDIO_S16, 2, 4096))
	{	fprintf(stderr,"audiomixer::initialise(): %s\n",SDL_GetError());
		audioactif = 0;
		SDL_Quit();
		return E_NO_ERROR;
	}
	musicVolum = Mix_VolumeMusic(-1);
	Mix_AllocateChannels(8);
	
	//###################################################################
	// load files waves in memory
	//###################################################################
	for(Sint32 i = 0; i < S_NOMBRETT; i++)
		sound_list[i] = NULL;
	waves_size = 0;
	for(Sint32 i = 0; i < S_NOMBRETT; i++)
	{	char *pathname = pRessource->getSndFile(i);
		if(!pathname)
		{	fprintf(stderr,
				"audiomixer::initialise() file %i not found\n", i);
			audioactif = 0;
			return (E_NO_ERROR);
		}
		
		Mix_Chunk* ptWav = Mix_LoadWAV(pathname);
		if(!ptWav)
		{	fprintf(stderr, "audiomixer::initialise() "
				"Mix_LoadWAV(%s): %s\n", pathname, SDL_GetError());
			audioactif = 0;
			return (E_NO_ERROR);
		}
		Mix_VolumeChunk(ptWav, 32);
		sound_list[i] = ptWav;
		waves_size += ptWav->alen;
	}
	audioactif = 1;
	flag_sound = 1;
	if(is_verbose)
		fprintf (stdout,
			"audiomixer::initialise() initialize succeded\n");
	return E_NO_ERROR;
}

//------------------------------------------------------------------------------
// display some infos
//------------------------------------------------------------------------------
void audiomixer::query_spec() 
{	Sint32 nopen, frequ, nchan;
	Uint16 fmNum;
	nopen = Mix_QuerySpec(&frequ, &fmNum, &nchan);
	if(!nopen)
		fprintf(stderr, "Mix_QuerySpec: %s\n", Mix_GetError());
	else
	{	char *fmStr = "Unknown";
		switch(fmNum)
		{	case AUDIO_U8: fmStr = "U8";
				break;
			case AUDIO_S8: fmStr = "S8";
				break;
			case AUDIO_U16LSB: fmStr = "U16LSB";
				break;
			case AUDIO_S16LSB: fmStr = "S16LSB";
				break;
			case AUDIO_U16MSB: fmStr = "U16MSB";
				break;
			case AUDIO_S16MSB: fmStr = "S16MSB";
				break;
		}
		printf("opened=%d times frequency=%dHz format=%s channels=%d",
				nopen, frequ, fmStr, nchan);
	}
}

//------------------------------------------------------------------------------
// play a music aera
// input	=> narea	: number of the area (1 to 5)
//			=> level	: number of the bricks level (1 to 12)
// ouput	<= error
//-----------------------------------------------------------------------------
Sint32 audiomixer::levelmusic(Uint32 narea, Uint32 level) 
{
	if (!audioactif) return E_NO_ERROR;
	areaNumber = narea;
	levelTecno = level;
	Uint32 music = area_music(narea);
	Uint32 paire = level & 0x1;
	if((level <= 5 && paire) || (level > 5 && !paire))
	{	modposloop = ptMusicpos[music].loopmusic1;
		position_1 = ptMusicpos[music].pos_music1; 
		position_2 = ptMusicpos[music].pos_music2 - 1;
	}
	else
	{	modposloop = ptMusicpos[music].loopmusic2;
		position_1 = ptMusicpos[music].pos_music2;
		position_2 = ptMusicpos[music].pos_winner - 1;
	}
	playModule (music);
	Mix_SetMusicPosition(position_1);
	musicphase = PHASE_GAME;
	Player_Stop();
	return(E_NO_ERROR);
}

//------------------------------------------------------------------------------
// start the music shop
//------------------------------------------------------------------------------
Sint32 audiomixer::shop_music (Uint32 narea) 
{
	if(!audioactif) return E_NO_ERROR;
	if(!ptModAmiga) return E_NO_ERROR;
	areaNumber = narea;
	Uint32 music = area_music(narea);
	position_1 = ptMusicpos[music].pos_zeshop;
	position_2 = ptModAmiga->numpos - 1;
	modposloop = position_1;
	playModule (music);
	Mix_SetMusicPosition(position_1);
	musicphase = PHASE_SHOP;
	return(E_NO_ERROR);
}

//------------------------------------------------------------------------------
// start the music win
//------------------------------------------------------------------------------
Sint32 audiomixer::winn_music() 
{	
	if (!audioactif) return E_NO_ERROR;
	Uint32 music = area_music(areaNumber);
	position_1 = ptMusicpos[music].pos_winner;
	position_2 = ptMusicpos[music].pos_losing - 1;
	modposloop = position_1;
	Mix_SetMusicPosition(position_1);
	musicphase = PHASE_WINN;
	return(E_NO_ERROR);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32 audiomixer::lost_music() 
{	
	if (!audioactif) return E_NO_ERROR;
	if (musicphase == PHASE_LOST)
		return (E_NO_ERROR);
	Uint32 music = area_music(areaNumber);
	position_1 = ptMusicpos[music].pos_losing;
	position_2 = ptMusicpos[music].pos_zeshop - 1;
	modposloop = position_1;
	Mix_SetMusicPosition(position_1);
	musicphase = PHASE_LOST;
	return(E_NO_ERROR);
}

//------------------------------------------------------------------------------
// stop the music lost
//------------------------------------------------------------------------------
Sint32 audiomixer::lostm_stop()
{	
	if (!audioactif) return E_NO_ERROR;
	if (musicphase != PHASE_LOST)
		return (E_NO_ERROR);
	levelmusic(areaNumber, levelTecno);
	return(E_NO_ERROR);
}

//------------------------------------------------------------------------------
// bricks level end music is finish? 
//------------------------------------------------------------------------------
Uint32	audiomixer::winn_isend()
{	
	if (!audioactif) return 1;
	if(musicphase == PHASE_WINN)
		return 0;
	else
		return 1;
}

//------------------------------------------------------------------------------
// 	if a music is played, it is stopped
//------------------------------------------------------------------------------
void audiomixer::stopModule()
{
	if (!audioactif) return;
	if (ptModAmiga) 
	{	Mix_HaltMusic();
		Mix_FreeMusic(pMixmodule);
		ptModAmiga = NULL;
		nummodule1 = -1;
	}
}

//------------------------------------------------------------------------------
// play a module
// input => audio	: 1 = sound actif
// ouput <= error 
//-----------------------------------------------------------------------------
Sint32 audiomixer::playModule(Uint32 modnu)
{
	//return E_NO_ERROR;	//test only
	if (!audioactif) return E_NO_ERROR;
	//if (!music_flag) return E_NO_ERROR;

	
	//###################################################################
	// 	if a music is played, it is stopped
	//###################################################################
	if (ptModAmiga) 
	{	if (nummodule1 == (Sint32) modnu)
			return E_NO_ERROR;
		else
			stopModule();
	}
	char *pathname = pRessource->getMusFile(modnu);
	if(is_verbose)
		printf ("audiomixer::playModule() try load pathname %s\n", pathname);
 	if(!pathname)
	{	fprintf(stderr,
			"audiomixer::playModule () modnu %i %s has no file \n",
			modnu, pathname);
		return (erreur_num = E_SDLMIXER);
 	}

	//###################################################################
	// load the music in memory
	//###################################################################
	pMixmodule = Mix_LoadMUS(pathname);
 	if(!pMixmodule)
	{	fprintf(stderr, "audiomixer::charge_mod () erreur %s\n",
			SDL_GetError());
		return (erreur_num = E_SDLMIXER);
	}
	Sint32 *p = (Sint32 *)pMixmodule;
	ptModAmiga = (MODULE *)p[1];


	//###################################################################
	// play the music 
	//###################################################################
	if(Mix_PlayMusic(pMixmodule, -1) == -1)
	{	fprintf(stderr, "! audiomixer::playModule() %s\n", SDL_GetError());
		return (erreur_num = E_SDLMIXER);
	}


	musicphase = PHASE_NORM;
	lastsngpos = -1;
	lastpatpos = -1;
	nummodule1 = modnu;
	if(is_verbose)
		fprintf(stdout, "audiomixer::playModule() module %i playing\n",
			nummodule1);
	lastModule = modnu;
	if(music_flag)
		Mix_VolumeMusic(musicVolum);
	else
		Mix_VolumeMusic(0);

	return (E_NO_ERROR);
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
Uint32	audiomixer::get_mphase()
{	return musicphase;
}

//------------------------------------------------------------------------------
// management of the music and sounds
//-----------------------------------------------------------------------------
void audiomixer::execution1() 
{	
	if (!audioactif) return;

	// [CTRL] + [S]: enable/disable sfx - mfx
	if(keyGestion->specKeyRaz(clavierMac::MFXSFXFLAG))
	{	if(!music_flag || !sound_flag)
		{	if(!music_flag) music_flag = ~music_flag;
			if(!sound_flag) sound_flag = ~sound_flag;
			Mix_VolumeMusic(musicVolum);
			//printf("[CTRL] + [S] : enable sound and music\n");
		}
		else
		{	music_flag = 0;
			sound_flag = 0;
			Mix_VolumeMusic(0);
			//printf("[CTRL] + [S] : disable sound and music\n");
		}
	}
	else
	{	// [CTRL] + [F]: enable/disable sfx
		if(keyGestion->specKeyRaz(clavierMac::SOUND_FLAG))
		{	sound_flag = ~sound_flag;
			//printf("[CTRL] + [F] : %i\n", sound_flag);
		}
		// [CTRL] + [D]: enable/disable mfx
		else
		{	if(keyGestion->specKeyRaz(clavierMac::MUSIC_FLAG))
			{	music_flag = ~music_flag;
				//printf("[CTRL] + [D] : %i %i\n", music_flag, musicVolum);
				if(music_flag)
					Mix_VolumeMusic(musicVolum);
				else
					Mix_VolumeMusic(0);
			}
		}	
	}
	mus_handle();
	snd_handle();
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void audiomixer::snd_handle() 
{	
	// return;	//test only
	if (!flag_sound) return;
	if (!sound_flag) return;
	for (Sint32 i = 0; i < S_NOMBRETT ; i++)
	{	if (soundtable[i])
		{	soundtable[i] = 0;
			Mix_PlayChannel(-1, sound_list[i], 0);
		}
	}
}

//------------------------------------------------------------------------------
// request of a sound
//------------------------------------------------------------------------------
void audiomixer::sound_play (Uint32 sndnu)
{	if (!flag_sound) return;
	soundtable[sndnu] = 1;
}

//------------------------------------------------------------------------------
// management of the music
//-----------------------------------------------------------------------------
void audiomixer::mus_handle()
{	
	if (!ptModAmiga) return;
	
	if(lastsngpos != ptModAmiga->sngpos)
		lastsngpos = ptModAmiga->sngpos;
		
	Sint32 posgo = -1;
	switch(musicphase)		
	{
		case PHASE_GAME:
		case PHASE_SHOP:
			if(ptModAmiga->sngpos < position_1)
				posgo = position_1;
			if(ptModAmiga->sngpos > position_2)
				posgo = modposloop;
			if(ptModAmiga->patpos >= 63 && ptModAmiga->sngpos == position_2)
					posgo = modposloop;	
			if(posgo >= 0)
			{	if(is_verbose)
					fprintf (stdout,
						"audiomixer::execution1() - Player_SetPosition(%i)\n",
						posgo);
				Mix_SetMusicPosition(posgo);
			}
			break;
			
		case PHASE_LOST:
			if(ptModAmiga->sngpos < position_1 ||
				ptModAmiga->sngpos > position_2 ||
				(ptModAmiga->patpos >= 63 && ptModAmiga->sngpos == position_2))
				levelmusic(areaNumber, levelTecno);
				break;

		case PHASE_WINN:
			if(ptModAmiga->sngpos < position_1 ||
				ptModAmiga->sngpos > position_2 ||
				(ptModAmiga->patpos >= 63 && ptModAmiga->sngpos == position_2))
			{	//Mix_PauseMusic();
				if (!Player_Paused ())
					Player_TogglePause();
				musicphase = PHASE_MOFF;
			}
			break;
			
			
	}
}

//------------------------------------------------------------------------------
// return the number of the music aera
// input => narea	: number of the area (1 to 5)
// ouput <= narea	: number of the music area (0 to 5) 
//-----------------------------------------------------------------------------
Uint32 audiomixer::area_music (Uint32 narea)
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
}

//------------------------------------------------------------------------------
// disable sound
//------------------------------------------------------------------------------
void audiomixer::stop_sound()
{
	flag_sound = 0;
	for(Sint32 i = 0; i < S_NOMBRETT; i++)
		soundtable[i] = 0;
}

//------------------------------------------------------------------------------
// enable sound
//------------------------------------------------------------------------------
void audiomixer::startSound()
{
	for(Sint32 i = 0; i < S_NOMBRETT; i++)
		soundtable[i] = 0;
	flag_sound = 1;
}


#endif

//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "audiomixer.h"
// created		: 2004-03-22
// updates		: 2004-10-24
// functions	: handler music and sound
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
//*****************************************************************************
//#define SOUNDISOFF if define, don't compile audiomixer.cc
#ifndef SOUNDISOFF
#ifndef __AUDIOMIXER__
#define __AUDIOMIXER__
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <mikmod.h>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class audiomixer;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "mentatCode.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// numbers of the music
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
{	Uint32	pos_music1;
	Uint32	loopmusic1;
	Uint32	pos_music2;
	Uint32	loopmusic2;
	Uint32	pos_winner;
	Uint32	pos_losing;
	Uint32	pos_zeshop;
} musics_pos;

// differents phases in a area music
typedef enum
{	PHASE_NORM,
	PHASE_GAME,
	PHASE_WINN,
	PHASE_LOST,
	PHASE_SHOP,
	PHASE_MOFF
} phasemusic;

// numbers of the sounds
typedef enum
{	S_ENLEVVIE,	// lose a life
	S_RAKEXPLO,	// explosion of the bumper
	S_AJOUTVIE,	// extra life
	S_MONNAIES,	// collect money
	S_TILTALAR, // alarm 
	S_GADGETGO, // collect option (extra balls, power balls)
	S_TECNOBAL, // "tecno" sound
	S_ATOMAPPA,
	S_TRANSFOR, // bumper transformation (expand, glue)
	S_TIR_GARD,	// guard's fire
	S_RAK_TIRS,	// bumper's fire
	S_BIGEXPLO,	// guard's explosion
	S_CASSEIND,	// break a indestructible brick
	S_ATOM_EXP, //
	S_GARDIENT, // hit the guard
	S_TO_ATOMS,	//
	S_TOINDES1,	// hit the indestructible brick 1
	S_TOINDES2,	// hit the indestructible brick 2
	S_TOUCHRAK,
	S_BRICOTES,
	S_TOUBRIK1,
	S_TOUBRIK2,
	S_TOUBRIK3,
	S_TOUBRIK4,
	S_TOUBRIK5,
	S_COINEJEC,
	S_COINASPI,
	S_NOMBRETT
} sound_code;

// definition of the class
class audiomixer:public virtual mentatCode
{
	public:
		static Sint32	audioactif;		// 1 = SDL_mixer is enable

	public:
				audiomixer();
				~audiomixer();
		void	stopModule();
		Sint32	playModule(Uint32 modnu);
		void	execution1();
		Sint32 	levelmusic(Uint32, Uint32);
		Sint32	shop_music(Uint32); 
		Sint32	winn_music(); 
		Sint32	lost_music(); 
		Sint32	lostm_stop();
		Uint32	winn_isend();
		Uint32	get_mphase();
		void	sound_play(Uint32);
		void	stop_sound();
		void	startSound();
		
	private:
		void			query_spec();
		Uint32			area_music(Uint32);
		Sint32			initialise();
		void			mus_handle();
		void			snd_handle();

	// statics members
	private:
		static char				soundtable[S_NOMBRETT];
		static Mix_Chunk*		sound_list[];
		static const musics_pos	ptMusicpos[];

	private:
		Uint32		areaNumber;		// area number (1 to 5)
		Uint32		levelTecno;		// level number (1 to 12) 
		
		Sint32		position_1;	
		Sint32		modposloop;	
		Sint32		position_2;	
		Uint32		musicphase;		// normal / level / shop / ...
	

		Uint32		flag_music;		// 1 = music enable
		Uint32		flag_sound;		// 1 = sound enable
		Uint32		lastModule;

		Uint32		music_flag;		// 0 or 1 (CTRL + D / CTRL + S)
		Uint32		sound_flag;		// 0 or 1 (CTRL + F / CTRL + S)
	
		Sint32		musicVolum;		
		Sint32		nummodule1;		// numero module charge
		Mix_Music*	pMixmodule;
		MODULE*		ptModAmiga;		// structure module
		Uint32		waves_size;		// size fo all waves
		Sint32		lastsngpos;
		Sint32		lastpatpos;
	
};
#endif
#endif

//*****************************************************************************
// copyright (c) 1991-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "mentatCode.h"
// created	: 2002-08-18
// updates	: 2006-10-02
// ID		: $Id: mentatCode.h,v 1.23 2007/01/31 15:20:07 gurumeditation Exp $
// updates	: 2006-10-02
// ID		: $Id: mentatCode.h,v 1.23 2007/01/31 15:20:07 gurumeditation Exp $
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
//...............................................................................
#ifdef WIN32
#include <io.h>
#include "SDL/SDL.h"
//...............................................................................
#else //WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <pthread.h>
#include <unistd.h>
#include "SDL/SDL.h"
#endif //WIN32
//...............................................................................
#include "../include/configfile.h"
//...............................................................................
#ifndef __MENTATCODE__
#define __MENTATCODE__
#define TECNOBALLZ_VERSION	"TECNOBALLZ 0.91+ (2007-01-31)"
#define TU_TRICHES		//devel only
#define BYTES_COPY		//force bytes copy (SPARC unaligned memory access)
#ifndef SCOREFILE
#define SCOREFILE "tecnoball"
#endif
#ifndef PREFIX
#define PREFIX ""
#endif
//...............................................................................
class mentatCode;
class configfile;
class scoretable;
class handler_resources;
class level_data;
class handler_memory;
class handler_display;
class handler_keyboard;
#ifndef SOUNDISOFF
class handler_audio;
#endif
class list_sprites;
class joueurData;
class tableaux_Z;
class shop_tecno;
class gard_tecno;
class menu_tecno;
class scrolledit;
class bitmap_data;
class offscreen_surface;
//..............................................................................
const Sint32 E_NO_ERROR = 0;
const Sint32 E_GENRIQUE = -1;	// erreur generique
const Sint32 E_OUT_LIST = -2;	// erreur reservation de la liste memoire
const Sint32 E_NOMEMORY = -3;	// erreur reservation : depassement memoire
const Sint32 E_OUT_ZONE = -4;	// erreur reservation : maxium de zones memoire 
const Sint32 E_NORESOUR = -7;	// ressource non presente
const Sint32 E_SDLERROR = -8;
const Sint32 E_FILERROR = -9;
const Sint32 E_SDLMIXER = -9;
const Sint32 SINUS_MASK = 511;
const Sint32 SINUS_DECA = 7;
//..............................................................................
class mentatCode 
{
	public:
		static const Sint32	NB_OPTIONS = 19;	//number maximum of bonus bought
		static const Uint32	NDIFFICULT = 4;		//4 levels of difficulty
		static const Sint32	MAX_PLAYER = 6;	
	
	public:
		static bool		is_verbose;		//1 = verbose mode
		static Sint32		bg4_colors;		//1 = force 4 colors background
		static Sint32		resolution;		//1:320*240 or 2:640*480 pixels
		static bool		bob_ground;		//1 = background is sprites 
		static Uint32		double_mem;		//double memory size of all allocations 
		static Sint32		arg_jumper;

	protected:
		Sint32			numero_obj;		//object number
		Sint32			erreur_num;		//error code

		//###############################################################
		// statics members 
		//###############################################################
		static Sint32		counterObj;		//number of objects
		static Sint32		hasard_val;		//random value
		static Sint32		countframe;		//frame counter
		static Sint32		num_erreur;		//error number
	
		static Sint32		super_jump;		// game phase:
								// - 0: bricks levels
								// - 1: shop
								// - 2: guards levels
								// - 3: menu

		static Sint32		hardChoice;		//difficulty
								// -1: easy
								// -2: hardChoice
								// -3: mad
								// -4: dead

		static Sint32		super_exit;		//1: game exit

		static Uint32		cheat_flag;		//E + T + B + Return (into shop)
		static Uint32		birth_flag;		//1 = all names is 040670
		static Sint32		vieInitial;		//nmber of initial lives
		static Sint32		nuOfPlayer;
		static const char	nomprefix[];
		static char		chainelog[100];
		static const Uint32	LEVEL_AREA = 12;
	
		static scoretable*	ptScoreTab;		//mange best scores
    /** Handler of the files resources */
		static handler_resources*	resources;
		static level_data*	ptLev_data;		//manage levels
		static handler_memory*	memory;		//manage memory allocation   
#ifndef SOUNDISOFF
		static handler_audio*	audio;		//manage sounds and musics
#endif
		static handler_display*	display;		//manage screen display
		static handler_keyboard*	keyboard;		//manage keyboard and mouse events 
		static list_sprites*	sprites;
		static joueurData*	joueurGere;

		static Sint16*		table_cosL;
		static Sint16*		table_sinL;
		static tableaux_Z*	tabGestion;
		static shop_tecno*	shpGestion;
		static gard_tecno*	garGestion;
		static menu_tecno*	menGestion;
		static scrolledit*	ptScrollEd;  
    /** A bitmap containing the current images of sprites */
		static bitmap_data*	sprites_bitmap;
		static char		zeAreaCode[11];

    static offscreen_surface* game_screen;
    static offscreen_surface* background_screen;

	public:
		static Sint32		first_init(configfile* pConf);
		static Sint32		game_begin();
		static void		fullscreen();
		static void		objectKill();
		static Sint32		desinstall(configfile* pConf);
 
		static void		ecritLog(char *f_nom, char* chain);
		static void		intToASCII(Sint32 value, char *strng, Uint32 reste);

					mentatCode();
					~mentatCode();
		void			mentatInit();
		void			mentatKill();
		Sint32			retour_err();
		void			error_init(Sint32 error);
		Sint32			get_number();
		Sint32			chaine_cmp(char *srcPT, char *desPT, Sint32 taille);
		Sint32			littleWord(char *memPT);
		void			bigendianw(Uint32*, Uint32*);
		void			bigendianr(Uint32*, Uint32*);
};
#endif

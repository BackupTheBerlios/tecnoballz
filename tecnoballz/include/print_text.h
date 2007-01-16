//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "print_text.h"
// created		: ?
// updates		: 2004-10-10
// fonction	: display chars
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
//******************************************************************************
#ifndef __PRINT_TEXT__
#define __PRINT_TEXT__
//...............................................................................
class print_text;
//...............................................................................
#include "../include/ecran_hard.h"
#include "../include/GIF_bitMap.h"
#include "../include/mentatCode.h"
#include "../include/sprite_object.h"
//...............................................................................
class print_text:public virtual mentatCode
{
	protected:
		static char			ascii2code[128];
		Sint32				off_desti1;	// modulo destination affichage ecran
		Sint32				off_source;	// modulo source page graphique
		GIF_bitMap*			GFX_fontes;
		char*				fontes_adr;
		char*				caract_adr;
		Uint32				charHeight;	//height of chars in pixels

	public:
							print_text();
							~print_text();
		void				initial_me();
		void				destroy_me();
		Sint32				initialise();
		Uint32				getCharHgt();
		Sint32				init_print(Sint32);
		void				affNombre1(char *desP1, Sint32 value, Sint32 baseN);
		void				bufferAff1(Sint32 x, Sint32 y, Sint32 value, Sint32 baseN);
		void				tamponAff1(Sint32 x, Sint32 y, Sint32 value, Sint32 baseN);
		void				aff_texte1(char *desP1, char *chain, Sint32 total);
		void				tamponAff2(Sint32 x, Sint32 y, char *chain, Sint32 total);
		void				bufferAff2(Sint32 x, Sint32 y, char *chain, Sint32 total);
		sprite_object*			string2bob(const char* ptStr);
};
#endif

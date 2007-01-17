//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "head_anima.h"
// created		: ?
// updates		: 2004-04-24
// fonction	: anim head
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
#ifndef __HEAD_ANIMA__
#define __HEAD_ANIMA__
class head_anima;
//.......................................................................
#include "../include/handler_display.h"
#include "../include/GIF_bitMap.h"
#include "../include/mentatCode.h"
//.......................................................................
class head_anima:public virtual mentatCode
{
	private:
		Sint32				off_source;	//modulo source
		Sint32				off_destin;	//modulo destination
		char*				adr_source;	//graphic page memory address
		char*				adr_destin;	//buffer memory address
		Sint32				la_hauteur;	//height of the graphic page 
		GIF_bitMap*			headbitmap;
		Sint32				head_count;	//temporization counter
		Sint32				head_reque;	//animation requested 
		Sint32				head_runni;	//current animation 
		Sint32				head_step1;	//offset (1 or -1)
    
		static const Sint32	TETE_PARA1 = 1;		//disturbance first image
		static const Sint32	TETE_PARA2 = 3;		//disturbance last image
		static const Sint32	TETE_BAIL1 = 4;		//yawning first image
		static const Sint32	TETE_BAIL2 = 8;		//yawning last image
		static const Sint32	TETE_RIRE1 = 9;		//laugh first image
		static const Sint32	TETE_RIRE2 = 13;	//laugh last image
		static const Sint32	TETESPEED1 = 5;		//speed of animation 
		static const Sint32	TETESPEED2 = 20;	//downtime on last animation 

	public:
							head_anima();
							~head_anima();
		Sint32				initialise();
		void				affiche_me();
		void				execution1();
		void				tetebaille();
		void				teterigole();
		void				teteparasi();
};
#endif

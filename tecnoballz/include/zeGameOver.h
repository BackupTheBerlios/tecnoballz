//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeGameOver.h"
// created		: 2002-12-14
// updates		: 2004-10-10
// fonctions	: handle the "Game Over"
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
#ifndef __ZEGAMEOVER__
#define __ZEGAMEOVER__
//...............................................................................
class zeGameOver;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/objet_gere.h"
#include "../include/score_over.h"
//...............................................................................
class zeGameOver:public objet_gere < sprite_object >
{
	private:
		Sint32				go_deplace;                          // step number: 0, 1, 2 or 3
		Sint32				go_zetempo;                          // tempo before next step
		GIF_bitMap*			ptGfxFonte;
		score_over*			ptScorOver;
		Sint32				chrOffsetX;
	
		static const Sint32	SIZETSINUS = 200; 	     
		static const Sint32	sinus_over[SIZETSINUS];
		static const Sint32	zeus_over1[];
	    
	private:
		void				deplace_01();
		void				deplace_02();
		void				deplace_03();
		void				deplace_04();
    
	public:
							zeGameOver();
							~zeGameOver();
		score_over*			gtScorOver();
		Sint32				first_init(Sint32 offzt = 0);
		void				initialize(Sint32 iswin);
		void				execution1(Sint32 iswin = 0);

};
#endif

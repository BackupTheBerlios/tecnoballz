//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeFireBump.h"
// created		: ?
// updates		: 2004-09-04
// fonction	: manage bumper's fire
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
//
//******************************************************************************

#ifndef __ZEFIREBUMP__
#define __ZEFIREBUMP__

class zeFireBump;

#include "../include/objects_list.h"
#include "../include/handler_display.h"
#include "../include/barreScore.h"
#include "../include/tecno_fire.h"
#include "../include/sprite_paddle.h"


class zeFireBump:public objects_list < tecno_fire >
{
	private:
		static const Sint32	NUMBERFIRE = 7;
		static const Sint32	SIZEOFFIRE = 10;
		static Sint16		bob11_fire[];
		static const Sint16	scieSin640[];
		static const Sint16	scieSin320[];

	private:
		sprite_paddle*			maRaquette;
		Sint32				countTempo;
		Sint32				raketLarge;
		const Sint16*		scie_sinus;

	public:
							zeFireBump();
							~zeFireBump();
		Sint32				installBOB(sprite_paddle *raket);
		void				disponible();
		void				nouveauTir();
		void				init_type1();
		void				init_type2();
		void				init_type3();
		void				init_type4();
		void				init_type5();
		void				init_type6();
		void				init_type7();
		void				deplaceTir();
		void				move_type1();
		void				move_type2();
		void				move_type3();
		void				move_type4();
		void				move_type5();
		void				move_type6();
		void				move_type7();
		void				fire1RunOn();
		void				fire2RunOn();
};
#endif

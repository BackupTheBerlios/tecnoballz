//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeCongBall.h"
// created		: 2004-08-05
// updates		: 2004-08-05
// fonctions	: handle the "Techno Balls" (congratulations) 
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
#ifndef __ZECONGBALL__
#define __ZECONGBALL__
//...............................................................................
class zeCongBall;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/objet_gere.h"
#include "../include/score_over.h"
//...............................................................................
class zeCongBall:public objet_gere < sprite_object >
{	
	private:
			Sint32			speed_rad0;
			Sint32			speed_rad1;
			Sint32			speed_rad2;
			Sint32			speed_rad3;
			Sint32			speed_rad4;
	public:
							zeCongBall();
							~zeCongBall();
		void				initialize();
		void				execution1();

};
#endif

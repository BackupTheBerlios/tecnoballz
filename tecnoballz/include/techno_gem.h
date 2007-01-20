//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "techno_gem.h"
// created		: 2004-04-12
// updates		: 2004-10-13
// fonction	: handle gem
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
//
//******************************************************************************
#ifndef __TECHNO_GEM__
#define __TECHNO_GEM__
class techno_gem;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/tecno_fire.h"
//...............................................................................
enum 
{
	GREYSQUARE,
	GREENSPHER,
	YELLOWRING,
	BLUETRIANG,
	RHOMBUGOLD,
	PENTABRONZ
};

class techno_gem:public sprite_object
{
	friend class zeCapsules;
	
	private:
		Sint32				directionX;
		Sint32				la_vitesse;
		Sint32				indicator1;
		Sint32				typeof_gem;
		tecno_bump*			raquettePT;
		Sint32				blinkcount;
		Sint32				rand_count;
	
	public:
							techno_gem();
							~techno_gem();
		void				littleInit();
		Sint32				disponible(sprite_ball *pball);
		Sint32				disponible(tecno_fire *pfire);
		void				initialGem(Sint32 pos_x, Sint32 pos_y, 
								tecno_bump *raket);
		void				gemcollect(Sint32 ztype);
		void				activBlink();
		Sint32				deplaceMoi();
	
	private:
		static const Sint32	gem_random[8];
};
#endif

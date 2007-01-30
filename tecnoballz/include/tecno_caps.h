//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_caps.h"
// created		: ?
// updates		: 2004-04-11
// fonction	: capsules of money
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
#ifndef __TECNO_CAPS__
#define __TECNO_CAPS__
class tecno_caps;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/sprite_paddle.h"
#include "../include/barreScore.h"
#include "../include/handler_display.h"
#include "../include/controller_bricks.h"
#include "../include/sprite_ball.h"
#include "../include/sprite_projectile.h"
//...............................................................................
class tecno_caps:public sprite_object
{
	friend class zeCapsules;

	private:
		Sint32			directionX;
		Sint32			valeurFric;
		Sint32			la_vitesse;
		sprite_paddle*		raquettePT;
		Sint32			area_multi;
    
	public:
						tecno_caps();
						~tecno_caps();
		void			littleInit();
		Sint32			disponible(brickClear *briPT);
		Sint32			disponible(sprite_ball *pball);
		Sint32			disponible(sprite_projectile *pfire);
	
		void			init_money(Sint32 pos_x, Sint32 pos_y, 
							sprite_paddle *raket);
  
		Sint32			deplaceMoi();
		Sint32			deplaceMe2();
};
#endif

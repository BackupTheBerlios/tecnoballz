//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file			: "zeMissiles.h"
// created			: ?
// updates			: 2004-08-06
// fonction		: handle weapons of the guards (no gigablitz!)
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
//*****************************************************************************
#ifndef __ZEMISSILES__
#define __ZEMISSILES__
class zeMissiles;

#include "../include/list_sprites.h"
#include "../include/tecno_miss.h"
#include "../include/objects_list.h"
#include "../include/sprite_guardian.h"
#include "../include/sprite_paddle.h"
#include "../include/zexplosion.h"

class zeMissiles:public objects_list < tecno_miss >
{
	friend class controller_guardians;
		
	private:
		static const Sint16		tir01_posi[62];
		static const Sint16		tir02_posi[60];
		static const Sint16		tir04_posi[14];
		static const Sint16		tir10_posi[12];
		static const Sint16		fire_sinus[60];
		sprite_paddle*				ptbumper01;
		zexplosion*				pexplosion;
	
	public:
								zeMissiles(sprite_paddle *pBump, 
									zexplosion *pexpl);
								~zeMissiles();
		void					execution1();
		void					anim_fires();
		void					bumper_col();
		tecno_miss*				getWeapOne();
		void					newMissile(Sint32 nfire, sprite_guardian *pgard);
    
  private:
		void					tir01_init(sprite_guardian *pgard);
		void					tir02_init(sprite_guardian *pgard);
		void					tir03_init(sprite_guardian *pgard);
		void					tir04_init(sprite_guardian *pgard);
		void					tir05_init(sprite_guardian *pgard);
		void					tir06_init(sprite_guardian *pgard);
		void					tir07_init(sprite_guardian *pgard);
		void					tir08_init(sprite_guardian *pgard);
		void					tir09_init(sprite_guardian *pgard);
		void					tir10_init(sprite_guardian *pgard);
		void					tir11_init(sprite_guardian *pgard);
};
#endif

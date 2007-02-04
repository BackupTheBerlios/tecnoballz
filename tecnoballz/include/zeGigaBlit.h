//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "zeGigaBlit.h"
// created      : ?
// updates      : 2005-08-26
// fonctions    : manage the Gigablitz (bumper and guards)
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
#ifndef __ZEGIGABLIT__
#define __ZEGIGABLIT__
class zeGigaBlit;
//...............................................................................
#include "../include/giga_blitz.h"
#include "../include/list_sprites.h"
#include "../include/barreScore.h"
#include "../include/handler_keyboard.h"
#include "../include/ballDirect.h"
#include "../include/zeFireBump.h"
#include "../include/joueurData.h"
#include "../include/supervisor_bricks_level.h"
#include "../include/objects_list.h"
#include "../include/head_anima.h"
#include "../include/controller_bricks.h"
#include "../include/zexplosion.h"
//...............................................................................
class zeGigaBlit:public objects_list < giga_blitz >
{	friend class            giga_blitz;
 
	private:
		static const Uint32		NOMBREGIGA = 7;
		static Sint32			numeroBOBs[NOMBREGIGA];  
    
		controller_bricks*			brickObjet;
		head_anima*			gugusObjet;
		zeRaquette*			ptRaquette;
		zexplosion*			pexplosion;
		sprite_paddle*			tecBumper1;
		sprite_paddle*			tecBumper3;
		giga_blitz*			blitzobjet;	//pointer on the current "giga_blitz" 
		Sint32				blitz_colx;
		Sint32				blitz_posx;
		Sint32				blitz_posy;
		Sint32				blitz_haut;	//gigablitz height
		Sint32				bitz_ystop;	//"gigablitz" Y max.
		Sint32				bitz_maxiy;	// 
		Sint32				bitz_miniy;	// 
		Sint32				blitz_seta;
		Sint32				blitz_brik;	//width of collison bricks in row
		Uint32				blitz_xsin;
  
	public:
						zeGigaBlit();
						~zeGigaBlit();
		Sint32				init_liste(zeRaquette *zeRak, head_anima *gugus, 
							controller_bricks *brick);
		void				initDepart();
		void				execution1();
		void				collision1();
		Sint32				init_liste(zeRaquette *zeRak, zexplosion *pexpl);
		void				execution2();
		void 				collision2();
		Sint32				guard_shoot(Sint32 value, Sint32 pos_x, Sint32 pos_y,
								Sint32 large, Sint32 haute);
		Sint32				isactivate();
};
#endif

//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeBouiBoui.h"
// created		: ?
// updates		: 2004-08-29
// fonction	: manage the BouiBoui
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
#ifndef __ZEBOUIBOUI__
#define __ZEBOUIBOUI__
class zeBouiBoui ;
//...............................................................................
#include "../include/list_sprites.h"
#include "../include/sprite_ship.h"
#include "../include/objects_list.h"
#include "../include/handler_display.h"
#include "../include/zeCapsules.h"
#include "../include/zeGemstone.h"
#include "../include/controller_bricks.h"
//..............................................................................
class zeBouiBoui : public objects_list < sprite_ship >
{	friend class sprite_ship ;
	private:
		zeCapsules*		ptCapsules;
		ze_gadgets*		pt_gadgets;
		zeGemstone*		ptGemstone;
		controller_bricks*		pt_briques;
		Sint32			apparition[4]; 		
		Sint32			reappTemps;
		Sint32			power_init;
	public:
						zeBouiBoui(zeCapsules*, ze_gadgets*, zeGemstone*,
							controller_bricks*);
						~zeBouiBoui();
		void			initialise(Sint32 time0, Sint32 time1, Sint32 time2,
							Sint32 time3, Sint32 time4, Sint32 power) ;
		void			atom_depla();
		void			atomexplos();
};
#endif

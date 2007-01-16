//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zexplosion.h"
// created		: 2003-03-02
// updates		: 2004-06-27
// fonction	: handle explosions
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
//******************************************************************************
#ifndef __ZEXPLOSION__
#define __ZEXPLOSION__
//...............................................................................
class zexplosion;
//...............................................................................
#include "../include/liste_BOBs.h"
#include "../include/sprite_object.h"
#include "../include/objet_gere.h"
//...............................................................................
class zexplosion:public objet_gere < sprite_object >
{
	//friend class sprite_object;
	public:
							zexplosion();
							~zexplosion();
		Sint32				init_liste();
		void				add_explos(Sint32 pos_x, Sint32 pos_y);
		void				execution1();
	private:
		Uint32				vFrequency;
};
#endif

//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ze_magneye.h"
// created		: 2004-09-17
// updates		: 2004-09-17
// function	: manage eye magneto (only bricks levels)
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
//*****************************************************************************
#ifndef __ZE_MAGNEYE__
#define __ZE_MAGNEYE__
class ze_magneye ;
//...............................................................................
#include "../include/list_sprites.h"
#include "../include/techno_eye.h"
#include "../include/objet_gere.h"
//..............................................................................
class ze_magneye : public objet_gere < techno_eye >
{	friend class techno_eye ;

	private:
		static const Uint16	Xcoordinat[32];
		static const Uint16	Ycoordinat[32];

	public:
		Sint32			hypotenuse;
		Sint32			eyeCenterX;
		Sint32			eyeCenterY;
	
	public:
						ze_magneye();
						~ze_magneye();
		Sint32			create_eye();
		Sint32 			initialize();
		void			execution1();
};
#endif

//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ballDirect.h"
// created		: ?
// updates		: 2004-05-18
// fonction	: bumper's viewfinder   
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
//******************************************************************************
#ifndef __BALLDIRECT__
#define __BALLDIRECT__
//...............................................................................
class ballDirect;
//...............................................................................
#include "../include/objet_gere.h"
#include "../include/tecno_bump.h"
#include "../include/zeRaquette.h"
//...............................................................................
class ballDirect:public objet_gere < BOB_killer >
{
	private:
		tecno_bump**		listBumper;	//list of "tecno_bump" objets
		Sint32				numBumpers;	//number of bumper(s) 4 or 1 
			
	public:
							ballDirect();
							~ballDirect();
		Sint32				initialize(zeRaquette* pBump, Uint32 nBump);
		void				execution1();
};

#endif

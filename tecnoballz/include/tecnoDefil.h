//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecnoDefil.h"
// created		: ?
// updates		: 2004-06-13
// fonctions	: chars of the menu scrolling text
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
//*****************************************************************************
#ifndef __TECNODEFIL__
#define __TECNODEFIL__
//...............................................................................
#include "../include/BOB_killer.h"
//...............................................................................
class tecnoDefil:public virtual BOB_killer
{
	friend class zeDefilTxt;
	protected:
		Sint32				zeCosValue;
		Sint32				zeSinValue;

	public:
							tecnoDefil();
							~tecnoDefil();
};
#endif

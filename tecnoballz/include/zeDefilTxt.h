//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeDefilTxt.h"
// created		: ?
// updates		: 2004-05-23
// fonctions	: management of menu scrolling text
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
#ifndef __ZEDEFILTXT__
#define __ZEDEFILTXT__

#include "../include/objet_gere.h"
#include "../include/tecnoDefil.h"
class zeDefilTxt:public objet_gere < tecnoDefil >
{
	private:
		static const Sint32	DEFINUMBER = 14;	//numbers maximum characters
		static char			scrollText[];
		static char			asciiToBob[128];

	private:
		tecnoDefil*			objectChar[DEFINUMBER + 2];
		tecnoDefil*			objectLeft;
		tecnoDefil*			objectRigh;
		tecnoDefil**		object_ptr;
		char*				scroll_ptr;
		Sint32				offset_xx1;
		Sint32				offset_yy1;

	public:
							zeDefilTxt();
							~zeDefilTxt();
		Sint32				init_chars();
		void				move_chars();

};
#endif

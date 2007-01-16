//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_text.h"
// created		: ?
// updates		: 2004-05-20
// fonction	: handle mobiles characters ("LEVEL x COMPLETED")
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
#ifndef __TECNO_TEXT__
#define __TECNO_TEXT__
//...............................................................................
class tecno_text;
//...............................................................................
#include "../include/sprite_object.h"
//...............................................................................
class tecno_text:public sprite_object
{
	friend class zeMoveText;

	private:
		Sint32				angleValue;	//radius value 0 to 511
		Sint32				posiYStart;	//start Y coordinate
		Sint32				posiX_stop;	//stop X coordinate
		Sint32				xIncrement;	//X move offset(1, -1 or 0)
		Sint32				yIncrement;	//Y move offset (1)
		Sint32				posiY_stop;	//stop Y coordinate

	public:
							tecno_text();
							~tecno_text();
		void				littleInit(Sint32 a, Sint32 x, Sint32 y, Sint32 i,
										Sint32 l, Sint32 s);
		void				moveCaract();
};
#endif

//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeMoveText.h"
// created		: ?
// updates		: 2004-05-20
// fonction	: manage mobiles characters ("LEVEL x COMPLETED")
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
#ifndef __ZEMOVETEXT__
#define __ZEMOVETEXT__
//...............................................................................
class zeMoveText;
//...............................................................................
#include "../include/objet_gere.h"
#include "../include/tecno_text.h"
//...............................................................................
class zeMoveText:public objet_gere < tecno_text >
{
	private:
		Sint32				size_line1;
		Sint32				size_line2;
		Sint32				size_total;
		Sint32				horz_large;
		static char			ze_bobText[];

	public:
							zeMoveText();
							~zeMoveText();
		void				initialise(Sint32 level);
		Sint32				startValue(Sint32 n, Sint32 a, Sint32 j, Sint32 y,
										Sint32 i3, Sint32 y3);
		void				goMoveText();
		void				activeText();
};
#endif

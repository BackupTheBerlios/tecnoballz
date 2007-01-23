//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_text.cc"
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
#include "../include/tecno_text.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
tecno_text::tecno_text()
{
	BOBprepare();
	littleInit(0, 0, 0, 0, 0, 0);
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
tecno_text::~tecno_text()
{
}

//-----------------------------------------------------------------------------
// initialize radius, stop coordinate and start y coordinate
//-----------------------------------------------------------------------------
void tecno_text::littleInit(Sint32 zeRad, Sint32 xStop, Sint32 yStrt, Sint32 xOffs,
								Sint32 yOffs, Sint32 yStop)
{
	angleValue = zeRad;	//radius (pt/sinus table)
	posiX_stop = xStop;	//stop x coordinate
	posiYStart = yStrt;	//start y coordinate
	xIncrement = xOffs;	//X move offset(1, -1 or 0)
	yIncrement = yOffs;	//Y move offset (1)
	posiY_stop = yStop;	//stop Y coordinate
}

//-----------------------------------------------------------------------------
// characters moving
//-----------------------------------------------------------------------------
void tecno_text::moveCaract()
{
	if(is_enabled)
	{	
		// ordinate linear moving
		Sint32 d = posiYStart;
		if(d > posiY_stop)
		{	d += yIncrement;
			posiYStart = d;
		}

		// ordinate sinus moving
		Sint32 a = angleValue + 5;
		a &= SINUS_MASK;
		angleValue = a;
		Sint16 *s = table_sinL + a;
		a = *s;
		a *= 10 * resolution;
		a >>= SINUS_DECA;
		a = a + d;
		y_coord = a;

		//absciss moving
		if(x_coord != posiX_stop)
			x_coord += xIncrement;
	}
	clip_coordinates();
}

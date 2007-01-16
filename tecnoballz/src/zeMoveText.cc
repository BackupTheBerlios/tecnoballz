//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeMoveText.cc"
// created	: ?
// updates	: 2005-01-23
// fonction	: manage mobiles characters ("LEVEL x COMPLETED")
// id		: $Id: zeMoveText.cc,v 1.5 2007/01/16 16:57:31 gurumeditation Exp $
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
#include "../include/zeMoveText.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
zeMoveText::zeMoveText()
{
	littleInit();
	size_line1 = 0;
	size_line2 = 0;
	horz_large = 0;
	objetTotal = 20;
	objects_have_shades = true;
	BOBtypeNum = BOB_LETTRE;
	chrOffsetX = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
zeMoveText::~zeMoveText()
{
	littleDead();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
void zeMoveText::initialise(Sint32 level, Sint32 offzt)
{
	chrOffsetX = offzt;
	horz_large = 256 * resolution;
	char *monPT = ze_bobText;
	//if(level <= 12)
	{	Sint32 d = level / 10;
		Sint32 u = level - d * 10;
		ze_bobText[6] = (char)d + '\\';
		ze_bobText[7] = (char)u + '\\';
	}	
	/*else
	{	monPT = ze_endText;
	}*/
		
	//###################################################################
	// count the number of characters
	//###################################################################
	size_line1 = 0;
	while (*(monPT++))
		size_line1++;
	size_line2 = 0;
	while (*(monPT++))
		size_line2++;
	size_total = size_line1 + size_line2;

	//###################################################################
	// initialize the images for each character
	//###################################################################
	monPT = ze_bobText;
	for(Sint32 i = 0; i < size_total; i++)
	{	tecno_text *chara = objetListe[i];
		char c = *(monPT++);
		if(c == '\0')
			c = *(monPT++);
		chara->new_offset(c - 'A');
	}
	
	//###################################################################
	// initialize characters of "LEVEL n"
	//###################################################################
	Sint32 yStrt = 200 * resolution;
	Sint32 a =	startValue(size_line1, 0, 0,
		yStrt, -6 * resolution, 60  * resolution);

	//###################################################################
	// initialize characters of "COMPLETED"
	//###################################################################
	yStrt += 10 * resolution;
	startValue(size_line2, a, size_line1,
		yStrt, -8 * resolution , 80 * resolution);
}

//-----------------------------------------------------------------------------
// initialize start values
// input	=> nchar: number of chars peer line
//			=> zerad: radius
//			=> index: 
//			=> yStrt: start y coordinate
//			=> yOffs: Y move offset 
//			=> yStop: stop Y coordinate
//-----------------------------------------------------------------------------
Sint32 zeMoveText::startValue(Sint32 nchar, Sint32 zeRad, Sint32 index,
				Sint32 yStrt, Sint32 yOffs, Sint32 yStop)
{
	Sint32 width = objetListe[0]->getHauteur();
	Sint32 e = (horz_large) / nchar;
	Sint32 xStop = (horz_large - (nchar * width)) / 2;
	Sint32 xOffs = 0;	//X move offset (1, -1 or 0)
	Sint32 xStrt = e / 2;	//start X coordinate

	xStop += chrOffsetX;
	xStrt += chrOffsetX;
	
	for(Sint32 i = index; i < (nchar + index); i++, xStrt += e, xStop += width)
	{	tecno_text *chara = objetListe[i];
		chara->coordonnee(xStrt, yStrt);
		if(xStrt > xStop)
			xOffs = -1;
		else
		{	if(xStrt == xStop)
				xOffs = 0;
			else
				xOffs = 1;
		}
		chara->littleInit(zeRad, xStop, yStrt, xOffs, yOffs, yStop);
		zeRad += 8;
		zeRad &= SINUS_MASK;
	}
	return zeRad;
}

//-----------------------------------------------------------------------------
// animation of characters sprites
//-----------------------------------------------------------------------------
void zeMoveText::goMoveText()
{
	for(Sint32 i = 0; i < size_total; i++)
	{	tecno_text *chara = objetListe[i];
		chara->moveCaract();
	}
}

//-----------------------------------------------------------------------------
// enable characters sprites
//-----------------------------------------------------------------------------
void zeMoveText::activeText()
{
	for(Sint32 i = 0; i < size_total; i++)
	{	tecno_text *chara = objetListe[i];
		chara->enable();
	}
}

//-----------------------------------------------------------------------------
// string to display
//-----------------------------------------------------------------------------
char zeMoveText::ze_bobText[] = "LEVEL[//\0COMPLETED\0";
char zeMoveText::ze_endText[] = "[[GAME[[\0FINISHED[\0";

//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_miss.cc"
// created		: ?
// updates		: 2004-05-14
// function	: handle weapons of guards
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
#include "../include/tecno_miss.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
tecno_miss::tecno_miss()
{
	BOBprepare();
	type_depla = 0;
	flagDepla1 = 0;
	flagDepla2 = 0;
	flagDepla3 = 0;
	flagDepla4 = 0;
	flagDepla5 = 0;
	flagDepla6 = 0;
	flagDepla7 = 0;
	tir_maxi_x = 303 * resolution;
	tir_maxi_y = 240 * resolution;
	tir_minixy = 2 * resolution;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
tecno_miss::~tecno_miss()
{
}

//------------------------------------------------------------------------------
// select a trajectory
//------------------------------------------------------------------------------
void tecno_miss::execution1()
{
	if(flag_actif)
	{ 
		switch(flag_actif)
		{	case 1:
				dmissile01();
				break;
			case 2:
				dmissile02();
				break;
			case 3:
				dmissile03();
				break;
			case 4:
				dmissile04();
				break;
			case 5:
				dmissile05();
				break;
			case 6:
				dmissile06();
				break;
			case 7:
				dmissile07();
				break;
			case 8:
				dmissile08();
				break;
			case 9:
				dmissile09();
				break;
			case 10:
				dmissile10();
				break;
			case 11:
				dmissile11();
				break;
			case 12:
				dmissile12();
				break;
		}
	}
}

//------------------------------------------------------------------------------
// weapons: trajectory number 1
//------------------------------------------------------------------------------
void tecno_miss::dmissile01()
{ 
	position_y = position_y + (4 * resolution);
	Sint32 v = tablesinus[++flagDepla1];
	if(v == 99)
	{	flagDepla1 = 0;
		v = 32;
	}
	position_x = flagDepla2 + (v * resolution);
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 2
//------------------------------------------------------------------------------
void tecno_miss::dmissile02()
{
	flagDepla3 = flagDepla3 + 2;
	if(tablesinus[flagDepla3] == 99)	//end of the table?
		flagDepla3 = 0;
	flagDepla2 = flagDepla2 + (4 * resolution);	//new position y 
	position_y = flagDepla2 + (tablesinus[flagDepla3+1] * resolution);
	position_x = flagDepla1 + (tablesinus[flagDepla3] * resolution);
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 3
//------------------------------------------------------------------------------
void tecno_miss::dmissile03()
{
	position_y = position_y + (4 * resolution);
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 4
//------------------------------------------------------------------------------
void tecno_miss::dmissile04()
{
	if(flagDepla5)
	{	if(--flagDepla4 <= 0)
		{	flagDepla4 = flagDepla5;
			flagDepla5--;
			flagDepla3 = flagDepla3 + 2;
			if(tablesinus[flagDepla3] == 99)	//end of the table?
				flagDepla3 = 0;
			position_y = flagDepla2 + (tablesinus[flagDepla3+1] * resolution);
			position_x = flagDepla1 + (tablesinus[flagDepla3] * resolution);
		}
	}
	else
	{	position_x -= flagDepla6;
		position_y += flagDepla7;
	}
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 5
//------------------------------------------------------------------------------
void tecno_miss::dmissile05()
{
	if(flagDepla1)
	{	flagDepla1--;
		position_y += resolution;
	}
	else
	{	position_x -= flagDepla2;
		position_y += flagDepla3;
	}
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 6
//------------------------------------------------------------------------------
void tecno_miss::dmissile06()
{
	flagDepla3 = flagDepla3 + 2;
	if(tablesinus[flagDepla3] == 99)	//end of the table?
	flagDepla3 = 0;
	flagDepla2 = flagDepla2 + (3 * resolution);	//new position y 
	position_y = flagDepla2 + (tablesinus[flagDepla3+1] * resolution);
	if(flagDepla2 < (150 * resolution))
	{	flagDepla1 = flagDepla1 + 2 * resolution;
		if(ptbumper01->position_x < flagDepla1)
			flagDepla1 = flagDepla1 - 4 * resolution;
	}
	position_x = flagDepla1 + (tablesinus[flagDepla3] * resolution);
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 7
//------------------------------------------------------------------------------
void tecno_miss::dmissile07()
{
	position_y = position_y + (3 * resolution);	//new position y 
	if(position_y < (150 * resolution))
	{	position_x = position_x + 2 * resolution;
		if(ptbumper01->position_x < position_x)
			position_x = position_x - 4 * resolution;
	}
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 8 - infernal circle (circle which grows)
//------------------------------------------------------------------------------
void tecno_miss::dmissile08()
{
	flagDepla3 = flagDepla3 + 2;
	if(flagDepla3 >= 360)
		flagDepla3 = 0;
	Sint32 x = tablesinus[flagDepla3] * flagDepla4;
	Sint32 y = tablecosin[flagDepla3] * flagDepla4;
	x = x >> 7;
	y = y >> 7;
	position_x = ptguardian->position_x + ptguardian->gard_xcent + x;
	position_y = ptguardian->position_y + ptguardian->gard_ycent + y;
	flagDepla4 += resolution;
	screenOver(tir_minixy);
}


//------------------------------------------------------------------------------
// weapons: trajectory number 9 - sucker circle (circle which rebounds )
//------------------------------------------------------------------------------
void tecno_miss::dmissile09()
{
	flagDepla3 = flagDepla3 + 4;
	if(flagDepla3 >= 360)
		flagDepla3 = 0;
	Sint32 r = (16 * resolution);
	Sint32 x = tablesinus[flagDepla3] * r;
	Sint32 y = tablecosin[flagDepla3] * r;
	x = x >> 7;
	y = y >> 7;
	position_x = flagDepla1 + x;
	position_y = flagDepla2 + y;
	flagDepla2 += resolution;
	if(flagDepla4 > 0)
	{	flagDepla1 += (5 * resolution);
		if(flagDepla1 > (284 * resolution))
			flagDepla4 = 0;
	}
	else
	{	flagDepla1 -= (5 * resolution);
		if(flagDepla1 < (-80 * resolution))
			flagDepla4 = 1;
	}
	screenStop(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 10 - elipse
//------------------------------------------------------------------------------
void tecno_miss::dmissile10()
{
	flagDepla3 = flagDepla3 + 6;
	if(flagDepla3 >= 360)
		flagDepla3 -= 360;
	Sint32 ray_x = flagDepla4;
	Sint32 ray_y = ray_x - (3 * resolution);
	if(--flagDepla5 <= 0)
	{	flagDepla5 = 3;
		flagDepla4 += resolution;
	}
	Sint32 x = tablesinus[flagDepla3] * ray_x;
	Sint32 y = tablecosin[flagDepla3] * ray_y;
	x = x >> 7;
	y = y >> 7;
	position_x = flagDepla1 + x;
	position_y = flagDepla2 + y;
	flagDepla2 += 3 * resolution;
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 11 - triangle I
//------------------------------------------------------------------------------
void tecno_miss::dmissile11()
{
	flagDepla3 = flagDepla3 + 4;
	if(flagDepla3 >= 360)
		flagDepla3 -= 360;
	Sint32 r = (11 * resolution);
	Sint32 x = tablesinus[flagDepla3] * r;
	Sint32 y = tablecosin[flagDepla3] * r;
	x = x >> 7;
	y = y >> 7;
	position_x = flagDepla1 + x;
	position_y = flagDepla2 + y;
	flagDepla2 += (resolution*3);
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 12 - triangle II
//------------------------------------------------------------------------------
void tecno_miss::dmissile12()
{
	flagDepla3 = flagDepla3 + 4;
	if(flagDepla3 >= 360)
		flagDepla3 -= 360;
	Sint32 r = (BOBlargeur * resolution);
	Sint32 x = tablesinus[flagDepla3] * r;
	Sint32 y = tablecosin[flagDepla3] * r;
	x = x >> 7;
	y = y >> 7;
	position_x = flagDepla1 + x;
	position_y = flagDepla2 + y;
	flagDepla1 += flagDepla4;
	flagDepla2 += flagDepla5;
	screenOver(tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: overflow test
//------------------------------------------------------------------------------
void tecno_miss::screenOver(Sint32 vmini)
{
	if(position_x > tir_maxi_x || position_x < vmini || 
		position_y < vmini || position_y > tir_maxi_y )
		flag_actif = 0;
}

//------------------------------------------------------------------------------
// weapons: overflow test
//------------------------------------------------------------------------------
void tecno_miss::screenStop(Sint32 vmini)
{	
	if(position_y>tir_maxi_y)
		flag_actif = 0;
	else
	{	if(position_y < vmini)
			position_y = vmini;
		if(position_x > tir_maxi_x)
			position_x = tir_maxi_x;
		else
		{	if(position_x < vmini)
			position_x = vmini;
		}
	}
}

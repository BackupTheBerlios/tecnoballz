//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_caps.cc"
// created		: ?
// updates		: 2004-10-23
// fonction	: capsules of money
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
//
//******************************************************************************
#include "../include/tecno_caps.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
tecno_caps::tecno_caps()
{
	clear_sprite_members();
	directionX = 0;
	valeurFric = 0;
	la_vitesse = 0;
	raquettePT = (tecno_bump *) NULL;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
tecno_caps::~tecno_caps()
{
}

//-----------------------------------------------------------------------------
// perform some initialization
//-----------------------------------------------------------------------------
void tecno_caps::littleInit()
{
	x_maximum = screen_width - ((64 + 16) * resolution);
	x_minimum = 3 * resolution;
	y_maximum = screen_height - 10  * resolution;
	y_minimum = 0 * resolution;
	frame_period = 5;			//animation delay 
	frame_delay = 1;			//animation delay counter
	
	//###################################################################
	//initialize multiplier of the value of the capsules of money 
	//###################################################################
	switch (joueurGere->getAreaNum())
	{	default:
			area_multi = 1;
			break;
		case 3:
		case 4:
			area_multi = 2;
			break;
		case 5:
			area_multi = 4;
			break;
	}
}

//-----------------------------------------------------------------------------
// initialize a new capsule of money from a bricks
//-----------------------------------------------------------------------------
Sint32 tecno_caps::disponible(brickClear *briPT)
{
	if(is_enabled)
		return 0;
	init_money(briPT->balle_posX, briPT->balle_posY, briPT->raquettePT);
	return 1;

}

//-----------------------------------------------------------------------------
// initialize a new capsule of money from a Bouiboui or Gards
//-----------------------------------------------------------------------------
Sint32 tecno_caps::disponible(sprite_ball *pball)
{
	if(is_enabled)
		return 0;
	init_money(pball->x_coord, pball->y_coord, pball->raket_ball);
	return 1;
}

//-----------------------------------------------------------------------------
// initialize a new capsule of money from a Bouiboui 
//-----------------------------------------------------------------------------
Sint32 tecno_caps::disponible(tecno_fire *pfire)
{
	if(is_enabled)
		return 0;
	init_money(pfire->x_coord, pfire->y_coord, pfire->raquettePT);
	return 1;
}

//-----------------------------------------------------------------------------
// initialize a new capsule of money
//-----------------------------------------------------------------------------
void tecno_caps::init_money(Sint32 pos_x, Sint32 pos_y, tecno_bump *raket)
{
	is_enabled = 1;
	x_coord = pos_x;
	y_coord = pos_y;
	raquettePT = raket;
	Sint32 i = hasard_val;
	i = i & 0x003;
	hasard_val += i;
	directionX = raket->bumper_num();
	switch (i)
	{	case 0:
			//printf("tecno_caps::init_money(): 10 * %i\n", area_multi);
			valeurFric = 10 * area_multi;
			la_vitesse = resolution;
			break;
		case 1:
			//printf("tecno_caps::init_money(): 20 * %i\n", area_multi);
			valeurFric = 20 * area_multi;
			la_vitesse = resolution * 2;
			break;
		case 2:
			//printf("tecno_caps::init_money(): 30 * %i\n", area_multi);
			valeurFric = 30 * area_multi;
			la_vitesse = resolution * 3;
			break;
		default:
			//printf("tecno_caps::init_money(): 20 * %i\n", area_multi);
			valeurFric = 20 * area_multi;
			la_vitesse = resolution * 2;
			break;
	}
}

//-----------------------------------------------------------------------------
// bricks level: displacement and collision of the money
//-----------------------------------------------------------------------------
Sint32 tecno_caps::deplaceMoi()
{
	if(is_enabled)
	{	Sint32 i = la_vitesse;
		tecno_bump *raket = raquettePT;
		switch (directionX)
		{
			//###########################################################
			// bottom bumper
			//###########################################################
			case 1:
				y_coord += i;
				if(y_coord < y_maximum)
				{	if(collision1(raket))
					{	is_enabled = 0;
#ifndef SOUNDISOFF
						audio->play_sound(S_MONNAIES);
#endif
						return (valeurFric);
					}
				}
				else
					is_enabled = 0;
				break;

			//###########################################################
			// right bumper          
			//###########################################################
			case 2:
				x_coord += i;
				if(x_coord < x_maximum)
				{	if(collision1(raket))
					{	is_enabled = 0;
#ifndef SOUNDISOFF
						audio->play_sound(S_MONNAIES);
#endif
						return (valeurFric);
					}
				}
				else
					is_enabled = 0;
				break;

			//###########################################################
			// top bumper
			//###########################################################
			case 3:
				y_coord -= i;
				if(y_coord > y_minimum)
				{	if(collision1(raket))
					{	is_enabled = 0;
#ifndef SOUNDISOFF
						audio->play_sound(S_MONNAIES);
#endif
						return (valeurFric);
					}
				}
				else
					is_enabled = 0;
				break;

			//###########################################################
			// left bumper
			//###########################################################
			case 4:
				x_coord -= i;
				if(x_coord > x_minimum)
				{	if(collision1(raket))
					{	is_enabled = 0;
#ifndef SOUNDISOFF
						audio->play_sound(S_MONNAIES);
#endif
						return (valeurFric);
					}
				}
				else
					is_enabled = 0;
				break;
		}
	}
	return (0);
}

//-----------------------------------------------------------------------------
// guards level: displacement and collision of the money
//-----------------------------------------------------------------------------
Sint32 tecno_caps::deplaceMe2()
{
	if(is_enabled)
	{	Sint32 i = la_vitesse;
		tecno_bump *raket = raquettePT;
		y_coord += i;
		if(y_coord < y_maximum)
		{	if(collision1(raket))
			{	is_enabled = 0;
#ifndef SOUNDISOFF
				audio->play_sound(S_MONNAIES);
#endif
				return valeurFric;
			}
		}
		else
			is_enabled = 0;
	}
	return 0;
}

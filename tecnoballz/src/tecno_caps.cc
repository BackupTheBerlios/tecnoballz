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
	BOBprepare();
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
	maximum_X1 = ecranLarge - ((64 + 16) * resolution);
	minimum_X1 = 3 * resolution;
	maximum_Y1 = ecranHaute - 10  * resolution;
	minimum_Y1 = 0 * resolution;
	init_tempo = 5;			//animation delay 
	anim_tempo = 1;			//animation delay counter
	
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
	if(flag_actif)
		return 0;
	init_money(briPT->balle_posX, briPT->balle_posY, briPT->raquettePT);
	return 1;

}

//-----------------------------------------------------------------------------
// initialize a new capsule of money from a Bouiboui or Gards
//-----------------------------------------------------------------------------
Sint32 tecno_caps::disponible(sprite_ball *pball)
{
	if(flag_actif)
		return 0;
	init_money(pball->position_x, pball->position_y, pball->raket_ball);
	return 1;
}

//-----------------------------------------------------------------------------
// initialize a new capsule of money from a Bouiboui 
//-----------------------------------------------------------------------------
Sint32 tecno_caps::disponible(tecno_fire *pfire)
{
	if(flag_actif)
		return 0;
	init_money(pfire->position_x, pfire->position_y, pfire->raquettePT);
	return 1;
}

//-----------------------------------------------------------------------------
// initialize a new capsule of money
//-----------------------------------------------------------------------------
void tecno_caps::init_money(Sint32 pos_x, Sint32 pos_y, tecno_bump *raket)
{
	flag_actif = 1;
	position_x = pos_x;
	position_y = pos_y;
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
	if(flag_actif)
	{	Sint32 i = la_vitesse;
		tecno_bump *raket = raquettePT;
		switch (directionX)
		{
			//###########################################################
			// bottom bumper
			//###########################################################
			case 1:
				position_y += i;
				if(position_y < maximum_Y1)
				{	if(collision1(raket))
					{	flag_actif = 0;
#ifndef SOUNDISOFF
						audio->play_sound(S_MONNAIES);
#endif
						return (valeurFric);
					}
				}
				else
					flag_actif = 0;
				break;

			//###########################################################
			// right bumper          
			//###########################################################
			case 2:
				position_x += i;
				if(position_x < maximum_X1)
				{	if(collision1(raket))
					{	flag_actif = 0;
#ifndef SOUNDISOFF
						audio->play_sound(S_MONNAIES);
#endif
						return (valeurFric);
					}
				}
				else
					flag_actif = 0;
				break;

			//###########################################################
			// top bumper
			//###########################################################
			case 3:
				position_y -= i;
				if(position_y > minimum_Y1)
				{	if(collision1(raket))
					{	flag_actif = 0;
#ifndef SOUNDISOFF
						audio->play_sound(S_MONNAIES);
#endif
						return (valeurFric);
					}
				}
				else
					flag_actif = 0;
				break;

			//###########################################################
			// left bumper
			//###########################################################
			case 4:
				position_x -= i;
				if(position_x > minimum_X1)
				{	if(collision1(raket))
					{	flag_actif = 0;
#ifndef SOUNDISOFF
						audio->play_sound(S_MONNAIES);
#endif
						return (valeurFric);
					}
				}
				else
					flag_actif = 0;
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
	if(flag_actif)
	{	Sint32 i = la_vitesse;
		tecno_bump *raket = raquettePT;
		position_y += i;
		if(position_y < maximum_Y1)
		{	if(collision1(raket))
			{	flag_actif = 0;
#ifndef SOUNDISOFF
				audio->play_sound(S_MONNAIES);
#endif
				return valeurFric;
			}
		}
		else
			flag_actif = 0;
	}
	return 0;
}

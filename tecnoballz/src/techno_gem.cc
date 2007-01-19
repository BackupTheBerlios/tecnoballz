//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "techno_gem.cc"
// created		: 2004-04-12
// updates		: 2004-10-23
// fonction	: handle gem
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
#include "../include/techno_gem.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
techno_gem::techno_gem()
{
	BOBprepare();
	directionX = 0;
	la_vitesse = 0;
	indicator1 = 0;
	blinkcount = 0;
	rand_count = 0;
	typeof_gem = GREYSQUARE;
	raquettePT = (tecno_bump *) NULL;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
techno_gem::~techno_gem()
{
}

//-----------------------------------------------------------------------------
// Perform some initializations
//-----------------------------------------------------------------------------
void techno_gem::littleInit()
{
	/*maximum_X1 = ecranLarge - 192;
	minimum_X1 = 3 * resolution;
	maximum_Y1 = ecranHaute - 20;
	minimum_Y1 = 10 * resolution;*/
	maximum_X1 = ecranLarge - ((64 + 16) * resolution);
	minimum_X1 = 3 * resolution;
	maximum_Y1 = ecranHaute - 10  * resolution;
	minimum_Y1 = 0 * resolution;
	
}

//-----------------------------------------------------------------------------
// initialize a new gem
//-----------------------------------------------------------------------------
Sint32 techno_gem::disponible(technoBall *pball)
{
	if(flag_actif)
		return 0;
	initialGem(pball->position_x, pball->position_y, pball->raket_ball);
	return 1;
}

//-----------------------------------------------------------------------------
// initialize a new gem
//-----------------------------------------------------------------------------
Sint32 techno_gem::disponible(tecno_fire *pfire)
{
	if(flag_actif)
		return 0;
	initialGem(pfire->position_x, pfire->position_y, pfire->raquettePT);
	return 1;
}

//-----------------------------------------------------------------------------
// initialize a new gem
//-----------------------------------------------------------------------------
void techno_gem::initialGem(Sint32 pos_x, Sint32 pos_y, tecno_bump *raket)
{
	flag_actif = 1;
	position_x = pos_x;
	position_y = pos_y;
	raquettePT = raket;
	directionX = raket->bumper_num();
	la_vitesse = resolution;
	Sint32 h = (hasard_val >> 4 + rand_count++) & 7;
	hasard_val += pos_x;
	h = gem_random[h];
	//h = BOBListNum;		// for tests only !
	typeof_gem = h;
	change_GFX(h);
	indicator1 = 0;
	flagShadow = 1;
	blinkcount = 0;
}

//-----------------------------------------------------------------------------
// a new gem was collected
//-----------------------------------------------------------------------------
void techno_gem::gemcollect(Sint32 ztype)
{
	typeof_gem = ztype;
	change_GFX(ztype);
	flagShadow = 0;
	indicator1 = 1;
	position_y = ecranHaute - BOBhauteur - 2 * resolution;
	position_x = 270 * resolution + BOBlargeur * ztype;
	flag_actif = 1;
	blinkcount = 0;
}

//-----------------------------------------------------------------------------
// active blink
//-----------------------------------------------------------------------------
void techno_gem::activBlink()
{
	if(flag_actif && indicator1)
		blinkcount = 30;
}

//-----------------------------------------------------------------------------
// move or blink gem
//-----------------------------------------------------------------------------
Sint32 techno_gem::deplaceMoi()
{
	if(flag_actif && !indicator1)
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
						return typeof_gem;
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
						return typeof_gem;
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
						return typeof_gem;
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
						return typeof_gem;
					}
				}
				else
					flag_actif = 0;
				break;
		}
	}
	else

	//###########################################################
	// blink gem
	//###########################################################
	{	if (blinkcount > 0 && indicator1)
		{	if (blinkcount > 20)
				flag_actif = 0;
			else
				flag_actif = 1;
			if (-- blinkcount <= 0)
				blinkcount = 30;
		
		}
	}
	return -1;
}


const Sint32 techno_gem::gem_random[8] = 
{	GREYSQUARE, GREENSPHER, YELLOWRING, BLUETRIANG,
	RHOMBUGOLD, PENTABRONZ, GREYSQUARE, GREENSPHER
};

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
	clear_sprite_members();
	directionX = 0;
	la_vitesse = 0;
	indicator1 = 0;
	blinkcount = 0;
	rand_count = 0;
	typeof_gem = GREYSQUARE;
	raquettePT = (sprite_paddle *) NULL;
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
	/*x_maximum = screen_width - 192;
	x_minimum = 3 * resolution;
	y_maximum = screen_height - 20;
	y_minimum = 10 * resolution;*/
	x_maximum = screen_width - ((64 + 16) * resolution);
	x_minimum = 3 * resolution;
	y_maximum = screen_height - 10  * resolution;
	y_minimum = 0 * resolution;
	
}

//-----------------------------------------------------------------------------
// initialize a new gem
//-----------------------------------------------------------------------------
Sint32 techno_gem::disponible(sprite_ball *pball)
{
	if(is_enabled)
		return 0;
	initialGem(pball->x_coord, pball->y_coord, pball->raket_ball);
	return 1;
}

//-----------------------------------------------------------------------------
// initialize a new gem
//-----------------------------------------------------------------------------
Sint32 techno_gem::disponible(sprite_projectile *pfire)
{
	if(is_enabled)
		return 0;
	initialGem(pfire->x_coord, pfire->y_coord, pfire->raquettePT);
	return 1;
}

//-----------------------------------------------------------------------------
// initialize a new gem
//-----------------------------------------------------------------------------
void techno_gem::initialGem(Sint32 pos_x, Sint32 pos_y, sprite_paddle *raket)
{
	is_enabled = 1;
	x_coord = pos_x;
	y_coord = pos_y;
	raquettePT = raket;
	directionX = raket->bumper_num();
	la_vitesse = resolution;
	Sint32 h = (hasard_val >> 4 + rand_count++) & 7;
	hasard_val += pos_x;
	h = gem_random[h];
	//h = BOBListNum;		// for tests only !
	typeof_gem = h;
	set_image(h);
	indicator1 = 0;
	sprite_has_shadow = 1;
	blinkcount = 0;
}

//-----------------------------------------------------------------------------
// a new gem was collected
//-----------------------------------------------------------------------------
void techno_gem::gemcollect(Sint32 ztype)
{
	typeof_gem = ztype;
	set_image(ztype);
	sprite_has_shadow = 0;
	indicator1 = 1;
	y_coord = screen_height - sprite_height - 2 * resolution;
	x_coord = 270 * resolution + sprite_width * ztype;
	is_enabled = 1;
	blinkcount = 0;
}

//-----------------------------------------------------------------------------
// active blink
//-----------------------------------------------------------------------------
void techno_gem::activBlink()
{
	if(is_enabled && indicator1)
		blinkcount = 30;
}

//-----------------------------------------------------------------------------
// move or blink gem
//-----------------------------------------------------------------------------
Sint32 techno_gem::deplaceMoi()
{
	if(is_enabled && !indicator1)
	{	Sint32 i = la_vitesse;
		sprite_paddle *raket = raquettePT;
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
						return typeof_gem;
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
						return typeof_gem;
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
						return typeof_gem;
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
						return typeof_gem;
					}
				}
				else
					is_enabled = 0;
				break;
		}
	}
	else

	//###########################################################
	// blink gem
	//###########################################################
	{	if (blinkcount > 0 && indicator1)
		{	if (blinkcount > 20)
				is_enabled = 0;
			else
				is_enabled = 1;
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

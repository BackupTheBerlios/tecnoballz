//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeCapsules.cc"
// created		: ?
// updates		: 2004-10-15
// fonction	: Manage the capsules of money
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

#include "../include/zeCapsules.h"

//-----------------------------------------------------------------------------
// Create the object
//-----------------------------------------------------------------------------
zeCapsules::zeCapsules()
{
	littleInit();
	max_of_sprites = 6;
	sprites_have_shades = true;
	sprite_type_id = BOB_MONEYS;
}

//-----------------------------------------------------------------------------
// Release the object
//-----------------------------------------------------------------------------
zeCapsules::~zeCapsules()
{
	release_sprites_list();
}

//-----------------------------------------------------------------------------
// bricks levels: initialize the capsules before one level
//-----------------------------------------------------------------------------
void zeCapsules::initialise(Sint32 frequ, barreScore *score, printmoney *money)
{
	frequenceX = frequ;
	//printf("zeCapsules::initialise() frequenceX = %i\n", frequenceX);
	ptbarreScr = score;
	ptPrntmney = money;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_money *capsu = sprites_list[i];
		capsu->littleInit();
	}
}

//-----------------------------------------------------------------------------
// bricks levels: send a capsule of money from brick
//-----------------------------------------------------------------------------
void zeCapsules::envoieFric(brickClear *briPT)
{
	zeCompteur++;
	if(zeCompteur > frequenceX)
	{	zeCompteur = 0;
		for(Sint32 i = 0; i < max_of_sprites; i++)
		{	sprite_money *capsu = sprites_list[i];
			if(capsu->disponible(briPT))
				return;
		}
	}
}

//-----------------------------------------------------------------------------
// bricks levels: send a capsule of money from BouiBoui
//-----------------------------------------------------------------------------
void zeCapsules::send_money(sprite_ball *pball)
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_money *capsu = sprites_list[i];
		if(capsu->disponible(pball))
			return;
	}
}

//-----------------------------------------------------------------------------
// bricks levels: send a capsule of money from BouiBoui
//-----------------------------------------------------------------------------
void zeCapsules::send_money(sprite_projectile *pfire)
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_money *capsu = sprites_list[i];
		if(capsu->disponible(pfire))
			return;
	}
}

//-----------------------------------------------------------------------------
// bricks levels: move capsule of money and collision with bumpers
//-----------------------------------------------------------------------------
void zeCapsules::bouge_fric()
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_money *capsu = sprites_list[i];
		capsu->play_animation_loop();
		Sint32 j = capsu->deplaceMoi();
		if(j)
		{	joueurGere->add_scores(20);
			ptPrntmney->creditPlus(j);
			//printf("zeCapsules::bouge_fric() : %i\n", j);
		}
	}
}

//-----------------------------------------------------------------------------
// guards levels: initialize the capsules before one level
//-----------------------------------------------------------------------------
void zeCapsules::initialise(Sint32 frequ, printmoney *money)
{
	frequenceX = frequ;
	ptPrntmney = money;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_money *capsu = sprites_list[i];
		capsu->littleInit();
	}
}

//-----------------------------------------------------------------------------
// guards levels: send a capsule of money from guard
//-----------------------------------------------------------------------------
void zeCapsules::envoieFric(sprite_ball *pball)
{
	zeCompteur++;
	if(zeCompteur > frequenceX)
	{	zeCompteur = 0;
		for(Sint32 i = 0; i < max_of_sprites; i++)
		{	sprite_money *capsu = sprites_list[i];
 			if(capsu->disponible(pball))
				return;
		}
	}
}

//-----------------------------------------------------------------------------
// guards levels: move capsule of money and collision with bumper
//-----------------------------------------------------------------------------
void zeCapsules::bougefric2()
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_money *capsu = sprites_list[i];
		capsu->play_animation_loop();
		Sint32 j = capsu->deplaceMe2();
		if(j)
		{	//printf("zeCapsules::bouge_fric2() : %i\n", j);
			ptPrntmney->creditPlus(j);
			joueurGere->add_scores(20);
		}
	}
}

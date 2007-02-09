//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeGemstone.cc"
// created		: 2004-04-12
// updates		: 2004-04-13
// fonction	: Manage the gem (bricks levels)
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

#include "../include/zeGemstone.h"

//-----------------------------------------------------------------------------
// Create the object
//-----------------------------------------------------------------------------
zeGemstone::zeGemstone()
{
	littleInit();
	max_of_sprites = NUMBER_GEM;
	sprites_have_shades = true;
	sprite_type_id = BOB_GEMSTO;
}

//-----------------------------------------------------------------------------
// Release the object
//-----------------------------------------------------------------------------
zeGemstone::~zeGemstone()
{
  release_sprites_list();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sint32	zeGemstone::initialise(handler_players* pjda, barreScore *score, printmoney *money,
	controller_paddles *pbump)
{
	pJoueurDat = pjda;
	ptBarScore = score;
	ptPrintmon = money;
	ptRaquette = pbump;
	for(Uint32 i = 0; i < max_of_sprites; i++)
	{	sprite_gem *ptGem = sprites_list[i];
		if (pjda->gem_is_set(i))
			ptGem->gemcollect(i);
	}
	return E_NO_ERROR;
}

//-----------------------------------------------------------------------------
// bricks levels:  send a gem
//-----------------------------------------------------------------------------
void zeGemstone::sendNewGem(sprite_ball *pball)
{
	for(Uint32 i = 0; i < max_of_sprites; i++)
	{	sprite_gem *ptGem = sprites_list[i];
		if(ptGem->disponible(pball))
			return;
	}
}

//-----------------------------------------------------------------------------
// bricks levels: send a gem
//-----------------------------------------------------------------------------
void zeGemstone::sendNewGem(sprite_projectile *pfire)
{
	for(Uint32 i = 0; i < max_of_sprites; i++)
	{	sprite_gem *ptGem = sprites_list[i];
		if(ptGem->disponible(pfire))
			return;
	}
}


//-----------------------------------------------------------------------------
// bricks levels: move gems and collision with bumpers
//-----------------------------------------------------------------------------
void zeGemstone::moving_gem()
{
	for(Uint32 i = 0; i < max_of_sprites; i++)
	{	sprite_gem *ptGem = sprites_list[i];
		Sint32 ztype = ptGem->move();
		if(ztype >= 0)
		{	if (!pJoueurDat->gem_is_set(ztype))
			{	ptBarScore->scoreAjout(500);	// one gem collected
				ptGem->gemcollect(ztype);
				if (pJoueurDat->gem_enable(ztype))
				{	//###################################################
					// 6 gems collected
					//###################################################
					for(Uint32 k = 0; k < max_of_sprites; k++)
					{	sprite_gem *zegem = sprites_list[k];
						zegem->activBlink();
					}
					ptBarScore->scoreAjout(2500);
					ptPrintmon->creditPlus(500);
					ptBarScore->add_life(3);
          /* jump to the next level */
					ptBarScore->scoreBrick(0);
          /* enable right paddle */
					pJoueurDat->set_bumpOn(controller_paddles::RIGHT_PADDLE, 3);
          /* enable top paddle */
					pJoueurDat->set_bumpOn(controller_paddles::TOP_PADDLE, 3);
          /* enable left paddle */
					pJoueurDat->set_bumpOn(controller_paddles::LEFT_PADDLE, 3);
					sprite_paddle* pBump;
					pBump = ptRaquette->get_paddle(controller_paddles::RIGHT_PADDLE);
					pBump->enable();
					pBump = ptRaquette->get_paddle(controller_paddles::TOP_PADDLE);
					pBump->enable();
					pBump = ptRaquette->get_paddle(controller_paddles::LEFT_PADDLE);
					pBump->enable();
					return;
				}
			}
		}
	}
}

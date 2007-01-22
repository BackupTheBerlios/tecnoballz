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
	objects_have_shades = true;
	BOBtypeNum = BOB_GEMSTO;
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
Sint32	zeGemstone::initialise(joueurData* pjda, barreScore *score, printmoney *money,
	zeRaquette *pbump)
{
	pJoueurDat = pjda;
	ptBarScore = score;
	ptPrintmon = money;
	ptRaquette = pbump;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	techno_gem *ptGem = sprites_list[i];
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
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	techno_gem *ptGem = sprites_list[i];
		if(ptGem->disponible(pball))
			return;
	}
}

//-----------------------------------------------------------------------------
// bricks levels: send a gem
//-----------------------------------------------------------------------------
void zeGemstone::sendNewGem(tecno_fire *pfire)
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	techno_gem *ptGem = sprites_list[i];
		if(ptGem->disponible(pfire))
			return;
	}
}


//-----------------------------------------------------------------------------
// bricks levels: move gems and collision with bumpers
//-----------------------------------------------------------------------------
void zeGemstone::moving_gem()
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	techno_gem *ptGem = sprites_list[i];
		Sint32 ztype = ptGem->deplaceMoi();
		if(ztype >= 0)
		{	if (!pJoueurDat->gem_is_set(ztype))
			{	ptBarScore->scoreAjout(500);	// one gem collected
				ptGem->gemcollect(ztype);
				if (pJoueurDat->gem_enable(ztype))
				{	//###################################################
					// 6 gems collected
					//###################################################
					for(Sint32 k = 0; k < max_of_sprites; k++)
					{	techno_gem *zegem = sprites_list[k];
						zegem->activBlink();
					}
					ptBarScore->scoreAjout(2500);
					ptPrintmon->creditPlus(500);
					ptBarScore->lifes_plus(3);
					ptBarScore->scoreBrick(0);		// next level
					pJoueurDat->set_bumpOn(2, 3);	// active right bumper
					pJoueurDat->set_bumpOn(3, 3);	// active top bumper
					pJoueurDat->set_bumpOn(4, 3);	// active left bumper
					tecno_bump* pBump;
					pBump = ptRaquette->demandeRak(2);
					pBump->enable();
					pBump = ptRaquette->demandeRak(3);
					pBump->enable();
					pBump = ptRaquette->demandeRak(4);
					pBump->enable();
					return;
				}
			}
		}
	}
}

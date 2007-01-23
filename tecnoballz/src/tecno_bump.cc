//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "tecno_bump.cc"
// created      : ?
// updates      : 2004-10-17
// fonction     : bumper sprite
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
//******************************************************************************
#include "../include/tecno_bump.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
tecno_bump::tecno_bump(zeFireBump * fBump)
{
	clear_sprite_members();
	raketLarge = 32 * resolution;
	bumperType = 0;
	bumperNorm = 2;
	bumperFire = 0;
	bumperGlue = 0;
	bump_speed = 0;
	bump_actif = 0;
	balleTouch = 0;
	bumpNumero = 0;
	balleColle = (sprite_ball *)NULL;
	bumperTirs = fBump;
	invincible = 0;
	flickerval = 0;
	width_mini = 0;
	width_maxi = 0;
	width_deca = 0;
	if(resolution == 1)
		width_deca = 3;
	else
		width_deca = 4;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
tecno_bump::~tecno_bump()
{
	if(bumperTirs)					//bumper has fires ? (robot has not fires)
		bumperTirs->release_sprites_list();	//release fires
}

//------------------------------------------------------------------------------
// initialize bumper's fires
//------------------------------------------------------------------------------
Sint32 tecno_bump::init_fires()
{
	Sint32 error = bumperTirs->installBOB(this);
	return (error);
}

//------------------------------------------------------------------------------
// start new bumper's fires
//------------------------------------------------------------------------------
void tecno_bump::lache_tirs()
{
	if(bumperFire)
		bumperTirs->disponible();	//zeFireBump::disponible();
}

//------------------------------------------------------------------------------
// move bumper's fires
//------------------------------------------------------------------------------
void tecno_bump::deplaceTir()
{
	bumperTirs->nouveauTir();	//zeFireBump::nouveauTir();
	bumperTirs->deplaceTir();	//zeFireBump::deplaceTir();
}

//------------------------------------------------------------------------------
// determine if the bumper must to be activated
// input => rTeam: 1 = mode team
//       => large: bumper's width (8,16,24,32,40,48,56 or 64)
//       => actif: if >0 bumper's active
//------------------------------------------------------------------------------
void tecno_bump::bumpActive(Sint32 rTeam, Sint32 large, Sint32 actif)
{
	raketLarge = large;
	is_enabled = 0;
	bump_actif = actif;
	if(bump_actif > 0)
	{	bump_actif--;
		is_enabled = 1;
	}
	if(bumpNumero == 1 || (bumpNumero == 4 && rTeam == 1))
	{	bump_actif = 1;
		is_enabled = 1;
	}
	bumper_gfx(large);
}

//------------------------------------------------------------------------------
// bricks levels: select the sprite image of a horizontal bumper 
// input	=> large: bumper's width 
//------------------------------------------------------------------------------
void tecno_bump::bumperHgfx(Sint32 large)
{
	raketLarge = large;
	collision_width = large;
	Sint32 i = (large >> width_deca) - 2;
	if(bumperFire)
		i += 7;
	if(bumperGlue)
		i += 14;
	change_GFX(i);
	bumperTirs->disable_sprites();
}

//------------------------------------------------------------------------------
// bricks levels: select the sprite image of a vertical bumper 
// input	=> large: bumper's width 
//------------------------------------------------------------------------------
void tecno_bump::bumperVgfx(Sint32 large)
{
	raketLarge = large;
	collision_height = large;
	Sint32 i = (large >> width_deca) - 2;
	if(bumperFire)
		i += 7;
	if(bumperGlue)
		i += 14;
	change_GFX(i);
	bumperTirs->disable_sprites();
}

//------------------------------------------------------------------------------
// select the sprite image of a bumper
// input	=> large: bumper's width 
//------------------------------------------------------------------------------
void tecno_bump::bumper_gfx(Sint32 large)
{
	Sint32 i = (large >> width_deca) - 2;
	if(bumperFire)
		i += 7;
	if(bumperGlue)
		i += 14;
	change_GFX(i);
}

//------------------------------------------------------------------------------
// select the sprite image of a bumper
//------------------------------------------------------------------------------
void tecno_bump::bumper_gfx()
{
	Sint32 i = (raketLarge >> width_deca) - 2;
	if(bumperFire)
		i += 7;
	if(bumperGlue)
		i += 14;
	change_GFX(i);
}

//------------------------------------------------------------------------------
// return the number of the bumper
// output	<= bumpNumero: 1 (bottom), 2 (right), 3 (top), 4 (left) or 5 (robot)
//------------------------------------------------------------------------------
Sint32 tecno_bump::bumper_num()
{
	return bumpNumero;
}

//------------------------------------------------------------------------------
// bricks levels: transform into bumper glue 
//------------------------------------------------------------------------------
void tecno_bump::bumpGoGlue()
{
	bumperGlue = 1;
	bumper_gfx();
}

//------------------------------------------------------------------------------
// bricks levels: transform into bumper fire 1 
//------------------------------------------------------------------------------
void tecno_bump::bump_fire1()
{
	bumperFire = 1;
	bumper_gfx();
	bumperTirs->fire1RunOn();
}

//------------------------------------------------------------------------------
// bricks levels: transform into bumper fire 2
//------------------------------------------------------------------------------
void tecno_bump::bump_fire2()
{
	bumperFire = 1;
	bumper_gfx();
	bumperTirs->fire2RunOn();
}

//------------------------------------------------------------------------------
// bricks levels: release the ball (if a ball's glued)
//------------------------------------------------------------------------------
void tecno_bump::lacheBalle()
{
	if(bumperGlue > 1)				//is the ball glued on bumper?
		bumperGlue = 1;				//bumper's free 
	sprite_ball *balle = balleColle;
	if(balle)
	{	balleColle = (sprite_ball *)NULL;
		balle->glueLibere();
	}
}

//------------------------------------------------------------------------------
// bricks levels: glue a ball to the bumper
//------------------------------------------------------------------------------
void tecno_bump::attachBall(sprite_ball * xBall)
{
	sprite_ball *balle = balleColle;
	if(balle)
		balle->glueLibere();
	balleColle = xBall;
	if(bumperGlue)
		bumperGlue = 2;
}


//------------------------------------------------------------------------------
// return bumper's width
//------------------------------------------------------------------------------
Sint32  tecno_bump::rakLargeur()
{ 
	return raketLarge;
}

//------------------------------------------------------------------------------
// guards levels: return invincible value
//------------------------------------------------------------------------------
Sint32 tecno_bump::getInvncbl()
{
	return invincible;
}

//------------------------------------------------------------------------------
// guards levels: initializer invincible value
//------------------------------------------------------------------------------
void tecno_bump::setInvncbl(Sint32 invis)
{
  invincible = invis;
}

//------------------------------------------------------------------------------
// guards levels: handle invincible bumper
//------------------------------------------------------------------------------
void tecno_bump::flickerRun()
{
	if(invincible > 0)
	{	invincible--;
		if(flickerval > 0)
		{	is_enabled = 0;
			flickerval = 0;
		}
		else
		{	is_enabled = 1;
			flickerval = 1;
		}
	}
	else
		is_enabled = 1;
}

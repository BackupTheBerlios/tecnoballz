//*****************************************************************************
// copyright (c) 1991-2003 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "tecno_bump.h"
// created      : ?
// updates      : 2004-08-31
// fonction     : bumper sprite
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
//******************************************************************************
#ifndef __TECNO_BUMP__
#define __TECNO_BUMP__
//...............................................................................
class tecno_bump;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/technoBall.h"
#include "../include/zeFireBump.h"
#include "../include/GIF_bitMap.h"
#include "../include/tecno_miss.h"
#include "../include/zeGigaBlit.h"
//...............................................................................
class tecno_bump:public sprite_object
{
	friend class zeRaquette;
	friend class zeNewBalls;
	friend class zeFireBump;
	friend class tecno_miss;
	friend class ballDirect;
	friend class zeGigaBlit;

	private:
		Sint32				bump_actif;	// flag : raquette active
		Sint32				bumperType;	// flag : type 0=horizontal / 1=vertical                
		Sint32				bumpNumero;	// numero de la raquette 1 a 4
		Sint32				raketLarge;	//bumper's width (in pixels)
		//Sint32				width_offs;	//8 (in 320) or 16 (in 640) 
		Sint32				width_mini;	//bumper mini width (32 or 64)
		Sint32				width_maxi;	//bumper maxi width (64 or 128)
		Sint32				width_deca; //3 or 4; offset between 2 size (2^3)
	
		Sint32				bumperNorm;	// flag : raquette normale
		Sint32				bumperFire;	// flag : raquette tireuse
		Sint32				bumperGlue;	// flag : 1=raquette colle libre

		const Sint32**		rebonds_Ga;	//ball rebounds table (move on the left)
		const Sint32**		rebonds_Dr;	//ball rebounds table (move on the right)
		const Sint32*		rebonds_GD;	//ball rebounds table (left and right)

		Sint32*				direct_tab;	// table direction si la balle collee

		technoBall*			balleColle;	// ptr  : l'objet balle collee
		Sint32				balleTouch;	// flag : balle touche raquette
		Sint32				invincible;	// > 0 bumper is invincible (guards phase)
		Sint32				flickerval;	// flag flicker
	
		// weapons's bumper
		zeFireBump*			bumperTirs;	// ptr  : objet tir du Bumper
		Sint32				bumper_FX0;	// tir inc0. X
		Sint32				bumper_FY0;	// tir inc0. Y
		Sint32				bumper_FX1;	// tir inc1. X
		Sint32				bumper_FY1;	// tir inc1. Y
		Sint32				bump_TFIRE;	// tir 0, 1, 2
		Sint32				bumper_FX2;	// tir inc2. X
		Sint32				bumper_FY2;	// tir inc2. Y
		Sint32				bump_Xscie;	// tir X centre scie
		Sint32				bump_Yscie;	// tir Y centre scie
		Sint32				bump_xdeca;	// decalage tir 6
		Sint32				bump_ydeca;	// decalage tir 6
		Sint32				bump_speed;	// vitesse deplacement (maximum 10)

	public:
							tecno_bump(zeFireBump * fBump);
							~tecno_bump();
		Sint32				init_fires();
		void				lache_tirs();
		void				deplaceTir();
		void				bumpActive(Sint32 rTeam, Sint32 large, Sint32 actif);
		void				bumperHgfx(Sint32 large);
		void				bumperVgfx(Sint32 large);
		void				bumper_gfx(Sint32 large);
		void				bumper_gfx();
		Sint32				bumper_num();
		void				bumpGoGlue();
		void				bump_fire1();
		void				bump_fire2();
		void				lacheBalle();
		void				attachBall(technoBall * xBall);
		void				directBall();
		Sint32				rakLargeur();
		Sint32				getInvncbl();
		void				setInvncbl(Sint32 invis);
		void				flickerRun();
};
#endif

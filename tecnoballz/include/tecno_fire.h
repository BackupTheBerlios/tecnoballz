//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_fire.h"
// created		: ?
// updates		: 2004-04-12
// fonction	: bumper's fire (bricks levels only)
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
#ifndef __TECNO_FIRE__
#define __TECNO_FIRE__
//...............................................................................
class tecno_fire;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/tecno_bump.h"
#include "../include/lesBriques.h"
#include "../include/zeBouiBoui.h"
#include "../include/tecno_boui.h"
#include "../include/barreScore.h"
//...............................................................................
class tecno_fire:public sprite_object
{
	friend class zeFireBump;
	friend class tecno_caps;	  
	friend class tecno_gads;
	friend class techno_gem;

	private:
		static const Sint32	maxi_fires = 200;
		static Sint32		total_fire;
		static tecno_fire*	list_fires[maxi_fires];
		static lesBriques*	brickObjet;
		static zeBouiBoui*	atomsObjet;
		static barreScore*	scoreObjet;
		Sint32				indexSinus;
		Sint32				fire_Xscie;
		Sint32				fire_Yscie;                          
		tecno_bump*			raquettePT;
		Sint32				fire_power;		// 1 = fire destroys the indestructible-destructibles bricks 
		Sint32				firePowerX;		// fire power 1 or 2

	public:
						tecno_fire();
						~tecno_fire();
		void			littleInit(tecno_bump * raket);
		void			firePower1();
		void			firePower2();
		static void		start_list (lesBriques * brick, zeBouiBoui * atoms,
					barreScore * score);
		static void		gestionTir();
		static void		hors_ecran();
		static void		anim_fires();
		static void		collision1();
		static void		collision2();
		static void		xDesactive();

};
#endif

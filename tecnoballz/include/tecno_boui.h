//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_boui.h"
// created		: ?
// updates		: 2004-08-29
// function	: handle BouiBoui (only bricks levels)
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
//*****************************************************************************
#ifndef __TECNO_BOUI__
#define __TECNO_BOUI__
//.......................................................................
class tecno_boui ;
//.......................................................................
#include "../include/sprite_object.h"
#include "../include/handler_audio.h"
#include "../include/zeCapsules.h"
#include "../include/ze_gadgets.h"
#include "../include/zeGemstone.h"
#include "../include/sprite_projectile.h"
#include "../include/sprite_ball.h"
#include "../include/lesBriques.h"
//.......................................................................
enum 
{	CODE_GEMME,
	CODE_MALUS,
	CODE_MONEY
};
//.......................................................................
class tecno_boui : public sprite_object
{	friend class zeBouiBoui;
	friend class sprite_projectile;
	friend class controller_balls;
	
	private:
		static const	Sint32	ATOM_XMINI = 32;
		static const	Sint32	ATOM_XMAXI = 191;
		static const	Sint32	ATOM_YMINI = 40; 
		static const	Sint32	ATOM_YMAXI = 191;
		static const	Sint32	ATOM_ANIMA = 8;
	
	private:
		zeCapsules*		ptCapsules;
		ze_gadgets*		pt_gadgets;
		zeGemstone*		ptGemstone;
		lesBriques*		pt_briques;
		Sint32			atom_power;		// strength
		Sint32			init_power;		// strength
		Sint32			atom_deplX;		// offset X
		Sint32			atom_deplY;		// offset Y
		Sint32			atom_count;		// trajectory change counter
		Sint32			atom_ghost;		// pointer on "tableGhost"
		Sint32*			tableGhost;		// table time keep same trajectory
		Sint32			atom_actif;		// counter activation (0 = active) 
		Sint32			atom_explo;		// 1 = current explosion
		Sint32			atom_oexpl;		// number of the first image's explosion
		Sint32			atom_traje;		// pointeur sur la trajectoir 
		Sint32			apparition;		// time before reactivation
		Sint32			atom_xmini;
		Sint32			atom_xmaxi;
		Sint32			atom_ymini;
		Sint32			atom_ymaxi;
		Sint32			collisionT[8];

		static Sint32	ghostWait1[16];
		static Sint32	ghostWait2[16];
		static Sint32	ghostWait3[16]; 
		static Sint32	ghostWait4[16];
		static Sint32*	ghost_wait[4];
		static Sint32	ghost_traj[32];
		static Sint32	ghost_bobs[32];	// index animations images of sprites
		static Sint32	codeBounty[16];

		public :
						tecno_boui();
						~tecno_boui();
		Sint32			over_brick(Sint32 pos_x, Sint32 pos_y);
		void			gere_atome();
		void			littleInit(Sint32 time0, Sint32 appar, Sint32 index, 
							Sint32 power, Sint32 pos_x, Sint32 pos_y, Sint32 offst, 
							zeCapsules*, ze_gadgets*, zeGemstone *, lesBriques*) ;
		void			explosion1(sprite_projectile *);
		void			explosion1(sprite_ball *);
		void			explosion2() ;
};
#endif

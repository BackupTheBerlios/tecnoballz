//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeguardian.cc"
// created		: 2003-01-10
// updates		: 2004-06-26
// fonction	: handle the guards (move and collisions)
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
//
//******************************************************************************
#ifndef __ZEGUARDIAN__
#define __ZEGUARDIAN__
//...............................................................................
class zeguardian;
//...............................................................................
#include "../include/mentatCode.h"
//...............................................................................

//-------------------------------------------------------------------------------
// structure of guards parameters
//-------------------------------------------------------------------------------
typedef struct
{
	Sint32	para_power;			// resistance
	Sint32	para_xcent;			// centre X depart tir
	Sint32	para_waitf;			// frequence tir gigaBlitz
	Sint32	para_speed;			// vitesse de deplacement
	Sint32	para_colx1;			// fenetre de collision x-mini
	Sint32	para_coly1;			// fenetre de collision y-mini
	Sint32	para_colx2;			// fenetre de collision x-maxi
	Sint32	para_coly2;			// fenetre de collision y-maxi
	Sint32	para_ycent;			// centre Y départ tir
	Sint32	para_wait2;			// frequence tir
	Sint32	para_nsbob;			// numero du BOB
	Sint32	para_lissa;			// numero de la courbe
	Sint32	para_tfire[16];		// table des pointeurs sur la liste tirs
}
gardlevel;
//...............................................................................
#include "../include/tecno_gard.h"
#include "../include/list_sprites.h"
#include "../include/objet_gere.h"
#include "../include/zeMissiles.h"
#include "../include/zeGigaBlit.h"
#include "../include/zexplosion.h"
//...............................................................................
class zeguardian:public objet_gere < tecno_gard >
{
	friend class tecno_gard;

	private:
		unsigned char*		getLissaPt(Sint32 lissa);
 		void				displayNrj();
	public:
							zeguardian();
							~zeguardian();
		Sint32				init_liste(zeMissiles *pMiss, Sint32 grdPt, 
										zeGigaBlit *pBliz, zexplosion* pExpl);
		void				execution1();
		void 				killguards(Sint32 numGa = 0);
		Sint32				next_level();
		Sint32				run_scroll(Uint32, Sint32,
										technoBall*, tecno_miss *);
	private:
		Sint32				scrollTemp;		//use for scroll background
		Sint32				offset_ptr;
		unsigned char*		lissaCurve;
		zeGigaBlit*			ptGigaBlit;
		zexplosion*			pexplosion;
		sprite_object**		pBobEnergy;

	public:
		static Sint32 		level2gdpt(Uint32 arean, Uint32 level);
	
	private:
		static gardlevel	guard_list[];
		static Sint32		level_list[];
		static const char	offzetbyte[256];
		static const Sint32	LISSA_NB01 = 0;
		static const Sint32	LISSA_NB02 = 1;
		static const Sint32	LISSA_NB03 = 2;
		static const Sint32	LISSA_NB04 = 3;
		static const Sint32	LISSA_NB05 = 4;
		static const Sint32	LISSA_NB07 = 5;
		static const Sint32	LISSA_NB08 = 6;
		static const Sint32	LISSA_NB09 = 7;
		static const Sint32	LISSA_NB11 = 8;
		static const Sint32	LISSA_NB12 = 9;
		static const Sint32	LISSA_NB13 = 10;
		static const Sint32	LISSA_NB15 = 11;
};
#endif

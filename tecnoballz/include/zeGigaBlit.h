//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "zeGigaBlit.h"
// created      : ?
// updates      : 2004-09-04
// fonctions    : manage the Gigablitz (bumper and guards)
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
//*****************************************************************************
#ifndef __ZEGIGABLIT__
#define __ZEGIGABLIT__
class zeGigaBlit;
//...............................................................................
#include "../include/giga_blitz.h"
#include "../include/liste_BOBs.h"
#include "../include/barreScore.h"
#include "../include/clavierMac.h"
#include "../include/ballDirect.h"
#include "../include/zeFireBump.h"
#include "../include/joueurData.h"
#include "../include/tableaux_Z.h"
#include "../include/objet_gere.h"
#include "../include/head_anima.h"
#include "../include/lesBriques.h"
//...............................................................................
class zeGigaBlit:public objet_gere < giga_blitz >
{	friend class            giga_blitz;
 
	private:
		static const Uint32	NOMBREGIGA = 7;
		static Sint32		numeroBOBs[NOMBREGIGA];  
    
		lesBriques*			brickObjet;
		head_anima*			gugusObjet;
		zeRaquette*			ptRaquette;
		zexplosion*			pexplosion;
		tecno_bump*			tecBumper1;
		tecno_bump*			tecBumper3;
		giga_blitz*			blitzobjet;	// pointer on the current "giga_blitz" 
		Sint32				blitz_colx;
		Sint32				blitz_posx;
		Sint32				blitz_posy;
		Sint32				blitz_haut;	// hauteur du gigablitz
		Sint32				bitz_ystop;	// ordonnee maximum du "gigablitz"
		Sint32				bitz_maxiy;	// 
		Sint32				bitz_miniy;	// 
		Sint32				blitz_seta;
		Sint32				blitz_brik;	//width of collison bricks in row
		Uint32				blitz_xsin;
  
	public:
							zeGigaBlit();
							~zeGigaBlit();
		Sint32				init_liste(zeRaquette *zeRak, head_anima *gugus, 
								lesBriques *brick);
		void				initDepart();
		void				execution1();
		void				collision1();
		Sint32				init_liste(zeRaquette *zeRak, zexplosion *pexpl);
		void				execution2();
		void 				collision2();
		Sint32				guard_shoot(Sint32 value, Sint32 pos_x, Sint32 pos_y,
											Sint32 large, Sint32 haute);
		Sint32				isactivate();
};
#endif

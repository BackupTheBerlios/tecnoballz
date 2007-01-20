//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeGesmstone.h"
// created		: 2004-04-12
// updates		: 2004-05-09
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
#ifndef __ZEGEMSTONE__
#define __ZEGEMSTONE__
//...............................................................................
class zeGemstone;
//...............................................................................
#include "../include/objet_gere.h"
#include "../include/zeRaquette.h"
#include "../include/techno_gem.h"
#include "../include/joueurData.h"
#include "../include/printmoney.h"
//...............................................................................

class zeGemstone:public objet_gere < techno_gem >
{
	public:
		static const Sint32	NUMBER_GEM = 6;
	
	private:
		zeRaquette*			ptRaquette;
		joueurData*			pJoueurDat;
		barreScore*			ptBarScore;
		printmoney*			ptPrintmon;

	public:
							zeGemstone();
							~zeGemstone();
		Sint32				initialise(joueurData*, barreScore*, 
								printmoney*, zeRaquette*);
		void				sendNewGem(sprite_ball *pball);
		void				sendNewGem(tecno_fire *pball);
		void				moving_gem();
};
#endif

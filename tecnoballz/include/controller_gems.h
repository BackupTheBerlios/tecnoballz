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
class controller_gems;
//...............................................................................
#include "../include/objects_list.h"
#include "../include/controller_paddles.h"
#include "../include/sprite_gem.h"
#include "../include/handler_players.h"
#include "../include/printmoney.h"
//...............................................................................

class controller_gems:public objects_list < sprite_gem >
{
	public:
		static const Sint32	NUMBER_GEM = 6;
	
	private:
		controller_paddles*			ptRaquette;
		handler_players*			pJoueurDat;
		barreScore*			ptBarScore;
		printmoney*			ptPrintmon;

	public:
							controller_gems();
							~controller_gems();
		Sint32				initialise(handler_players*, barreScore*, 
								printmoney*, controller_paddles*);
		void				sendNewGem(sprite_ball *pball);
		void				sendNewGem(sprite_projectile *pball);
		void				moving_gem();
};
#endif

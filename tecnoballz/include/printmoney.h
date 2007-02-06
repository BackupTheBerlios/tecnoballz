//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "printmoney.h"
// created		: 2002-11-28
// updates		: 2004-04-18
// fonction	: print credit of money and reverser malus
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
#ifndef __PRINTMONEY__
#define __PRINTMONEY__
class printmoney;
//...............................................................................
#include "../include/objects_list.h"
#include "../include/handler_players.h"
#include "../include/controller_paddles.h"
#include "../include/sprite_capsule.h"
//...............................................................................
class printmoney:public objects_list < sprite_object >
{
	private:
		static const Sint32	YINVERSEUR = 232;
		Sint32				y_position;
		handler_players*			ptJoueurDa;
		controller_paddles*			ptZraquett;
		Sint32				money_posy;
		sprite_object*			ptBobMoney;
		sprite_capsule*			ptBobRever;
		sprite_capsule*			ptBobLifes;
		//Sint32				flag_level;	// 0 = "bricks" / 1 = "guards"

	public:
							printmoney();
							~printmoney();
		void				initialise(handler_players*, controller_paddles*, sprite_object*, 
									sprite_capsule*);
		void				init_guard(handler_players*, controller_paddles*, sprite_object*, 
									sprite_capsule*);
		void				execution1(Sint32 value);
		void				execution2(Sint32, Sint32);
		void				creditPlus(Sint32 value);
	
	private:
		void				init_money();
		void				exec_money(Sint32);
};
#endif

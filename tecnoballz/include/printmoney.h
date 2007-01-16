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
#include "../include/objet_gere.h"
#include "../include/joueurData.h"
#include "../include/zeRaquette.h"
#include "../include/tecno_gads.h"
//...............................................................................
class printmoney:public objet_gere < sprite_object >
{
	private:
		static const Sint32	YINVERSEUR = 232;
		Sint32				y_position;
		joueurData*			ptJoueurDa;
		zeRaquette*			ptZraquett;
		Sint32				money_posy;
		sprite_object*			ptBobMoney;
		tecno_gads*			ptBobRever;
		tecno_gads*			ptBobLifes;
		//Sint32				flag_level;	// 0 = "bricks" / 1 = "guards"

	public:
							printmoney();
							~printmoney();
		void				initialise(joueurData*, zeRaquette*, sprite_object*, 
									tecno_gads*);
		void				init_guard(joueurData*, zeRaquette*, sprite_object*, 
									tecno_gads*);
		void				execution1(Sint32 value);
		void				execution2(Sint32, Sint32);
		void				creditPlus(Sint32 value);
	
	private:
		void				init_money();
		void				exec_money(Sint32);
};
#endif

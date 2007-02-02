//******************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "gard_techno.h"
// created	: ?
// updates	: 2005-01-11
// fonction	: handle the the guards levels
// id		: $Id: gard_tecno.h,v 1.7 2007/02/02 17:05:53 gurumeditation Exp $
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
#ifndef __GARD_TECNO__
#define __GARD_TECNO__
class gard_tecno;
#include "../include/lastScroll.h"
#include "../include/tecno_miss.h"
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/runGestion.h"
#include "../include/zeguardian.h"
#include "../include/zeMissiles.h"
#include "../include/zeRaquette.h"
#include "../include/controller_balls.h"
#include "../include/zeCapsules.h"
#include "../include/ze_gadgets.h"
#include "../include/printmoney.h"
#include "../include/zeMiniMess.h"
#include "../include/zeMiniMess.h"
#include "../include/zeGigaBlit.h"
#include "../include/zexplosion.h"
#include "../include/ballDirect.h"
#include "../include/zeGameOver.h"
#include "../include/zeCongBall.h"
#include "../include/handler_popup_menu.h"
#include "../include/zeMoveText.h"


class gard_tecno:public virtual runGestion
{
	friend class zeMoveText;

	private:
		zeGigaBlit*			ptGigaBlit;
		lastScroll*			defilement;
		zeguardian*			ptguardian;
		zeMissiles*			ptMissiles;
		zeRaquette*			ptRaquette;
		controller_balls*			ptNewBalls;
		ballDirect*			ptBaDirect;
		zeCapsules*			ptCapsules;
		ze_gadgets*			pt_gadgets;
		printmoney*			ptPrntmney;
		zeMiniMess*			ptMiniMess;
		zexplosion*			pExplosion;
		sprite_object*			ptBobMoney;
		sprite_object*			ptBob_name;
		tecno_gads*			ptBobLifes;
		handler_popup_menu*			ptrEscMenu;
		Sint32				areaNumber;		// number of area 1 to 5
		Sint32				levelTecno;		// number of level 1 to 12
		const atariLevel*		levelParam;
		Sint32				scrollTemp;		// time before scroll start
		Sint32				scrollType;		// type of background scrolling
		Sint32				scrolSpeed;		// speed of the scrolling
		zeGameOver*			ptGameOver;
		zeCongBall*			ptCongBall;
		Sint32				isgameover;
		Sint32				count_next;		//temporisation before next level 
		Sint32				tecnwinner;	 	//1 = end of the game
		zeMoveText*			ptMoveText;
	
	private:
		void				init_level();
		void				run_scroll();
		void				cheat_keys();
 
	public:
						gard_tecno();
						~gard_tecno();
		Sint32				first_init();
		Sint32				zeMainLoop();
};
#endif

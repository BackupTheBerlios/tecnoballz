//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ze_gadgets.h"
// created		: ?
// updates		: 2004-09-17
// fonction	: manage the gadgets (malus and bonus)
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
#ifndef __ZE_GADGETS__
#define __ZE_GADGETS__
class ze_gadgets; 
//...............................................................................
#include "../include/objects_list.h"
#include "../include/tecno_gads.h"
#include "../include/zeMiniMess.h"
#include "../include/zeRaquette.h"
#include "../include/controller_balls.h"
#include "../include/barreScore.h"
#include "../include/ze_magneye.h"

//...............................................................................
class ze_gadgets:public objects_list < tecno_gads >
{
	public:
		static const Sint32		SGADGET_X1 = 146;	//abscissa of the first bonus
		static const Sint32		SGADGET_Y1 = 2;		//ordinate of the first bonus 
		static const Sint32		SGADGET_X2 = 280;	//abscissa of the bonus indicator
		static const Sint32		SGADGET_Y2 = 204;	//ordinate of the bonus indicator

	private:
		sprite_paddle*			bumpSelect;		//bump select (cheat mode)
		zeMiniMess*			ptMiniMess;		//use for display minis messages
		zeRaquette*			ptRaquette;
		controller_balls*			ptNewBalls;
		barreScore*			ptbarreScr;
		sprite_object*			ptBob_wall;
		ze_magneye*			pt_magneye;
		Sint32				frame_delay;
		Sint32				frame_period;
		Sint32				frame_index;

		// shop
		tecno_gads*			temoin_gad;		// bonus indicator
	
		// maluses
		Sint32				malus_step;		// drop malus counter
		Sint32				malus_frek;		// drop frequency malus
		const Sint16*		malusTable;		// table of the maluses

		// bonuses
		Sint32				brick_kass;		// counter of bricks breaks
		Sint32				bonusAchet;		// number of bonuses buy
		Sint32				bonusTombe;		// number of bonuses droped
		Sint32				bonus_step;		// drop frequency bonuses
		Sint32				course_ptr;		// pointer to "courseList"
		Sint16				courseList[NB_OPTIONS + 1];
		static Sint16		keysTriche[];
		static const Uint16	randomlist[128];

	public:
							ze_gadgets(Sint32 total, Sint32 vShad = 1);
							~ze_gadgets();
		void				initialise(Sint32 mStep, Sint32 mKauf, Sint32 bStep, 
								const Sint16 *table, Sint32 *cours, 
								zeMiniMess* ptMes, zeRaquette *pRaqu, 
								controller_balls* pBall, barreScore* pScor, sprite_object*,
								ze_magneye* ptEye);
		void				envoieGads(brickClear *briPT);
		void				send_malus(sprite_ball *);
		void				send_malus(sprite_projectile *);	
		void				envoieGads(sprite_ball *pball);
		void create_shop_sprites_list();
		void				gadgetShop(Sint32 nuGad);
		void				gadgetShop(joueurData * gamer);
		void				bouge_gads();
		void				bougegads2();
		void				animations(Sint32 value = 1);
		void				gadgetKeys();

 
	private:
		void				gadget_run(sprite_paddle *raket, Sint32 nuGad);
		void				gadgetrun2(sprite_paddle *raket, Sint32 nuGad);

};
#endif

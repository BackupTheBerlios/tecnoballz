//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "tableaux_Z.h"
// created      : ?
// updates      : 2004-08-28
// fonctions    : manage the bricks levels
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
#ifndef __TABLEAUX_Z__
#define __TABLEAUX_Z__
class tableaux_Z;
//-------------------------------------------------------------------------------
#include "../include/handler_display.h"
#include "../include/handler_audio.h"
#include "../include/sprite_object.h"
#include "../include/bitmap_data.h"
#include "../include/list_sprites.h"
#include "../include/handler_memory.h"
#include "../include/objects_list.h"
#include "../include/runGestion.h"
#include "../include/zeBouiBoui.h"
#include "../include/sprite_ship.h"
#include "../include/handler_keyboard.h"
#include "../include/tiles_background.h"
#include "../include/zeRaquette.h"
#include "../include/briqueCote.h"
#include "../include/barreScore.h"
#include "../include/handler_display.h"
#include "../include/ejectBalls.h"
#include "../include/controller_bricks.h"
#include "../include/controller_balls.h"
#include "../include/zeCapsules.h"
#include "../include/ze_gadgets.h"
#include "../include/zeGigaBlit.h"
#include "../include/joueurData.h"
#include "../include/zeMoveText.h"
#include "../include/head_anima.h"
#include "../include/printmoney.h"
#include "../include/zeGameOver.h"
#include "../include/zeMiniMess.h"
#include "../include/level_data.h"
#include "../include/zeGemstone.h"
#include "../include/ballDirect.h"
#include "../include/escapeMenu.h"
#include "../include/ze_magneye.h"


//-------------------------------------------------------------------------------
class tableaux_Z:public virtual runGestion
{
	friend class            tiles_background;
	friend class            zeCapsules;
	friend class            controller_balls;
	friend class            zeRaquette;
	friend class            ze_gadgets;
	friend class            zeMoveText;
	friend class            zeBouiBoui;
	friend class            sprite_paddle;

	private:
		//static const Uint32	bkgd_order[50];	
	
		zeMiniMess*			ptMiniMess;
		printmoney*			ptPrntmney;
		briqueCote*			gereBricot;
		head_anima*			tete_gugus;
		controller_bricks*			briquesTab;
		tiles_background*			ecranfond4;
		ejectBalls*			gereEjects;
		controller_balls*			gereBalles;
		ballDirect*			ptBaDirect;
		zeRaquette*			theBumpers;
		barreScore*			tecZ_barre;
		zeBouiBoui*			les_atomes;
		ze_magneye*			pt_magneye;
		zeCapsules*			gereCapsul;
		ze_gadgets*			gereGadget;
		zeGemstone*			ptGemstone;
		zeMoveText*			gere_texte;
		zeGigaBlit*			ptGigaBlit;
		zeGameOver*			ptGameOver;
		sprite_object*			BottomWall;
		sprite_object*			ptBobMoney;
		escapeMenu*			ptrEscMenu;
		tecno_gads*			ptBobRever;
		Sint32				areaNumber;	// number of area 1 to 5
		Sint32				levelTecno;	// number of level 1 to 12
		const amigaLevel*	levelParam;
		Sint32				next_level;
		Sint32				count_next;	//temporisation before next level 
		Sint32				isgameover;
		
		Sint32				devel_keyx;	//next backgound
		Sint32				devel_keyw;	//previous backgound
		
		Sint32				indexbgrnd;
	public:
							tableaux_Z();
							~tableaux_Z();
		Sint32				first_init();
		Sint32				zeMainLoop();
	
	private:
		void				init_level();
		void				changebkgd();
		Sint32				background(Sint32 nbkdg = -1);
	
	
};
#endif

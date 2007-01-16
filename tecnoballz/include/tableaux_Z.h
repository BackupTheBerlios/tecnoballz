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
#include "../include/ecran_hard.h"
#include "../include/audiomixer.h"
#include "../include/sprite_object.h"
#include "../include/GIF_bitMap.h"
#include "../include/liste_BOBs.h"
#include "../include/handler_memory.h"
#include "../include/objet_gere.h"
#include "../include/runGestion.h"
#include "../include/zeBouiBoui.h"
#include "../include/tecno_boui.h"
#include "../include/clavierMac.h"
#include "../include/fond_ecran.h"
#include "../include/zeRaquette.h"
#include "../include/briqueCote.h"
#include "../include/barreScore.h"
#include "../include/ecran_hard.h"
#include "../include/ejectBalls.h"
#include "../include/lesBriques.h"
#include "../include/zeNewBalls.h"
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
	friend class            fond_ecran;
	friend class            zeCapsules;
	friend class            zeNewBalls;
	friend class            zeRaquette;
	friend class            ze_gadgets;
	friend class            zeMoveText;
	friend class            zeBouiBoui;
	friend class            tecno_bump;

	private:
		//static const Uint32	bkgd_order[50];	
	
		zeMiniMess*			ptMiniMess;
		printmoney*			ptPrntmney;
		briqueCote*			gereBricot;
		head_anima*			tete_gugus;
		lesBriques*			briquesTab;
		fond_ecran*			ecranfond4;
		ejectBalls*			gereEjects;
		zeNewBalls*			gereBalles;
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

//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file			: "tecno_gads.h"
// created			: ?
// updates			: 2004-06-13
// fonction		: objects malus and bonus gadgets
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
#ifndef __TECNO_GADS__
#define __TECNO_GADS__
class tecno_gads;
//..............................................................................
#include "../include/sprite_object.h"
#include "../include/sprite_paddle.h"
#include "../include/barreScore.h"
#include "../include/handler_display.h"
#include "../include/tecno_fire.h"
//..............................................................................
const Sint32	GAD_VIDE00 = 0;
const Sint32	GAD_GLUE00 = 2;		// glue (only briks levels)
const Sint32	GAD_NEXTLV = 4;		// next level (only briks levels)
const Sint32	GAD_PROTEC = 4;		// bumper protect (only gards levels)
const Sint32	GAD_FIRE01 = 6;		// fire power 1 (only briks levels)
const Sint32	GAD_FIRE02 = 8;		// fire power 2 (only briks levels)
const Sint32	GAD_SIZE_M = 10;	// retrecit la raquette
const Sint32	GAD_SIZE_P = 12;	// allonge la raquette
const Sint32	GAD_LIFE_M = 14;	// enleve une vie
const Sint32	GAD_LIFE_P = 16;	// ajoute une vie
const Sint32	GAD_BALLE2 = 18;	// ajoute 2 balles
const Sint32	GAD_BALLE3 = 20;	// ajoute 3 balles
const Sint32	GAD_POWER1 = 22;	// balle puissance 1
const Sint32	GAD_POWER2 = 24;	// balle puissance 2
const Sint32	GAD_INVERS = 26;	// inverseur de commandes
const Sint32	GAD_REBUIL = 28;	// REBUILD MUR (ONLY SHOP)
const Sint32	GAD_SPEEDM = 30;	// Vitesse MAXI (pas de Gadgets)
const Sint32	GAD_BUMP01 = 32;	// BUMP1 ACTIF (pas de Gadgets)
const Sint32	GAD_BUMP02 = 34;	// BUMP2 ACTIF (pas de Gadgets)
const Sint32	GAD_BUMP03 = 36;	// BUMP3 ACTIF (pas de Gadgets)
const Sint32	GAD_BUMP04 = 38;	// BUMP4 ACTIF (pas de Gadgets)
const Sint32	GAD_SIZE01 = 40;	// Balle taille 2
const Sint32	GAD_SIZE02 = 42;	// Balle taille 3
const Sint32	GAD_RANDOM = 44;	// Random
const Sint32	GAD_MEGA00 = 46;	// Toutes les options
const Sint32	GAD_PRICE1 = 48;	// Bonus Price=1 (pas de Gadgets)
const Sint32	GAD_WALL01 = 50;	// Mur du bas actif
const Sint32	GAD_ROBOT1 = 52;	// Robot actif
const Sint32	GAD_CONTRO = 54;	// Controle balles
const Sint32	GAD_OEIL00 = 56;	// Oeil
const Sint32	GAD_LESSBR = 58;	// Less-Brick (only Shop)
const Sint32	GAD_INFORM = 60;	// Informations (only Shop)
const Sint32	GAD_EXITSH = 62;	// Exit (only shop)
//..............................................................................
const Sint32	XXX_IMAGES = 7;
const Sint16	XXX_VIDE00 = -1;
const Sint16	XXX_SIZE_P = 0 * XXX_IMAGES;
const Sint16	XXX_SIZE_M = 1 * XXX_IMAGES;
const Sint16	XXX_LIFE_P = 2 * XXX_IMAGES;
const Sint16	XXX_LIFE_M = 3 * XXX_IMAGES;
const Sint16	XXX_POWER1 = 4 * XXX_IMAGES;
const Sint16	XXX_POWER2 = 5 * XXX_IMAGES;
const Sint16	XXX_BALLE2 = 6 * XXX_IMAGES;
const Sint16	XXX_BALLE3 = 7 * XXX_IMAGES;
const Sint16	XXX_FIRE01 = 8 * XXX_IMAGES;
const Sint16	XXX_GLUE00 = 9 * XXX_IMAGES;
const Sint16	XXX_INVERS = 10 * XXX_IMAGES;
const Sint16	XXX_FIRE02 = 11 * XXX_IMAGES;
const Sint16	XXX_SIZE01 = 12 * XXX_IMAGES;
const Sint16	XXX_SIZE02 = 13 * XXX_IMAGES;
const Sint16	XXX_RANDOM = 14 * XXX_IMAGES;
const Sint16	XXX_MEGA00 = 15 * XXX_IMAGES;
const Sint16	XXX_WALL01 = 16 * XXX_IMAGES;
const Sint16	XXX_ROBOT1 = 17 * XXX_IMAGES;
const Sint16	XXX_CONTRO = 18 * XXX_IMAGES;
const Sint16	XXX_PROTEC = 19 * XXX_IMAGES;
//..............................................................................
class tecno_gads:public sprite_object
{
	friend class ze_gadgets;
	friend class shop_tecno;

	private:
		Sint32				directionX;
		Sint32				gadget_num;
		sprite_paddle*			raquettePT;
		static const Sint16	gagdetBrik[];
		static const Sint16	gagdetGuar[];

	public:
							tecno_gads();
							~tecno_gads();
		void				littleInit();
		void				new_gadget(Sint32 nuGad);
		void				nouveauGad(brickClear *briPT, Sint32 nuGad);
		void				nouveauGad(sprite_ball *balle, Sint32 nuGad);
		void				nouveauGad(tecno_fire *pfire, Sint32 nuGad);
		void				new_gadget(sprite_ball *balle, Sint32 nuGad);
		void				nouveauGad(Sint32 nuGad);
		void				copiegadet(tecno_gads *gadgt);
		Sint32				get_gadget();
		sprite_paddle*			deplaceMoi();
		void				gagdet_run(Sint32 nuGad);
};
#endif

//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "supervisor_shop.h"
// created		: ?
// updates		: 2004-10-11
// fonction	: manage the shop
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
//******************************************************************************
#ifndef __SHOP_TECNO__
#define __SHOP_TECNO__
//...............................................................................
class supervisor_shop;
//...............................................................................
#include "../include/list_sprites.h"
#include "../include/sprite_mouse_pointer.h"
#include "../include/sprite_object.h"
#include "../include/controller_capsules.h"
#include "../include/print_text.h"
#include "../include/handler_players.h"
#include "../include/supervisor.h"
#include "../include/handler_popup_menu.h"
#include "../include/tiles_background.h"
//...............................................................................
class supervisor_shop:public supervisor
{
	private:
		static const Sint32	STEXTWIDHT = 22;	//largeur d'une ligne texte en caracteres
		tiles_background*			ecranfond4;
		sprite_mouse_pointer*			mouse_pointer;
		sprite_object*			BOB_allume;
		controller_capsules*			gereGadget;
		print_text*			mega_print;
		handler_popup_menu*			ptrEscMenu;
		Sint32				shop_point;	//pointeur
		Sint32				shoppoint3;	//pointeur
    
		Sint32				optioninfo; // 0;1;2;3
		Sint32				infodejavu;	//1 = info already seen at least once
		Sint32				prixActuel;	//current price
		Sint32				bonusachat;	//number of bought bonus
		char*				shop_line1;
		char*				shop_line2;
		char*				shop_line3;
		Sint32				curseur_x1;
		Sint32				curseur_y1;
		Sint32				get_object;	// drag object : pointer to the table "case_price" (-1 = no drag object)
		Sint32				pt_get_obj;
		sprite_capsule*			bobclignot;
		sprite_capsule*			bob_volant;
		Sint32*				courseList;
	
		// temporary list of the bonuses bought
		Sint32				coursetemp[NB_OPTIONS + 1];
    
		Sint32				shop_xmini;
		Sint32				shop_xmaxi;
		Sint32				shop_xmax2;
		Sint32				shop_ymax1;
		Sint32				shop_ymax2;
    
		Sint32				cadre_offs;
		Sint32				cadre_flag;	// 1 = display box
		Sint32				cadre_ymin;
		Sint32				cadre_ymax;
		Sint32				cadre_posx;
		Sint32				cadre_posy;	//mouse y coordinate (collision box)
		Sint32				cadre_haut;
		Sint32				cadre_larg;
		Sint32				angleValue;
		Sint32				box_colour;
		
		Sint32				triche_key;
		Sint32				triche_etb;
		Sint32				tricheCode;
    
		// table de pointeurs sur "case_price"
		static Sint32		sh_tablept[NB_OPTIONS];
		
		//static Sint32		curseurPos[];
		static Sint32		case_types[];
		static Sint32		case_price[];
		static char			shoptext00[];
		static char			shoptext41[];
		static char			shoptext63[];
		static char			shoptext56[];
		static char			shoptextPT[];
		static char			shoptext12[];
		static char			info_text1[];
		static char			info_text2[];
		static char			info_text3[];
    
	public:
								supervisor_shop();
								~supervisor_shop();
		Sint32				first_init();
		Sint32				main_loop();
		Sint32				testkursor(Sint32 x, Sint32 y);
		void				faitcourse(Sint32 gadnu);
		void				aff_course();
		Sint32				sub_credit(Sint32 value);
		void				achete_gad(Sint32 gadnb);
		void				message_ok();
		//Sint32			ledTableau();
		//Sint32			led_tablo2();
		Sint32				led_moving(Sint32 point);
		//Sint32			get_nbgadt(Sint32 x, Sint32 y);
  
	private:
		void				putthetext(char *ligne);
		void				affichtext();
		void				sh_ballade();
		Sint32				collisions();
		void				pos_select();
		void				aff_select();
		void				tu_triches();
	
		static const unsigned char cyclingtab[];
};
#endif

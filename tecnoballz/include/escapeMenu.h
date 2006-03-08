//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "escapeMenu.h"
// created		: 2004-08-08
// updates		: 2004-08-28
// fonction	: handle escape menu
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
#ifndef __ESCAPEMENU__
#define __ESCAPEMENU__
//...............................................................................
class escapeMenu;
//...............................................................................
#include "../include/mentatCode.h"
#include "../include/print_text.h"
#include "../include/BOB_killer.h"
//...............................................................................
class escapeMenu:public virtual print_text, public virtual BOB_killer
{
	private:
		static const Sint32 YCOORDNULL = -10240;
	
	private:
		GIF_bitMap*			graphTexte;
		Sint32				nb_columns;
		Sint32				nb_ofLines;
		Sint32				space2next;
		Sint32				yPressLeft;
		Sint32				yPressRigh;
		Sint32				menu_color;
		Sint32				restoreBkg;	//1: restore background when leave menu
		Sint32				menuNumber;

	public:
		static const Sint32	WECONTINUE = 1;
		static const Sint32	GOGAMEOVER = 2;
		static const Sint32	EXITTOMENU = 3;
		static const Sint32	WEQUITGAME = 4;
	
	public:
					escapeMenu();
					~escapeMenu();
	
		Sint32		first_init(GIF_bitMap *image, Sint32 nmenu, Sint32 large,
							Sint32 resto = 1, Sint32 palet = 0);
		Sint32		execution1();
	
	private:
		void 		displayBOB(char *ptSrc, Sint32 pos_x, Sint32 pos_y, Sint32 NxLine, Sint32 width, Sint32 heigh);
		void		display640();
		void		display320();
		static const unsigned char cyclingtab[];
		static const char *menu_texts[2];
		static const char menu_text1[];
		static const char menu_text2[];
	
};
#endif

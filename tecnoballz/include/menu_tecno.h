//******************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "menu_tecno.h"
// created      : ?
// updates      : 2005-01-18
// fonction     : management of the menu principal
// id		: $Id: menu_tecno.h,v 1.2 2005/01/18 13:21:39 gurumeditation Exp $
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
#ifndef __MENU_TECNO__
#define __MENU_TECNO__
#include "../include/lastScroll.h"
#include "../include/runGestion.h"
#include "../include/clavierMac.h"
#include "../include/BOB_killer.h"
#include "../include/liste_BOBs.h"
#include "../include/zeDefilTxt.h"
#include "../include/print_text.h"
#include "../include/GFX_bitmap.h"
#include "../include/print_menu.h"
#include "../include/mousePoint.h"
#include "../include/audiomixer.h"
class menu_tecno:public virtual runGestion
{
	private:
		lastScroll*		defilement;
		BOB_killer*		BOBtecLogo;
		zeDefilTxt*		BOB_defile;
		print_menu*		menu_texte;
		Sint32			offset_xx1;
		mousePoint*		objetMouse;
		//static unsigned char	colors_map[];
		static const char	codesarea[241];

	public:
					menu_tecno();
					~menu_tecno();
		Sint32			first_init();
		Sint32			zeMainLoop();
		static const char*	getTheCode(Uint32 arean, Uint32 hardc);
	
	private:
		void			offset_inc();
		void			moveZeLogo();
		Sint32			image_fond();
		Sint32			start_tecz();
};
#endif

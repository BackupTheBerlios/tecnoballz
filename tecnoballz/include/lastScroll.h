//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "lastScroll.h"
// created		: ?
// updates		: 2004-09-10
// fonction	: handle of the scrolling background (menu and gards levels)
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
#ifndef __LASTSCROLL__
#define __LASTSCROLL__
//...............................................................................
class lastScroll;
//...............................................................................
#include "../include/handler_display.h"
#include "../include/bitmap_data.h"
#include "../include/mentatCode.h"
#include "../include/scrolledit.h"
//...............................................................................
class lastScroll:public virtual mentatCode
{	friend class scrolledit;
	
	public:
		static const Uint32		COLOR_GARD = 0;
		static const Uint32		TECZ_COLOR_MENU = 1;
		static const Uint32		COLOR_CONG = 2;
		static const Uint32		MAPED_GARD = 0;
		static const Uint32		MAPED_MENU = 1;
		static const Uint32		MAPED_CONG = 2;
		static const Sint32		CARTELARGE = 20;	//number maps peer raw	
		static const Sint32		CARTEHAUTE = 273;	//number lines total of maps

	private:
	
		Sint32					motiflarge;
		Sint32					motifhaute;

		Sint32					destinMod1;
		Sint32					destinMod2;
		Sint32					destinMod3;
		Sint32					source_mod;
		Sint32*					afficheAdr;
		char**					mapAddress;	// adresse des dalles

		Uint16*					carteTable;	// table des codes des motifs
		Sint32					largeMotif;	//number maps peer line (into page)
		Sint32					hauteMotif;	//number total linesof maps (into page)
		Sint32					offset_aff;
		Sint32					largeEcran;
		Uint16*					carteFirst;	// table des codes des motifs
		Sint32					y_coord;
		bitmap_data*				gfx_bitMap;
		Sint32					dalleTotal;	//number total of maps
		char*					bitmap_adr;
		
		static const unsigned char	colors_map[48];
	
	public:
							lastScroll();
							~lastScroll();
		bitmap_data*			getBitMap();
		Sint32				initialise(Uint32 PalNu = 0, Uint32 edmap = 0);
		Sint32				tile_width();
		Sint32				returnPosy();
		void				palette_go(Uint32 PalNu = 0);
		Sint32				swapScroll(Uint32 PalNu, Uint32 edmap);
		void				scrolling1(Sint32 index = 0);
		void				afficheAll();
		void				display320();
		void				display640();

	private:
		Sint32				initMapAdr();
		Sint32				ld_mapfile(Uint32 edmap = 0);
};
#endif

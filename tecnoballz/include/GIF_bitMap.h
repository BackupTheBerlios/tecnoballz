//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "GIF_bitMap.h"
// created		: 1996-06-29
// updates		: 2004-05-13
// fonctions	: handle BMP images
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
#ifndef __GIF_BITMAP__
#define __GIF_BITMAP__
//...............................................................................
class GIF_bitMap ;
//...............................................................................
#include "../include/GFX_bitmap.h"
//...............................................................................
class GIF_bitMap : public virtual GFX_bitmap
{
	private :	
		unsigned char		GIFpalette[768];	//palette of 256 colours
												//(256*3=768 octets)
	public :
							GIF_bitMap();
							~GIF_bitMap();
		void				palette_go();
		void				paletteDup();	//copy first 128 colors in 128 last
		void				speciaFond();	//special 4 colors background
		unsigned char*		paletteADR();	//return palette address 
		
		Sint32				decompacte(char* nomfichier);
		Sint32				decompacte(Sint32);
		GIF_bitMap*			coupe_page(Sint32 pos_X, Sint32 pos_Y, Sint32 large, Sint32 haute);
		

	private :
		Sint32				SDLLoadBMP(char* nomfichier);
};
#endif

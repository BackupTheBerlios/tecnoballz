//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "GFX_bitmap.cc"
// created		: ?
// updates		: 2004-10-10
// function	: handle bitmap image
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
//*****************************************************************************
#ifndef __GFX_BITMAP__
#define __GFX_BITMAP__
//......................................................................
class GFX_bitmap ;
//......................................................................
#include "../include/mentatCode.h"
//......................................................................
class GFX_bitmap : public virtual mentatCode
{
	protected :
		SDL_Surface*		gfxSurface;
		char*				gfxAdresse;	//page memory address
		Sint32				gfxHauteur;	//height in lines
		Sint32				gfxLargeur;	//wight in pixels
		Sint32				gfx_nextLn;	//wight in bytes
		Sint32				gfxProfond;	//bytes peer pixel (always 1)
		Sint32				gfx_taille;	//page size in bytes

	public :
							GFX_bitmap() ;
							~GFX_bitmap() ;
		void				GFXInitial();
		void				GFXLiberat();
		Sint32				GFXlargeur();
		Sint32				GFX_nextLn();
		Sint32				GFXhauteur();
		char*				GFXadresse(Sint32 posX, Sint32 posY);
		char*				GFXadresse();
		Sint32				GFXrelatif(Sint32 posX, Sint32 posY);
		Sint32				GFX_modulo(Sint32 large);
		Sint32				GFXnouveau(Sint32 large, Sint32 haute, Sint32 depth);
		char*				duplicates();
		void				copyTampon();
		void				copyTampon(Sint32 srceX, Sint32 srceY, Sint32 destX,
										Sint32 destY, Sint32 large, Sint32 haute);
		void				copyBuffer(Sint32 srceX, Sint32 srceY, Sint32 destX,
										Sint32 destY, Sint32 large, Sint32 haute);
		void				buffer_clr(Sint32 pixel = 0);

} ;
#endif
//......................................................................

//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "GIF_bitMap.cc"
// created	: 1996-06-29
// updates	: 2005-07-07
// fonctions	: handle BMP images (SDL)
// id		: $Id: GIF_bitMap.cc,v 1.4 2007/01/17 20:05:07 gurumeditation Exp $
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
#include "../include/GIF_bitMap.h"
#include "../include/ressources.h"
#include "../include/handler_display.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
GIF_bitMap::GIF_bitMap()
{
	GFXInitial();
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
GIF_bitMap::~GIF_bitMap()
{
	GFXLiberat();
}

//-------------------------------------------------------------------------------
// set palette corresponding with image 
//-------------------------------------------------------------------------------
void GIF_bitMap::palette_go()
{
	display->palette_go(GIFpalette);
}

//-------------------------------------------------------------------------------
// recopy the first 128 colors of the palette in the 128 last
//-------------------------------------------------------------------------------
void GIF_bitMap::paletteDup()
{
	for(Sint32 i = 0; i < 16*3; i++)
		GIFpalette[i + (128*3)] = GIFpalette[i];
}

//-------------------------------------------------------------------------------
// return pointer to the palette
//-------------------------------------------------------------------------------
unsigned char *GIF_bitMap::paletteADR()
{
	return GIFpalette;
}

//-------------------------------------------------------------------------------
// recopy a piece of the page in a new page 
//-------------------------------------------------------------------------------
GIF_bitMap *GIF_bitMap::coupe_page(Sint32 pos_X, Sint32 pos_Y, Sint32 large, Sint32 haute)
{ 
	GIF_bitMap *image = new GIF_bitMap();
	erreur_num = image->GFXnouveau(large, haute, 1);
	if(!erreur_num)
	{	char *detPT = image->GFXadresse(0, 0);
		char *srcPT = GFXadresse(pos_X, pos_Y);
		Sint32 srcOf = gfxLargeur;
		for(Sint32 i = 0; i < haute; i++, srcPT += srcOf)
		{	char *monPT = srcPT;
			for(Sint32 j = 0; j < large; j++)
				*(detPT++) = *(monPT++);
		}
	}
	return image;
}

//-------------------------------------------------------------------------------
// load a bitmap file
//-------------------------------------------------------------------------------
Sint32 GIF_bitMap::decompacte(char* fname)
{
	char *fpath = pRessource->locateFile(fname);
	return SDLLoadBMP(fpath);
}

//-------------------------------------------------------------------------------
// load a bitmap file
//-------------------------------------------------------------------------------
Sint32 GIF_bitMap::decompacte(Sint32 ident)
{
	char *fpath = pRessource->locate_res(ident);
	return SDLLoadBMP(fpath);
}

//-------------------------------------------------------------------------------
// load a bitmap file
//-------------------------------------------------------------------------------
Sint32 GIF_bitMap::SDLLoadBMP(char* fpath)
{
	GFXLiberat();
	gfxSurface = SDL_LoadBMP(fpath);
	if(!gfxSurface)
	{	fprintf(stderr,
			"GIF_bitMap::decompacte(): Impossible to create a surface: %s\n",
			SDL_GetError());
		erreur_num = E_SDLERROR;
		return erreur_num;
	}
	gfxAdresse = (char*)gfxSurface->pixels;
	gfxLargeur = gfxSurface->w;
	gfx_nextLn = gfxLargeur;
	gfxHauteur = gfxSurface->h;
	gfx_taille = gfxHauteur * gfxLargeur;
	gfxProfond = 1;
	SDL_Color *couleurs = gfxSurface->format->palette->colors;
	Sint32 k=0;
	for (Sint32 j = 0 ; j<gfxSurface->format->palette->ncolors ; j++)
	{	GIFpalette[k++] = couleurs->r; 
		GIFpalette[k++] = couleurs->g;
		GIFpalette[k++] = couleurs->b;
		couleurs++;
	}
	erreur_num = E_NO_ERROR;
	return erreur_num;
}

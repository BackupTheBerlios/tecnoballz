/** 
 * @file handler_display.cc 
 * @briefi Handle the bitmap 
 * @created 1996-06-29 
 * @date 2007-01-24
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: bitmap_data.cc,v 1.1 2007/01/24 11:52:25 gurumeditation Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include "../include/bitmap_data.h"
#include "../include/ressources.h"
#include "../include/handler_display.h"

/** 
 * Create the object bitmap
 */
bitmap_data::bitmap_data()
{
	GFXInitial();
}

/**
 * Release the object bitmap
 */
bitmap_data::~bitmap_data()
{
	GFXLiberat();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
void bitmap_data::GFXInitial()
{
	mentatInit();
	gfxSurface = (SDL_Surface*)NULL;
	gfxAdresse = (char*)NULL;	// Adresse memoire du tampon
	gfxHauteur = 0;				// height in lines
	width = 0;				// width in pixels
	gfx_nextLn = 0;				// size of line in bytes
	gfxProfond = 0;				// number of bytes peer pixel
	gfx_taille = 0;				// size of bitmap in btyes
}

//-----------------------------------------------------------------------------
// release bitmap
//-----------------------------------------------------------------------------
void bitmap_data::GFXLiberat()
{
	if(gfxSurface)
	{	SDL_FreeSurface(gfxSurface);
		gfxSurface = (SDL_Surface*)NULL;
		gfxAdresse = (char*)NULL;
	}
	else
	if(gfxAdresse)
	{	memory->release(gfxAdresse);
		gfxAdresse = (char*)NULL;
	}
	mentatKill();
}

//-----------------------------------------------------------------------------
// return size of line in pixels
//-----------------------------------------------------------------------------
Sint32 bitmap_data::GFXlargeur()
{
	return width;
}

//-----------------------------------------------------------------------------
// return size of line in bytes
//-----------------------------------------------------------------------------
Sint32 bitmap_data::GFX_nextLn()
{
	return (gfx_nextLn);
}

//-----------------------------------------------------------------------------
// return bitmap height in lines
//-----------------------------------------------------------------------------
Sint32 bitmap_data::GFXhauteur()
{
	return gfxHauteur;
}

//-----------------------------------------------------------------------------
// return bitmap memory address from the corresponding coordinate
//-----------------------------------------------------------------------------
char *bitmap_data::GFXadresse(Sint32 posX, Sint32 posY)
{
	return (gfxAdresse + posY * gfx_nextLn + (posX * gfxProfond));
}

//-----------------------------------------------------------------------------
// return bitmap memory address
//-----------------------------------------------------------------------------
char *bitmap_data::GFXadresse()
{
	return gfxAdresse;
}

//-----------------------------------------------------------------------------
// return modulo 
//-----------------------------------------------------------------------------
Sint32 bitmap_data::GFX_modulo(Sint32 w)
{
	return (gfx_nextLn - (w * gfxProfond));
}

//-----------------------------------------------------------------------------
// return bitmap memory offset from the corresponding coordinate
//-----------------------------------------------------------------------------
Sint32 bitmap_data::GFXrelatif(Sint32 posX, Sint32 posY)
{
	return (posY * gfx_nextLn + posX * gfxProfond);
}

//-----------------------------------------------------------------------------
// allocate new bitmap
//-----------------------------------------------------------------------------
Sint32 bitmap_data::GFXnouveau(Sint32 w, Sint32 heigh, Sint32 depth)
{
	width = w;
	gfx_nextLn = (Sint32)(width * depth);
	gfxHauteur = heigh;
	gfx_taille = gfxHauteur * gfx_nextLn;
	gfxProfond = depth;
	gfxAdresse = memory->alloc(gfx_taille);
	error_init(memory->retour_err());
	return erreur_num;
}

//-------------------------------------------------------------------------------
// duplicate buffer and return pointer
//-------------------------------------------------------------------------------
char* bitmap_data::duplicates()
{
	char *ptGfx = memory->alloc(gfx_taille);
	for(Sint32 i = 0; i < gfx_taille; i++)
		ptGfx[i] = gfxAdresse[i];
	error_init(memory->retour_err());
	return ptGfx;
}

//-------------------------------------------------------------------------------
// copy the page into the "tampon" memory
//-------------------------------------------------------------------------------
void bitmap_data::copyTampon()
{
	copyTampon(0, 0, 0, 0, width, gfxHauteur);
}

//-------------------------------------------------------------------------------
// recopy a piece of the page into the "tampon" memory
//-------------------------------------------------------------------------------
void bitmap_data::copyTampon(Sint32 srceX, Sint32 srceY, Sint32 destX, Sint32 destY, 
								Sint32 large, Sint32 haute)
{
	Sint32 n = width;
	Sint32 o = display->tamponNext();
	char *s = GFXadresse(srceX, srceY);
	char *d = display->tampon_pos(destX, destY);
	Sint32 h = haute;
	for(Sint32 i = 0; i < h; i++, s += n, d += o)
	{	for(Sint32 j = 0; j < large; j++)
			d[j] = s[j];
	}
}

//-------------------------------------------------------------------------------
// recopy a piece of the page into the "buffer" memory
//-------------------------------------------------------------------------------
void bitmap_data::copyBuffer(Sint32 srceX, Sint32 srceY, Sint32 destX,
								Sint32 destY, Sint32 large, Sint32 haute)
{
	
	if(large == -1) large = width;
	if(haute == -1) haute = gfxHauteur;
	Sint32 n = width;
	Sint32 o = display->bufferNext();
	char *s = GFXadresse(srceX, srceY);
	char *d = display->buffer_pos(destX, destY);
	Sint32 h = haute;
	for(Sint32 i = 0; i < h; i++, s += n, d += o)
	{	for(Sint32 j = 0; j < large; j++)
			d[j] = s[j];
	}
}

//-------------------------------------------------------------------------------
// clear image buffer memory
//-------------------------------------------------------------------------------
void bitmap_data::buffer_clr(Sint32 pixel)
{
	Sint32 large = width;
	Sint32 h = gfxHauteur;
	Sint32 p = pixel;
	char *d = gfxAdresse;
	Sint32 n = width;
	for(Sint32 i = 0; i < h; i++, d += n)
	{	for(Sint32 j = 0; j < large; j++)
			d[j] = p;
	}
}



















//-------------------------------------------------------------------------------
// set palette corresponding with image 
//-------------------------------------------------------------------------------
void bitmap_data::palette_go()
{
	display->palette_go(GIFpalette);
}

//-------------------------------------------------------------------------------
// recopy the first 128 colors of the palette in the 128 last
//-------------------------------------------------------------------------------
void bitmap_data::paletteDup()
{
	for(Sint32 i = 0; i < 16*3; i++)
		GIFpalette[i + (128*3)] = GIFpalette[i];
}

//-------------------------------------------------------------------------------
// return pointer to the palette
//-------------------------------------------------------------------------------
unsigned char *bitmap_data::paletteADR()
{
	return GIFpalette;
}

//-------------------------------------------------------------------------------
// recopy a piece of the page in a new page 
//-------------------------------------------------------------------------------
bitmap_data *bitmap_data::coupe_page(Sint32 pos_X, Sint32 pos_Y, Sint32 large, Sint32 haute)
{ 
	bitmap_data *image = new bitmap_data();
	erreur_num = image->GFXnouveau(large, haute, 1);
	if(!erreur_num)
	{	char *detPT = image->GFXadresse(0, 0);
		char *srcPT = GFXadresse(pos_X, pos_Y);
		Sint32 srcOf = width;
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
Sint32 bitmap_data::decompacte(char* fname)
{
	char *fpath = pRessource->locateFile(fname);
	return SDLLoadBMP(fpath);
}

//-------------------------------------------------------------------------------
// load a bitmap file
//-------------------------------------------------------------------------------
Sint32 bitmap_data::decompacte(Sint32 ident)
{
	char *fpath = pRessource->locate_res(ident);
	return SDLLoadBMP(fpath);
}

//-------------------------------------------------------------------------------
// load a bitmap file
//-------------------------------------------------------------------------------
Sint32 bitmap_data::SDLLoadBMP(char* fpath)
{
	GFXLiberat();
	gfxSurface = SDL_LoadBMP(fpath);
	if(!gfxSurface)
	{	fprintf(stderr,
			"bitmap_data::decompacte(): Impossible to create a surface: %s\n",
			SDL_GetError());
		erreur_num = E_SDLERROR;
		return erreur_num;
	}
	gfxAdresse = (char*)gfxSurface->pixels;
	width = gfxSurface->w;
	gfx_nextLn = width;
	gfxHauteur = gfxSurface->h;
	gfx_taille = gfxHauteur * width;
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

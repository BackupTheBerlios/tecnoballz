//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "GFX_bitmap.cpp"
// created		: ?
// updates		: 2004-05-13
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
#include "../include/GFX_bitmap.h"
#include "../include/RAM_killer.h"
#include "../include/ecran_hard.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
GFX_bitmap::GFX_bitmap()
{
	GFXInitial();
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
GFX_bitmap::~GFX_bitmap()
{
	GFXLiberat();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
void GFX_bitmap::GFXInitial()
{
	mentatInit();
	gfxSurface = (SDL_Surface*)NULL;
	gfxAdresse = (char*)NULL;	// Adresse memoire du tampon
	gfxHauteur = 0;				// height in lines
	gfxLargeur = 0;				// width in pixels
	gfx_nextLn = 0;				// size of line in bytes
	gfxProfond = 0;				// number of bytes peer pixel
	gfx_taille = 0;				// size of bitmap in btyes
}

//-----------------------------------------------------------------------------
// release bitmap
//-----------------------------------------------------------------------------
void GFX_bitmap::GFXLiberat()
{
	if(gfxSurface)
	{	SDL_FreeSurface(gfxSurface);
		gfxSurface = (SDL_Surface*)NULL;
		gfxAdresse = (char*)NULL;
	}
	else
	if(gfxAdresse)
	{	memGestion->liberation(gfxAdresse);
		gfxAdresse = (char*)NULL;
	}
	mentatKill();
}

//-----------------------------------------------------------------------------
// return size of line in pixels
//-----------------------------------------------------------------------------
Sint32 GFX_bitmap::GFXlargeur()
{
	return gfxLargeur;
}

//-----------------------------------------------------------------------------
// return size of line in bytes
//-----------------------------------------------------------------------------
Sint32 GFX_bitmap::GFX_nextLn()
{
	return (gfx_nextLn);
}

//-----------------------------------------------------------------------------
// return bitmap height in lines
//-----------------------------------------------------------------------------
Sint32 GFX_bitmap::GFXhauteur()
{
	return gfxHauteur;
}

//-----------------------------------------------------------------------------
// return bitmap memory address from the corresponding coordinate
//-----------------------------------------------------------------------------
char *GFX_bitmap::GFXadresse(Sint32 posX, Sint32 posY)
{
	return (gfxAdresse + posY * gfx_nextLn + (posX * gfxProfond));
}

//-----------------------------------------------------------------------------
// return bitmap memory address
//-----------------------------------------------------------------------------
char *GFX_bitmap::GFXadresse()
{
	return gfxAdresse;
}

//-----------------------------------------------------------------------------
// return modulo 
//-----------------------------------------------------------------------------
Sint32 GFX_bitmap::GFX_modulo(Sint32 width)
{
	return (gfx_nextLn - (width * gfxProfond));
}

//-----------------------------------------------------------------------------
// return bitmap memory offset from the corresponding coordinate
//-----------------------------------------------------------------------------
Sint32 GFX_bitmap::GFXrelatif(Sint32 posX, Sint32 posY)
{
	return (posY * gfx_nextLn + posX * gfxProfond);
}

//-----------------------------------------------------------------------------
// allocate new bitmap
//-----------------------------------------------------------------------------
Sint32 GFX_bitmap::GFXnouveau(Sint32 width, Sint32 heigh, Sint32 depth)
{
	gfxLargeur = width;
	gfx_nextLn = (Sint32)(width * depth);
	gfxHauteur = heigh;
	gfx_taille = gfxHauteur * gfx_nextLn;
	gfxProfond = depth;
	gfxAdresse = memGestion->reserveMem(gfx_taille, 0x21474658);
	error_init(memGestion->retour_err());
	return erreur_num;
}

//-------------------------------------------------------------------------------
// duplicate buffer and return pointer
//-------------------------------------------------------------------------------
char* GFX_bitmap::duplicates()
{
	char *ptGfx = memGestion->reserveMem(gfx_taille, 0x21474658);
	for(Sint32 i = 0; i < gfx_taille; i++)
		ptGfx[i] = gfxAdresse[i];
	error_init(memGestion->retour_err());
	return ptGfx;
}

//-------------------------------------------------------------------------------
// copy the page into the "tampon" memory
//-------------------------------------------------------------------------------
void GFX_bitmap::copyTampon()
{
	copyTampon(0, 0, 0, 0, gfxLargeur, gfxHauteur);
}

//-------------------------------------------------------------------------------
// recopy a piece of the page into the "tampon" memory
//-------------------------------------------------------------------------------
void GFX_bitmap::copyTampon(Sint32 srceX, Sint32 srceY, Sint32 destX, Sint32 destY, 
								Sint32 large, Sint32 haute)
{
	Sint32 n = gfxLargeur;
	Sint32 o = ecran_gere->tamponNext();
	char *s = GFXadresse(srceX, srceY);
	char *d = ecran_gere->tampon_pos(destX, destY);
	Sint32 h = haute;
	for(Sint32 i = 0; i < h; i++, s += n, d += o)
	{	for(Sint32 j = 0; j < large; j++)
			d[j] = s[j];
	}
}

//-------------------------------------------------------------------------------
// recopy a piece of the page into the "buffer" memory
//-------------------------------------------------------------------------------
void GFX_bitmap::copyBuffer(Sint32 srceX, Sint32 srceY, Sint32 destX,
								Sint32 destY, Sint32 large, Sint32 haute)
{
	
	if(large == -1) large = gfxLargeur;
	if(haute == -1) haute = gfxHauteur;
	Sint32 n = gfxLargeur;
	Sint32 o = ecran_gere->bufferNext();
	char *s = GFXadresse(srceX, srceY);
	char *d = ecran_gere->buffer_pos(destX, destY);
	Sint32 h = haute;
	for(Sint32 i = 0; i < h; i++, s += n, d += o)
	{	for(Sint32 j = 0; j < large; j++)
			d[j] = s[j];
	}
}

//-------------------------------------------------------------------------------
// clear image buffer memory
//-------------------------------------------------------------------------------
void GFX_bitmap::buffer_clr(Sint32 pixel)
{
	Sint32 large = gfxLargeur;
	Sint32 h = gfxHauteur;
	Sint32 p = pixel;
	char *d = gfxAdresse;
	Sint32 n = gfxLargeur;
	for(Sint32 i = 0; i < h; i++, d += n)
	{	for(Sint32 j = 0; j < large; j++)
			d[j] = p;
	}
}

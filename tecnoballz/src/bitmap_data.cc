/** 
 * @file handler_display.cc 
 * @briefi Handle the bitmap 
 * @created 1996-06-29 
 * @date 2007-01-24
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: bitmap_data.cc,v 1.2 2007/01/24 12:32:30 gurumeditation Exp $
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
	clear_members();
}

/**
 * Release the object bitmap
 */
bitmap_data::~bitmap_data()
{
	release();
}

/**
 * Clear some members of the object
 */
void bitmap_data::clear_members()
{
	mentatInit();
	surface = (SDL_Surface*)NULL;
	pixel_buffer = (char*)NULL;
	height = 0;
	width = 0;
	row_size = 0;
	depth = 0;
	bytes_size = 0;
}

/**
 * Release bitmap surface or bitmap buffer
 */
void bitmap_data::release()
{
	if(surface != NULL)
	{	SDL_FreeSurface(surface);
		surface = (SDL_Surface*)NULL;
		pixel_buffer = (char*)NULL;
	}
	else
	if(pixel_buffer != NULL)
	{	memory->release(pixel_buffer);
		pixel_buffer = (char*)NULL;
	}
	mentatKill();
}

//-----------------------------------------------------------------------------
// return size of line in pixels
//-----------------------------------------------------------------------------
Sint32 bitmap_data::get_width()
{
	return width;
}

//-----------------------------------------------------------------------------
// return size of line in bytes
//-----------------------------------------------------------------------------
Sint32 bitmap_data::get_row_size()
{
	return row_size;
}

/**
 * Return bitmap height
 * @return the height of the bitmap in pixels
 */ 
Sint32 bitmap_data::get_height()
{
	return height;
}

/**
 * Return bitmap memory address from the corresponding coordinate
 * @return a pointer to the buffer data
 */
char *bitmap_data::get_pixel_data(Sint32 posX, Sint32 posY)
{
	return (pixel_buffer + posY * row_size + (posX * depth));
}

/** 
 * Return bitmap memory address
 * @return a pointer to the buffer data
 */
char *bitmap_data::get_pixel_data()
{
	return pixel_buffer;
}

//-----------------------------------------------------------------------------
// return modulo 
//-----------------------------------------------------------------------------
Sint32 bitmap_data::GFX_modulo(Sint32 w)
{
	return (row_size - (w * depth));
}

//-----------------------------------------------------------------------------
// return bitmap memory offset from the corresponding coordinate
//-----------------------------------------------------------------------------
Sint32 bitmap_data::GFXrelatif(Sint32 posX, Sint32 posY)
{
	return (posY * row_size + posX * depth);
}

/** 
 * Allocate a new bitmap
 * @param w width of the bitmap in pixels
 * @param h height of the bitmap in pixels
 * @param d depth of the bitmap 
 */
Sint32 bitmap_data::create(Sint32 w, Sint32 h, Sint32 d)
{
	width = w;
	row_size = (Sint32)(width * d);
	height = h;
	bytes_size = height * row_size;
	depth = d;
	pixel_buffer = memory->alloc(bytes_size);
	error_init(memory->retour_err());
	return erreur_num;
}

//-------------------------------------------------------------------------------
// duplicate buffer and return pointer
//-------------------------------------------------------------------------------
char* bitmap_data::duplicates()
{
	char *ptGfx = memory->alloc(bytes_size);
	for(Sint32 i = 0; i < bytes_size; i++)
		ptGfx[i] = pixel_buffer[i];
	error_init(memory->retour_err());
	return ptGfx;
}

//-------------------------------------------------------------------------------
// copy the page into the "tampon" memory
//-------------------------------------------------------------------------------
void bitmap_data::copyTampon()
{
	copyTampon(0, 0, 0, 0, width, height);
}

//-------------------------------------------------------------------------------
// recopy a piece of the page into the "tampon" memory
//-------------------------------------------------------------------------------
void bitmap_data::copyTampon(Sint32 srceX, Sint32 srceY, Sint32 destX, Sint32 destY, 
								Sint32 large, Sint32 haute)
{
	Sint32 n = width;
	Sint32 o = display->tamponNext();
	char *s = get_pixel_data(srceX, srceY);
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
	if(haute == -1) haute = height;
	Sint32 n = width;
	Sint32 o = display->bufferNext();
	char *s = get_pixel_data(srceX, srceY);
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
void bitmap_data::clear(Sint32 pixel)
{
	Sint32 large = width;
	Sint32 h = height;
	Sint32 p = pixel;
	char *d = pixel_buffer;
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
	erreur_num = image->create(large, haute, 1);
	if(!erreur_num)
	{	char *detPT = image->get_pixel_data(0, 0);
		char *srcPT = get_pixel_data(pos_X, pos_Y);
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
Sint32 bitmap_data::load(char* fname)
{
	char *fpath = pRessource->locateFile(fname);
	return sdl_load_bmp(fpath);
}

//-------------------------------------------------------------------------------
// load a bitmap file
//-------------------------------------------------------------------------------
Sint32 bitmap_data::load(Sint32 ident)
{
	char *fpath = pRessource->locate_res(ident);
	return sdl_load_bmp(fpath);
}

/** 
 * Load a bitmap file
 * @param fpath filename specified by path
 */
Sint32 bitmap_data::sdl_load_bmp(char* fpath)
{
	release();
	surface = SDL_LoadBMP(fpath);
	if(NULL == surface)
	{	fprintf(stderr,
			"bitmap_data::sdl_load_bmp(): Impossible to create a surface: %s\n",
			SDL_GetError());
		erreur_num = E_SDLERROR;
		return erreur_num;
	}
	pixel_buffer = (char*)surface->pixels;
	width = surface->w;
	row_size = width;
	height = surface->h;
	bytes_size = height * width;
	depth = 1;
	SDL_Color *couleurs = surface->format->palette->colors;
	Sint32 k=0;
	for (Sint32 j = 0 ; j<surface->format->palette->ncolors ; j++)
	{	GIFpalette[k++] = couleurs->r; 
		GIFpalette[k++] = couleurs->g;
		GIFpalette[k++] = couleurs->b;
		couleurs++;
	}
	erreur_num = E_NO_ERROR;
	return erreur_num;
}

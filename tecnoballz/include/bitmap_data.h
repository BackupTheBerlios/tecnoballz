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
 * $Id: bitmap_data.h,v 1.2 2007/01/24 12:32:30 gurumeditation Exp $
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
#ifndef __BITMAP_DATA__
#define __BITMAP_DATA__
class bitmap_data ;
#include "../include/mentatCode.h"

class bitmap_data : public virtual mentatCode
{
	private :	
		SDL_Surface*		surface;
    /** The actual pixel data */
		char*				pixel_buffer;
    /** Height in linges */
		Sint32				height;
    /** Width in pixels */
		Sint32				width;
    /** Real row length in bytes */
		Sint32				row_size;
    /** Bytes peer pixel 1 = 256 colors */
		Sint32				depth;
    /** Size of the bitmap in bytes */
		Sint32				bytes_size;

unsigned char		GIFpalette[768];	//palette of 256 colours
												//(256*3=768 octets)
	public :
							bitmap_data();
							~bitmap_data();

	void				clear_members();
		void			release();
		Sint32				get_width();
		Sint32				get_row_size();
		Sint32				get_height();
		char*				get_pixel_data(Sint32 xcoord, Sint32 ycoord);
		char*				get_pixel_data();
		Sint32				GFXrelatif(Sint32 posX, Sint32 posY);
		Sint32				GFX_modulo(Sint32 w);
		Sint32				create(Sint32 w, Sint32 h, Sint32 d);
		char*				duplicates();
		void				copyTampon();
		void				copyTampon(Sint32 srceX, Sint32 srceY, Sint32 destX,
										Sint32 destY, Sint32 large, Sint32 haute);
		void				copyBuffer(Sint32 srceX, Sint32 srceY, Sint32 destX,
										Sint32 destY, Sint32 large, Sint32 haute);
		void				clear(Sint32 pixel = 0);







		void				palette_go();
		void				paletteDup();	//copy first 128 colors in 128 last
		void				speciaFond();	//special 4 colors background
		unsigned char*		paletteADR();	//return palette address 
		
		Sint32	load(char* nomfichier);
		Sint32	load(Sint32);
		bitmap_data*			coupe_page(Sint32 pos_X, Sint32 pos_Y, Sint32 large, Sint32 haute);
		

	private :
		Sint32				sdl_load_bmp(char* filename);
};
#endif

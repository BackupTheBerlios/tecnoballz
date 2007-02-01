/** 
 * @file bitmap_data.h 
 * @briefi Handle the bitmap 
 * @created 1996-06-29 
 * @date 2007-02-01
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: bitmap_data.h,v 1.5 2007/02/01 13:24:22 gurumeditation Exp $
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
class bitmap_data;
#include "../include/mentatCode.h"

class bitmap_data:public virtual mentatCode
{
private:
  SDL_Surface * surface;
  /** The actual pixel data */
  char *pixel_buffer;
  /** Height in linges */
  Sint32 height;
  /** Width in pixels */
  Sint32 width;
  /** Real row length in bytes */
  Sint32 row_size;
  /** Bytes peer pixel 1 = 256 colors */
  Sint32 depth;
  /** Size of the bitmap in bytes */
  Sint32 bytes_size;
  /** Palette of 256 colors */
  unsigned char palette[768];

public:
    bitmap_data ();
   ~bitmap_data ();

  void clear_members ();
  void release ();
  SDL_Surface* get_surface ();
  Sint32 get_width ();
  Sint32 get_row_size ();
  Sint32 get_height ();
  char *get_pixel_data (Sint32 xcoord, Sint32 ycoord);
  char *get_pixel_data ();
  Sint32 get_offset (Sint32 xcoord, Sint32 ycoord);
  Sint32 get_line_modulo (Sint32 w);
  void create (Sint32 w, Sint32 h, Sint32 d);
  char *duplicate_pixel_data ();
  void copyTampon ();
  void copyTampon (Sint32 srceX, Sint32 srceY, Sint32 destX,
                   Sint32 destY, Sint32 large, Sint32 haute);
  void copyBuffer (Sint32 srceX, Sint32 srceY, Sint32 destX,
                   Sint32 destY, Sint32 large, Sint32 haute);
  void clear (Sint32 pixel = 0);

  void enable_palette ();
  void speciaFond ();           //special 4 colors background
  unsigned char *get_palette ();        //return palette address 

  void load (char *filename);
  void load (Sint32 id);
  bitmap_data *cut (Sint32 xcoord, Sint32 ycoord, Sint32 l, Sint32 h);
  bitmap_data *cut_to_surface (Sint32 xcoord, Sint32 ycoord, Sint32 l,
                               Sint32 h);

private:
  void create_surface (Sint32 width, Sint32 height, Sint32 depth,
                       Uint32 flags = SDL_ANYFORMAT, Uint32 red_mask =
                       0xf00, Uint32 green_mask = 0x0f0, Uint32 blue_mask =
                       0x00f, Uint32 alpha_mask = 0x00);
  void sdl_load_bmp (char *filename);
};
#endif

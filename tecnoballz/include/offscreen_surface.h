/** 
 * @file offscreen_surface.h 
 * @brief an offscreen drawing surface
 * @created 2007-01-31
 * @date 2007-01-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: offscreen_surface.h,v 1.3 2007/01/31 21:20:02 gurumeditation Exp $
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
#ifndef __OFFSCREEN_SURFACE__ 
#define __OFFSCREEN_SURFACE__
#include "../include/mentatCode.h"

class offscreen_surface:public virtual mentatCode
{
private:
  SDL_Surface* surface;
  /** The number of bytes used to represent each pixel in a surface */
  Uint32 bytes_per_pixel;
  Uint32 vertical_offset;
  char* pixel_data;

public:
  offscreen_surface(Sint32 width, Sint32 height, Sint32 depth, Uint32 voffset = 0);
  ~offscreen_surface();
  SDL_Surface* get_surface ();
  char* get_pixel_data ();
  char* get_pixel_data (Uint32 coord_x, Uint32 coord_x);
  Uint32 get_row_size ();
  Uint32 get_offset (Uint32 xcoord, Uint32 ycoord);
  void clear (Uint32 color = 0);
  void clear (Uint32 color, Uint32 xcoord, Uint32 ycoord, Uint32 width, Uint32 height);
  Uint32 get_width ();
  Uint32 get_height ();
  Uint32 get_line_modulo (Sint32 w);
  void lock_surface ();
  void unlock_surface ();
  void blit_surface (offscreen_surface *offscreen);
  void blit_surface (offscreen_surface *offscreen, Uint32 xcoord, Uint32 ycoord, Uint32 width, Uint32 height);
  void set_palette (SDL_Color *colors);

private:
  void create_surface (Sint32 width, Sint32 height, Sint32 depth, Uint32 flags = SDL_ANYFORMAT, Uint32 red_mask = 0xf00, Uint32 green_mask = 0x0f0, Uint32 blue_mask = 0x00f, Uint32 alpha_mask = 0x00);
};


#endif

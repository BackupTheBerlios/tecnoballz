/** 
 * @file surface_surface.h 
 * @brief an surface drawing surface
 * @created 2007-02-01
 * @date 2007-02-01
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: surface_sdl.h,v 1.1 2007/02/01 13:24:22 gurumeditation Exp $
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
#ifndef __SURFACE_SDL__ 
#define __SURFACE_SDL__
#include "../include/mentatCode.h"

class surface_sdl:public virtual mentatCode
{
protected:
  SDL_Surface* surface;
  /** The number of bytes used to represent each pixel in a surface */
  Uint32 bytes_per_pixel;
  char* pixel_data;

public:
  surface_sdl ();
  surface_sdl (Uint32 w, Uint32 h, Uint32 depth);
  ~surface_sdl ();
  SDL_Surface* get_surface ();
  char* get_pixel_data ();
  char* get_pixel_data (Uint32 coord_x, Uint32 coord_x);
  Uint32 get_row_size ();
  Uint32 get_offset (Uint32 xcoord, Uint32 ycoord);
  void clear (Uint32 color = 0);
  void clear (Uint32 color, Uint32 xcoord, Uint32 ycoord, Uint32 w, Uint32 height);
  Uint32 get_width ();
  Uint32 get_height ();
  Uint32 get_line_modulo (Uint32 w);
  void lock_surface ();
  void unlock_surface ();
  void blit_surface (surface_sdl *dest);
  void blit_surface (surface_sdl *dest, Uint32 xcoord, Uint32 ycoord, Uint32 w, Uint32 h);
  void set_palette (SDL_Color *colors);

protected:
  void create_surface (Uint32 w, Uint32 h, Sint32 depth, Uint32 flags = SDL_ANYFORMAT, Uint32 red_mask = 0xf00, Uint32 green_mask = 0x0f0, Uint32 blue_mask = 0x00f, Uint32 alpha_mask = 0x00);
};


#endif

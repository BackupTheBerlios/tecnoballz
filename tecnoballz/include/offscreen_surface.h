/** 
 * @file offscreen_surface.h 
 * @brief an offscreen drawing surface
 * @created 2007-01-31
 * @date 2007-02-15
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.8 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: offscreen_surface.h,v 1.8 2007/02/15 17:12:24 gurumeditation Exp $
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
#include "../include/tecnoballz.h"
#include "../include/surface_sdl.h"

class offscreen_surface:public virtual surface_sdl 
{
private:
  Uint32 vertical_offset;

public:
  offscreen_surface (Uint32 width, Uint32 height, Uint32 depth, Uint32 voffset = 0);
  ~offscreen_surface ();
  Uint32 get_vertical_offset ();
  void clear (Uint32 color = 0);
  void clear (Uint32 color, Uint32 xcoord, Uint32 ycoord, Uint32 w, Uint32 height);
  void blit_to_surface (offscreen_surface *offscreen);
  void blit_to_surface (offscreen_surface *offscreen, Uint32 xcoord, Uint32 ycoord, Uint32 width, Uint32 height);
  void blit_to_surface (offscreen_surface *offscreen, Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2, Uint32 w, Uint32 h);
  void blit_surface (surface_sdl *dest, Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2, Uint32 w, Uint32 h);
};


#endif

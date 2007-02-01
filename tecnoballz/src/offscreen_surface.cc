/** 
 * @file offscreen_surface.cc 
 * @brief an offscreen drawing surface
 * @created 2007-01-31
 * @date 2007-01-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: offscreen_surface.cc,v 1.4 2007/02/01 13:24:22 gurumeditation Exp $
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
#include "../include/mentatCode.h"
#include "../include/offscreen_surface.h"

/**
 * Create the offscreen surface object
 * @param w width of the offscreen in pixels 
 * @param h height of the offscreen in pixels
 * @param depth number of bits per pixel: 8, 16, or 24
 * @param voffset 
 */
offscreen_surface::offscreen_surface (Uint32 w, Uint32 h, Uint32 depth, Uint32 voffset)
{
  std::cout << "offscreen_surface() w=" << w << " h=" << h << " d=" << depth << std::endl; 
  vertical_offset = voffset;
  create_surface (w, h, depth);
  pixel_data = (char *) surface->pixels + surface->pitch * vertical_offset; 
}

/**
 * Destroy the offscreen surface object
 */
offscreen_surface::~offscreen_surface ()
{
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param offscreen pointer to a offscreen surface object
 */
void
offscreen_surface::blit_surface (offscreen_surface *offscreen)
{
  SDL_Surface* surface_dest = offscreen->get_surface ();
  SDL_Rect rect = {0, vertical_offset, surface->w, surface->h - vertical_offset};
  if (SDL_BlitSurface (surface, &rect, surface_dest, &rect) < 0)
    {
      std::cerr << "offscreen_surface::blit_surface() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param offscreen pointer to a offscreen surface object
 */
void
offscreen_surface::blit_surface (offscreen_surface *offscreen, Uint32 xcoord, Uint32 ycoord, Uint32 width, Uint32 height)
{
  SDL_Surface* surface_dest = offscreen->get_surface ();
  SDL_Rect rect = {xcoord, ycoord + vertical_offset, width, height};
  if (SDL_BlitSurface (surface, &rect, surface_dest, &rect) < 0)
    {
      std::cerr << "offscreen_surface::blit_surface() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}



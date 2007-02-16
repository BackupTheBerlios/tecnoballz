/** 
 * @file offscreen_surface.cc 
 * @brief an offscreen drawing surface
 * @created 2007-02-15
 * @date 2007-02-16
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.9 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: offscreen_surface.cc,v 1.9 2007/02/16 20:46:24 gurumeditation Exp $
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
#include "../include/tecnoballz.h"
#include "../include/offscreen_surface.h"

/**
 * Create the offscreen surface object
 * @param w width of the offscreen in pixels 
 * @param h height of the offscreen in pixels
 * @param depth number of bits per pixel: 8, 16, or 24
 * @param voffset 
 */
offscreen_surface::offscreen_surface (Uint32 w, Uint32 h, Uint32 depth,
                                      Uint32 voffset)
{
 /*
  std::
    cout << "offscreen_surface() w=" << w << " h=" << h << " d=" << depth <<
    std::endl;
    */
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
 * Get the vertical offset
 * @return vertical offset
 */
Uint32 offscreen_surface::get_vertical_offset ()
{
  return vertical_offset;
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param offscreen pointer to a offscreen surface object
 */
void
offscreen_surface::blit_to_surface (offscreen_surface * offscreen)
{
  SDL_Surface *surface_dest = offscreen->get_surface ();
  SDL_Rect rect =
    { 0, vertical_offset, surface->w, surface->h - vertical_offset };
  if (SDL_BlitSurface (surface, &rect, surface_dest, &rect) < 0)
    {
      std::cerr << "offscreen_surface::blit_to_surface() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param offscreen pointer to a offscreen surface object
 */
void
offscreen_surface::blit_to_surface (offscreen_surface * offscreen,
                                    Uint32 xcoord, Uint32 ycoord,
                                    Uint32 width, Uint32 height)
{
  SDL_Surface *surface_dest = offscreen->get_surface ();
  SDL_Rect rect = { xcoord, ycoord + vertical_offset, width, height };
  if (SDL_BlitSurface (surface, &rect, surface_dest, &rect) < 0)
    {
      std::cerr << "offscreen_surface::blit_to_surface() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param dest pointer to a surface object
 * @param x1 source x coordinate in the source and destination
 * @param y1 source y coordinate in the source and destination
 * @param x2 destination x coordinate in the source and destination
 * @param y2 destination y coordinate in the source and destination
 * @param w the width in pixels to copy 
 * @param h the height in pixels to copy
 */
void
offscreen_surface::blit_to_surface (offscreen_surface * offscreen, Uint32 x1,
                                    Uint32 y1, Uint32 x2, Uint32 y2, Uint32 w,
                                    Uint32 h)
{
  dynamic_cast < surface_sdl * >(this)->blit_to_surface (offscreen,
                                                         x1, y1,
                                                         x2,
                                                         y2 + vertical_offset,
                                                         w, h);
}

/**
 * Clear surface
 * @param color pixel color value
 */
void
offscreen_surface::clear (Uint32 color)
{
  dynamic_cast < surface_sdl * >(this)->clear (color, 0, vertical_offset,
                                               surface->w,
                                               surface->h - vertical_offset);
}

/**
 * Clear surface
 * @param color pixel color value
 * @param xcoord x coordinate in the surface
 * @param ycoord y coordinate in the surface 
 * @param w the width in pixels to clear
 * @param h the height in pixels to clear
 */
void
offscreen_surface::clear (Uint32 color, Uint32 xcoord, Uint32 ycoord,
                          Uint32 w, Uint32 h)
{
  dynamic_cast < surface_sdl * >(this)->clear (color, xcoord,
                                               ycoord + vertical_offset, w,
                                               h);
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param source pointer to a source surface object
 * @param x1 source x coordinate in the source and destination
 * @param y1 source y coordinate in the source and destination
 * @param x2 destination x coordinate in the source and destination
 * @param y2 destination y coordinate in the source and destination
 * @param w the width in pixels to copy 
 * @param h the height in pixels to copy
 */
void
offscreen_surface::blit_surface (surface_sdl * dest, Uint32 x1, Uint32 y1,
                                 Uint32 x2, Uint32 y2, Uint32 w, Uint32 h)
{
  dynamic_cast < surface_sdl * >(this)->blit_surface (dest, x1, y1, x2,
                                                      y2 + vertical_offset, w,
                                                      h);
}

/**
 * Draw a string into the offscreen
 */
void
offscreen_surface::draw_text (display_text_bitmap * display_text,
                              Uint32 xcoord, Uint32 ycoord, const char *str,
                              Uint32 length)
{
  display_text->draw (this, xcoord, ycoord + vertical_offset, str, length);
}

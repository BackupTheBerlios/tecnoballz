/** 
 * @file offscreen_surface.cc 
 * @brief an offscreen drawing surface
 * @created 2007-01-31
 * @date 2007-01-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: offscreen_surface.cc,v 1.2 2007/01/31 16:45:39 gurumeditation Exp $
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
 * @param width width of the offscreen in pixels 
 * @param height height of the offscreen in pixels
 * @param depth number of bits per pixel: 8, 16, or 24
 * @param voffset 
 */
offscreen_surface::offscreen_surface (Sint32 width, Sint32 height, Sint32 depth, Uint32 voffset)
{
  std::cout << "offscreen_surface() w=" << width << " h=" << height << " d=" << depth << std::endl; 
  vertical_offset = voffset;
  create_surface (width, height, depth, voffset);
}

offscreen_surface::~offscreen_surface ()
{
  std::cout << "offscreen_surface() destroyed!" << std::endl;
  if (NULL == surface)
    {
      SDL_FreeSurface (surface);
      surface = NULL;;
    }
}

SDL_Surface*
offscreen_surface::get_surface ()
{
  return surface;
}
/** 
 * Create an empty SDL surface
 * @param width width of the offscreen in pixels 
 * @param height height of the offscreen in pixels
 * @param depth number of byte(s) per pixel (1 to 4)
 * @param flags specifies the type of surface
 * @param red_mask
 * @param green_mask
 * @param blue_mask
 * @param alpha_mask
 */
void
offscreen_surface::create_surface (Sint32 width, Sint32 height, Sint32 depth, Uint32 flags, Uint32 red_mask, Uint32 green_mask, Uint32 blue_mask, Uint32 alpha_mask)
{

  surface = SDL_CreateRGBSurface (flags, width, height, depth, red_mask, green_mask, blue_mask, alpha_mask);
  if (NULL == surface)
    {
      std::cerr << "(!)handler_display::initialize() " <<
        "SDL_CreateRGBSurface return  " << SDL_GetError ();
      throw std::runtime_error ("SDL_CreateRGBSurface() failed!");
    }
  pixel_data = (char *) surface->pixels + surface->pitch * vertical_offset; 
  bytes_per_pixel = surface->format->BytesPerPixel;
}

/** 
 * Return offscreen memory address
 * @return a pointer to the buffer data
 */
char*
offscreen_surface::get_pixel_data ()
{
  return pixel_data;
}

/**
 * Return offscreen memory address from the corresponding coordinates
 * @param xcoord x coordinate in the offscreen
 * @param ycoord y coordinate in the offscreen 
 * @return a pointer to the pixel data
 */
char*
offscreen_surface::get_pixel_data (Uint32 xcoord, Uint32 ycoord)
{
  return pixel_data + ycoord * surface->pitch + xcoord * bytes_per_pixel;
}

/**
 * Return size of line in bytes
 * @return row size in bytes
 */
Uint32
offscreen_surface::get_row_size ()
{
  return surface->pitch;
}

/**
 * Return offscreen memory offset from the corresponding coordinate
 * @param xcoord x coordinate in the offscreen
 * @param ycoord y coordinate in the offscreen 
 * @return offset to the pixel data
 */
Uint32
offscreen_surface::get_offset (Uint32 xcoord, Uint32 ycoord)
{
  return ycoord * surface->pitch + xcoord * bytes_per_pixel;
}

/**
 * Clear offscreen
 */
void
offscreen_surface::clear (Uint32 color)
{
  SDL_Rect rect = {0, vertical_offset, surface->w, surface->h - vertical_offset};
  SDL_FillRect (surface, &rect, color);
}


/**
 * Clear offscreen
 */
void
offscreen_surface::clear (Uint32 color, Uint32 xcoord, Uint32 ycoord, Uint32 width, Uint32 height)
{
  SDL_Rect rect = {xcoord, vertical_offset + ycoord, width, height};
  SDL_FillRect (surface, &rect, color);
}

/**
 * Return the width of the offscreen 
 * @return width in pixels
 */
Uint32
offscreen_surface::get_width ()
{
  return surface->w;
}

/**
 * Return the height of the offscreen
 * @return the height of the bitmap in pixels
 */
Uint32
offscreen_surface::get_height ()
{
  return surface->h;
}

/** 
 * Return amount to add to get to the next line  
 * @param w width of source element in bytes
 * return modulo line
 */
Uint32
offscreen_surface::get_line_modulo (Sint32 w)
{
  return surface->pitch - w * bytes_per_pixel;
}

/**
 * Lock surface
 */
void
offscreen_surface::lock_surface ()
{
  if (SDL_LockSurface (surface))
    {
      std::cerr << "(!)offscreen_surface::lock_surface() " <<
        " SDL_LockSurface return " << SDL_GetError () << std::endl; 
    }
}

/**
 * Unlock surface
 */
void
offscreen_surface::unlock_surface ()
{
  SDL_UnlockSurface (surface);
}


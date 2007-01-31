/** 
 * @file handler_display.cc 
 * @brief Handle the bitmap 
 * @created 1996-06-29 
 * @date 2007-01-24
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.9 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: bitmap_data.cc,v 1.9 2007/01/31 16:45:39 gurumeditation Exp $
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
#include "../include/handler_resources.h"
#include "../include/handler_display.h"

/** 
 * Create the object bitmap
 */
bitmap_data::bitmap_data ()
{
  clear_members ();
}

/**
 * Release the object bitmap
 */
bitmap_data::~bitmap_data ()
{
  release ();
}

/**
 * Clear some members of the object
 */
void
bitmap_data::clear_members ()
{
  mentatInit ();
  surface = (SDL_Surface *) NULL;
  pixel_buffer = (char *) NULL;
  height = 0;
  width = 0;
  row_size = 0;
  depth = 0;
  bytes_size = 0;
}

/**
 * Release bitmap surface or bitmap buffer
 */
void
bitmap_data::release ()
{
  if (surface != NULL)
    {
      SDL_FreeSurface (surface);
      surface = (SDL_Surface *) NULL;
      pixel_buffer = (char *) NULL;
    }
  else if (pixel_buffer != NULL)
    {
      delete[]pixel_buffer;
      pixel_buffer = (char *) NULL;
    }
  mentatKill ();
}

/**
 * Return width of the bitmap
 * @return width in pixels
 */
Sint32 bitmap_data::get_width ()
{
  return width;
}

/**
 * Return size of line in bytes
 * @return row size in bytes
 */
Sint32 bitmap_data::get_row_size ()
{
  return row_size;
}

/**
 * Return bitmap height
 * @return the height of the bitmap in pixels
 */
Sint32 bitmap_data::get_height ()
{
  return height;
}

/**
 * Return bitmap memory address from the corresponding coordinates
 * @param xcoord x coordinate in the bitmap
 * @param ycoord y coordinate in the bitmap
 * @return a pointer to the pixel data
 */
char *
bitmap_data::get_pixel_data (Sint32 xcoord, Sint32 ycoord)
{
  return (pixel_buffer + ycoord * row_size + (xcoord * depth));
}

/** 
 * Return bitmap memory address
 * @return a pointer to the buffer data
 */
char *
bitmap_data::get_pixel_data ()
{
  return pixel_buffer;
}

/** 
 * Return amount to add to get to the next line  
 * @param w width of source element in bytes
 * return modulo line
 */
Sint32 bitmap_data::get_line_modulo (Sint32 w)
{
  return (row_size - (w * depth));
}

/**
 * Return bitmap memory offset from the corresponding coordinate
 * @param xcoord x coordinate in the bitmap
 * @param xcoord y coordinate in the bitmap
 * @return offset to the pixel data
 */
Sint32 bitmap_data::get_offset (Sint32 posX, Sint32 posY)
{
  return (posY * row_size + posX * depth);
}

/** 
 * Allocate a new bitmap
 * @param w width of the bitmap in pixels
 * @param h height of the bitmap in pixels
 * @param d depth of the bitmap 
 */
void
bitmap_data::create (Sint32 w, Sint32 h, Sint32 d)
{
  width = w;
  height = h;
  depth = d;
  row_size = (Sint32) (width * depth);
  bytes_size = height * row_size;
  try
    {
      pixel_buffer = new char[bytes_size];
    }
  catch (std::bad_alloc &)
    {
      std::
        cerr << "bitmap_data::create() not enough memory to allocate " <<
        bytes_size << " bytes " << std::endl;
      throw;
    }
  clear ();
}

/**
 * Allocate and return a copy of the current pixel data
 * @return pointer to the new pixel data
 */
char *
bitmap_data::duplicate_pixel_data ()
{
  char *pixel;
  try
    {
      pixel = new char[bytes_size];
    }
  catch (std::bad_alloc &)
    {
      std::
        cerr << "bitmap_data::duplicate_pixel_data() " <<
            "not enough memory to allocate " <<
            bytes_size << " bytes " << std::endl;
      throw;
    }
  for (Sint32 i = 0; i < bytes_size; i++)
    {
      pixel[i] = pixel_buffer[i];
    } 
  return pixel;
}

//-------------------------------------------------------------------------------
// copy the page into the "tampon" memory
//-------------------------------------------------------------------------------
void
bitmap_data::copyTampon ()
{
  copyTampon (0, 0, 0, 0, width, height);
}

//-------------------------------------------------------------------------------
// recopy a piece of the page into the "tampon" memory
//-------------------------------------------------------------------------------
void
bitmap_data::copyTampon (Sint32 srceX, Sint32 srceY, Sint32 destX,
                         Sint32 destY, Sint32 large, Sint32 haute)
{
  Sint32 n = width;
  Sint32 o = display->tamponNext ();
  char *s = get_pixel_data (srceX, srceY);
  char *d = display->tampon_pos (destX, destY);
  Sint32 h = haute;
  for (Sint32 i = 0; i < h; i++, s += n, d += o)
    {
      for (Sint32 j = 0; j < large; j++)
        d[j] = s[j];
    }
}

//-------------------------------------------------------------------------------
// recopy a piece of the page into the "buffer" memory
//-------------------------------------------------------------------------------
void
bitmap_data::copyBuffer (Sint32 srceX, Sint32 srceY, Sint32 destX,
                         Sint32 destY, Sint32 large, Sint32 haute)
{

  if (large == -1)
    large = width;
  if (haute == -1)
    haute = height;
  Sint32 n = width;
  Sint32 o = game_screen->get_row_size ();
  char *s = get_pixel_data (srceX, srceY);
  char *d = game_screen->get_pixel_data (destX, destY);
  Sint32 h = haute;
  for (Sint32 i = 0; i < h; i++, s += n, d += o)
    {
      for (Sint32 j = 0; j < large; j++)
        d[j] = s[j];
    }
}

/**
 * Clear image buffer memory
 * @param pixel value of the pixel (0 by default)
 */
void
bitmap_data::clear (Sint32 pixel)
{
  Sint32 w = width;
  Sint32 h = height;
  Sint32 p = pixel;
  char *d = pixel_buffer;
  Sint32 n = width;
  for (Sint32 i = 0; i < h; i++, d += n)
    {
      for (Sint32 j = 0; j < w; j++)
        {
          d[j] = p;
        }
    }
}

/**
 * Enable palette of the bitmap 
 */
void
bitmap_data::enable_palette ()
{
  display->enable_palette (palette);
}

/**
 * Return palette of colors
 * @return pointer to the palette
 */
unsigned char *
bitmap_data::get_palette ()
{
  return palette;
}

/** 
 * Copy a part of the bitmap in a new bitmap
 * @param xcoord 
 * @param ycoord
 * @param l width of the detination bitmap
 * @param h height of the destination bitmap
 */
bitmap_data *
bitmap_data::cut (Sint32 xcoord, Sint32 ycoord, Sint32 l, Sint32 h)
{
  bitmap_data *bmp = new bitmap_data ();
  bmp->create (l, h, depth);
  char *dest = bmp->get_pixel_data (0, 0);
  char *src = get_pixel_data (xcoord, ycoord);
  Sint32 next = row_size;
  for (Sint32 i = 0; i < h; i++, src += next)
    {
      char *s = src;
      for (Sint32 j = 0; j < l; j++)
        {
          *(dest++) = *(s++);
        }
    }
  return bmp;
}

/**
 * Load a bitmap file
 * @param fname filename specified by path
 */
void bitmap_data::load (char *fname)
{
  char * fpath = resources->locate_data_file (fname);
  sdl_load_bmp (fpath);
}

/**
 * Load a bitmap file
 * @param ident filename specified by the ID
 */
void bitmap_data::load (Sint32 id)
{
  char * fpath = resources->get_full_pathname (id);
  sdl_load_bmp (fpath);
}

/** 
 * Load a bitmap file
 * @param fpath filename specified by path
 */
void bitmap_data::sdl_load_bmp (char *fpath)
{
  release ();
  surface = SDL_LoadBMP (fpath);
  if (NULL == surface)
    {
      std::cerr << "(!)bitmap_data::sdl_load_bmp() " 
                << "SDL_LoadBMP return " 
                << SDL_GetError ()
                << std::endl; 
      throw std::runtime_error ("SDL_LoadBMP() failed!"); 
    }
  pixel_buffer = (char *) surface->pixels;
  width = surface->w;
  row_size = width;
  height = surface->h;
  bytes_size = height * width;
  depth = 1;
  SDL_Color * couleurs = surface->format->palette->colors;
  Sint32 k = 0;
  for (Sint32 j = 0; j < surface->format->palette->ncolors; j++)
    {
      palette[k++] = couleurs->r;
      palette[k++] = couleurs->g;
      palette[k++] = couleurs->b;
      couleurs++;
    }
}

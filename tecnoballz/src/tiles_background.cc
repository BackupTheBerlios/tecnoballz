/** 
 * @file tiles_background.cc 
 * @brief Draw tiles background in bricks levels 
 * @date 2007-02-01
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tiles_background.cc,v 1.1 2007/02/01 11:17:24 gurumeditation Exp $
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
#include "../include/handler_display.h"
#include "../include/tiles_background.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_resources.h"

/** 
 * Create the tiles background object
 */
tiles_background::tiles_background ()
{
  mentatInit ();
  type_of_tiles = TILES_64x64_WITH_16_COLORS; 
  palette_index = 0;
  if (resolution == 1 || bg4_colors)
    {
      type_of_tiles = TILES_32x32_WITH_4_COLORS; 
    }
}

/** 
 * Release the titles background object
 */
tiles_background::~tiles_background ()
{
  mentatKill ();
}

/**
 * Load and draw a tiles background
 * @param tiles_num 
 */
void
tiles_background::setup(Uint32 tiles_num)
{
  if (is_verbose)
    {
      std::cout << "tiles_background::setup() tiles_num:" << tiles_num << std::endl;
    }
  if (type_of_tiles > TILES_64x64_WITH_16_COLORS)
    {
      type_of_tiles = TILES_64x64_WITH_16_COLORS;
    }

  bitmap_data *fonds = new bitmap_data ();
  Sint32 width = 0;
  Sint32 height = 0;
  Sint32 *t_pos;
  switch (type_of_tiles)
    {
    case TILES_32x32_WITH_4_COLORS:
      if (is_verbose)
        {
          std::cout << "tiles_background::setup() "
            "load 16x16 tiles with 4 colors" << std::endl;
        }
      fonds->load (handler_resources::RES60BACKG);
      width = FONDLARGE1;
      height = FONDHAUTE1;
      t_pos = table_pos1;
      break;

    case TILES_64x64_WITH_16_COLORS:
    default:
      {
        if (tiles_num < 1)
          {
            tiles_num = (hasard_val & 127) + 1;
          }
        if (tiles_num > 77)
          {
            tiles_num = tiles_num - 77;
          }
        char *pathname = resources->get_tilemaps_filename (tiles_num);
        if (is_verbose)
          {
            std::cout << "tiles_background::setup() " <<
              "try to initialize" << pathname << std::endl;
          }
        fonds->load (pathname);
        width = FONDLARGE2;
        height = FONDHAUTE2;
        t_pos = table_pos1;
        if (fonds->get_width () / FONDLARGE2 > 5)
          {
            t_pos = table_pos2;
          }
      }
      break;
    }

  Sint32 dHorz = (game_screen->get_width () - 64 * resolution) / width - 1;
  Sint32 oSour = fonds->get_row_size ();
  Sint32 dVert = (240 * resolution) / height - 1;
  Sint32 mVert = (240 * resolution) % height - 1;
  if (mVert > 0)
    {
      dVert++;
    }


  Sint32 baseX;
  Sint32 baseY;
  switch (type_of_tiles)
    {
      case TILES_32x32_WITH_4_COLORS:
      /* select one of the 60 backgrounds */
      /* value from 0 to 63 */
        baseY = hasard_val & 0x3F;
        if (baseY >= 60)
          {
            baseY -= 60;
          }
        if (baseY & 0x1)
          {
            /* left side */
            baseX = fonds->get_width () / 2;
          }
        else
          {
            baseX = 0;
          }
        baseY >>= 1;
        baseY = baseY * height;
        break;

      case TILES_64x64_WITH_16_COLORS:
      default:
        baseX = 0;
        baseY = 0;
        dVert = 480 / height;
        break;
    }

  //###############################################################
  // display background map 
  //###############################################################
  Sint32 src_X = 0;
#if __WORDSIZE == 64
  Sint32 h = (long) display;
  Sint32 k = (long) fonds;
#else
  Sint32 h = (Sint32) display;  //use pointer address as random value
  Sint32 k = (Sint32) fonds;    //use pointer address as random value
#endif
  Sint32 nline;
  if (mVert > 0)
    nline = mVert;
  else
    nline = height;
  for (Sint32 det_Y = dVert; det_Y >= 0; det_Y--)
    {
      for (Sint32 det_X = dHorz; det_X > -1; det_X--)
        {
          hasard_val = hasard_val + h + k + 1 + keyboard->get_mouse_x ();
          h = h + countframe + det_Y;
          k = k + display->get_framepee ();
          src_X = hasard_val;
          src_X &= 0x0f;        //table index (0 to 15)
          src_X = t_pos[src_X]; //source position (0 to 4)
          src_X *= width;
          char *srcPT = fonds->get_pixel_data (baseX + src_X, baseY);
          char *detPT =
            background_screen->get_pixel_data (det_X * width, det_Y * height);
          switch (type_of_tiles)
            {
            case 0:
              display->buf_affx32 (srcPT, detPT, oSour, nline);
              break;
            case 1:
              display->buf_affx64 (srcPT, detPT, oSour, nline);
              break;
            case 2:
              display->buf_affx64 (srcPT, detPT, oSour, nline);
              break;
            }
        }
      nline = height;
    }

  //###############################################################
  // display top shadow
  //###############################################################
  dHorz = display->get_width () - (64 * resolution);
  dVert = display->get_height ();
  for (Sint32 det_Y = 0; det_Y < (handler_display::SHADOWOFFY * resolution);
       det_Y++)
    {
      for (Sint32 det_X = 0; det_X < dHorz; det_X++)
        {
          char *detPT = background_screen->get_pixel_data (det_X, det_Y);
          *detPT |= handler_display::SHADOW_PIX;
        }
    }

  //###############################################################
  // display right shadow
  //###############################################################
  for (Sint32 det_Y = 0; det_Y < dVert; det_Y++)
    {
      for (Sint32 det_X = (252 * resolution); det_X < (256 * resolution);
           det_X++)
        {
          char *detPT = background_screen->get_pixel_data (det_X, det_Y);
          *detPT |= handler_display::SHADOW_PIX;
        }
    }

  /* initialize color palette */
  switch (type_of_tiles)
    {
      case TILES_32x32_WITH_4_COLORS:
        set_4_color_palette ();
        break;
    
      case TILES_64x64_WITH_16_COLORS:
      default:
        unsigned char *colPT = fonds->get_palette ();
        SDL_Color *palPT = display->paletteAdr ();
        SDL_Color *palP1 = palPT;
        SDL_Color *palP2 = palP1 + 128;
        unsigned char pixel;
        for (Sint32 i = 0; i < 16; i++)
          {
            /* red */
            pixel = *(colPT++);
            palP1->r = pixel;
            pixel >>= 1;
            palP2->r = pixel;
            /* green */
            pixel = *(colPT++);
            palP1->g = pixel;
            pixel >>= 1;
            palP2->g = pixel;
            /* blue */
            pixel = *(colPT++);
            palP1->b = pixel;
            pixel >>= 1;
            palP2->b = pixel;
            palP1++;
            palP2++;
          }
        display->enable_palette (palPT);
    }
  delete fonds;

}

/**
 * x-coordinates of sources in the tiles bitmap
 */
Sint32
tiles_background::table_pos1[16] =
  { 3, 0, 0, 3, 4, 2, 1, 4, 3, 2, 1, 1, 0, 0, 2, 4 };
Sint32
tiles_background::table_pos2[16] =
  { 3, 0, 0, 3, 4, 2, 1, 4, 3, 2, 1, 1, 5, 0, 5, 4 };


/**
 * Randomly select one of 112 4-color palettes
 */
void
tiles_background::set_4_color_palette ()
{
  Sint32 j = hasard_val & 0x1ff;
  if (j >= 448)
    {
      /* 112 preset 4 color palettes */
      j -= 448;
    }
  j = j & 0xfff0;
  set_4_color_palette (j);
}

/**
 * Set next background palette of 4 colors
 */
void
tiles_background::next_4_color_palette ()
{
  palette_index += 16;
  if (palette_index >= 432)
    {
      palette_index = 0;
    }
  set_4_color_palette (palette_index);
}

/**
 * Set previous background palette of 4 colors
 */
void
tiles_background::prev_4_color_palette ()
{
  palette_index -= 16;
  if (palette_index < 0)
    {
      palette_index = 432;
    }
  set_4_color_palette (palette_index);
}

/**
 * Initialize a background palette color of 4 colors
 * @param pal_index 4 color palette number
 */
void
tiles_background::set_4_color_palette (Uint32 pal_index)
{
  palette_index = pal_index;
  char *color = &couleurs[0];
  unsigned char *colPT = (unsigned char *) (color) + pal_index;
  SDL_Color *palPT = display->paletteAdr ();
  SDL_Color *palP1 = palPT + 1;
  SDL_Color *palP2 = palP1 + 128;
  for (Sint32 i = 0; i < 4; i++)
    {
      unsigned char pixel = *(colPT++);
      /* red */
      pixel = *(colPT++);
      palP1->r = pixel;
      pixel >>= 1;
      palP2->r = pixel;
      /* green */
      pixel = *(colPT++);
      palP1->g = pixel;
      pixel >>= 1;
      palP2->g = pixel;
      /* blue */
      pixel = *(colPT++);
      palP1->b = pixel;
      pixel >>= 1;
      palP2->b = pixel;
      palP1++;
      palP2++;
    }
  display->enable_palette (palPT);
}

//------------------------------------------------------------------------------
// preset palettes: 4 colors original background (mode 320x200)
//------------------------------------------------------------------------------
// 448 / 16     : 28 preset palettes
// 4 * 4        : 16 composantes by palette
// 112 * 4      : 448 composantes
char
  tiles_background::couleurs[448] = {
  0x00, 0x40, 0x20, 0x40, 0x00, 0x60, 0x40, 0x60, 0x00, 0x80, 0x60, 0x80,
    0x00, 0xA0, 0x80, 0xA0, 0x00, 0x00, 0x20, 0x40, 0x00, 0x20,
  0x40, 0x60, 0x00, 0x40, 0x60, 0x80, 0x00, 0x60, 0x80, 0xA0, 0x00, 0x00,
    0x20, 0x20, 0x00, 0x20, 0x40, 0x40, 0x00, 0x40, 0x60, 0x60,
  0x00, 0x60, 0x80, 0x80, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x40, 0x60,
    0x00, 0x20, 0x60, 0x80, 0x00, 0x40, 0x80, 0xA0, 0x00, 0x30,
  0x40, 0x30, 0x00, 0x50, 0x60, 0x50, 0x00, 0x70, 0x80, 0x70, 0x00, 0x90,
    0xA0, 0x90, 0x00, 0x20, 0x20, 0x40, 0x00, 0x40, 0x40, 0x60,
  0x00, 0x60, 0x60, 0x80, 0x00, 0x80, 0x80, 0xA0, 0x00, 0x00, 0x40, 0x40,
    0x00, 0x20, 0x60, 0x60, 0x00, 0x40, 0x80, 0x80, 0x00, 0x60,
  0xA0, 0xA0, 0x00, 0x20, 0x00, 0x20, 0x00, 0x40, 0x20, 0x40, 0x00, 0x60,
    0x40, 0x60, 0x00, 0x80, 0x60, 0x80, 0x00, 0x00, 0x40, 0x20,
  0x00, 0x00, 0x60, 0x40, 0x00, 0x20, 0x80, 0x60, 0x00, 0x40, 0xA0, 0x80,
    0x00, 0x40, 0x20, 0x00, 0x00, 0x60, 0x40, 0x20, 0x00, 0x80,
  0x60, 0x40, 0x00, 0xA0, 0x80, 0x60, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60,
    0x20, 0x20, 0x00, 0x80, 0x40, 0x40, 0x00, 0xA0, 0x60, 0x60,
  0x00, 0x40, 0x00, 0x40, 0x00, 0x60, 0x20, 0x60, 0x00, 0x80, 0x40, 0x80,
    0x00, 0xA0, 0x60, 0xA0, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20,
  0x40, 0x20, 0x00, 0x40, 0x60, 0x40, 0x00, 0x60, 0x80, 0x60, 0x00, 0x20,
    0x40, 0x20, 0x00, 0x40, 0x60, 0x40, 0x00, 0x60, 0x80, 0x60,
  0x00, 0x80, 0xA0, 0x80, 0x00, 0x40, 0x40, 0x00, 0x00, 0x60, 0x60, 0x20,
    0x00, 0x80, 0x80, 0x40, 0x00, 0xA0, 0xA0, 0x60, 0x00, 0x00,
  0x40, 0x00, 0x00, 0x20, 0x60, 0x20, 0x00, 0x40, 0x80, 0x40, 0x00, 0x60,
    0xA0, 0x60, 0x00, 0x20, 0x20, 0x20, 0x00, 0x40, 0x40, 0x40,
  0x00, 0x60, 0x60, 0x60, 0x00, 0x80, 0x80, 0x80, 0x00, 0x40, 0x20, 0x60,
    0x00, 0x60, 0x40, 0x80, 0x00, 0x80, 0x60, 0xA0, 0x00, 0xA0,
  0x80, 0xC0, 0x00, 0x20, 0x20, 0x00, 0x00, 0x40, 0x40, 0x20, 0x00, 0x60,
    0x60, 0x40, 0x00, 0x80, 0x80, 0x60, 0x00, 0x20, 0x40, 0x60,
  0x00, 0x40, 0x60, 0x80, 0x00, 0x60, 0x80, 0xA0, 0x00, 0x80, 0xA0, 0xC0,
    0x00, 0x60, 0x40, 0x20, 0x00, 0x80, 0x60, 0x40, 0x00, 0xA0,
  0x80, 0x60, 0x00, 0xC0, 0xA0, 0x80, 0x00, 0x40, 0x00, 0x60, 0x00, 0x60,
    0x20, 0x80, 0x00, 0x80, 0x40, 0xA0, 0x00, 0xA0, 0x60, 0xC0,
  0x00, 0x40, 0x00, 0x20, 0x00, 0x60, 0x20, 0x40, 0x00, 0x80, 0x40, 0x60,
    0x00, 0xA0, 0x60, 0x80, 0x00, 0x20, 0x20, 0x60, 0x00, 0x40,
  0x40, 0x80, 0x00, 0x60, 0x60, 0xA0, 0x00, 0x80, 0x80, 0xC0, 0x00, 0x60,
    0x40, 0x00, 0x00, 0x80, 0x60, 0x20, 0x00, 0xA0, 0x80, 0x40,
  0x00, 0xC0, 0x80, 0x60, 0x00, 0x20, 0x40, 0x00, 0x00, 0x40, 0x60, 0x20,
    0x00, 0x60, 0x80, 0x40, 0x00, 0x80, 0xA0, 0x60, 0x00, 0x40,
  0x20, 0x20, 0x00, 0x60, 0x40, 0x40, 0x00, 0x80, 0x60, 0x60, 0x00, 0xA0,
    0x80, 0x80, 0x00, 0x20, 0x40, 0x40, 0x00, 0x40, 0x60, 0x60,
  0x00, 0x60, 0x80, 0x80, 0x00, 0x80, 0xA0, 0xA0
};

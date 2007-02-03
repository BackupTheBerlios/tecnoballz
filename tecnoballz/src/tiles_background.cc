/** 
 * @file tiles_background.cc 
 * @brief Draw tiles background in bricks levels 
 * @date 2007-02-01
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tiles_background.cc,v 1.4 2007/02/03 20:52:28 gurumeditation Exp $
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
  current_tiles = NULL;
  map_tiles = NULL;
  map_height = 0;
  map_width = 0;
  map_xcoord = 0;
  map_ycoord = 0;
  map_xmax = 0;
  map_ymax = 0;
  tiles_width = 0;
  tiles_height = 0;
}

/** 
 * Release the titles background object
 */
tiles_background::~tiles_background ()
{
  if (NULL != current_tiles)
    {
      delete current_tiles;
      current_tiles = NULL;
    }
  if (NULL != map_tiles)
    {
      delete map_tiles;
      map_tiles = NULL;
    }
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

  Sint32 *t_pos;
  switch (type_of_tiles)
    {
    case TILES_32x32_WITH_4_COLORS:
    {
      bitmap_data *bmp = new bitmap_data ();
      if (is_verbose)
        {
          std::cout << "tiles_background::setup() "
            "load 32x32 tiles with 4 colors" << std::endl;
        }
      bmp->load (handler_resources::RES60BACKG);
      tiles_width = TILES_32_WIDTH;
      tiles_height = TILES_32_HEIGHT;
      t_pos = table_pos1;

      /* select one of the 60 backgrounds */
      /* value from 0 to 63 */
      Uint32 x;
      Uint32 y = hasard_val & 0x3F;
      if (y >= 60)
        {
          y -= 60;
        }
      if (y & 0x1)
        {
          /* right side */
          x = bmp->get_width () / 2;
        }
      else
        {
          x = 0;
        }
      y >>= 1;
      y = y * tiles_height;
      current_tiles = bmp->cut_to_surface (x, y, 5 * tiles_width, tiles_height);
      delete bmp;
    }
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
        current_tiles = new bitmap_data ();
        current_tiles->load (pathname);
        tiles_width = TILES_64_WIDTH;
        tiles_height = TILES_64_HEIGHT;
        t_pos = table_pos1;
        if (current_tiles->get_width () / TILES_64_WIDTH > 5)
          {
            t_pos = table_pos2;
          }
      }
      break;
    }

  map_width = (game_screen->get_width () - 64 * resolution) / tiles_width;
  Sint32 oSour = current_tiles->get_row_size ();
  Sint32 mVert = (240 * resolution) % tiles_height - 1;
  map_height = (240 * resolution) / tiles_height;
  map_xmax = tiles_width * map_width;
  if (mVert > 0)
    {
      map_height++;
    }
  map_ymax = tiles_height * map_height;
  map_row_size = tiles_height * map_width;

  try
  {
    map_tiles = new Uint32[map_width * map_height];
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)tiles_background::setup() not enough memory to allocate "
        << map_width * map_height << " bytes!" << std::endl;
    throw;
  }

  Sint32 baseX;
  Sint32 baseY;
  switch (type_of_tiles)
    {
      case TILES_32x32_WITH_4_COLORS:
        baseX = 0;
        baseY = 0;
        break;

      case TILES_64x64_WITH_16_COLORS:
      default:
        baseX = 0;
        baseY = 0;
        break;
    }
 
  set_palette ();
 

  //###############################################################
  // display background map 
  //###############################################################
  Sint32 src_X = 0;
#if __WORDSIZE == 64
  Sint32 h = (long) display;
  Sint32 k = (long) current_tiles;
#else
  Sint32 h = (Sint32) display;  //use pointer address as random value
  Sint32 k = (Sint32) current_tiles;    //use pointer address as random value
#endif
  Sint32 nline;
  if (mVert > 0)
    {
      nline = mVert;
    }
  else
    {
      nline = tiles_height;
    }

  std::cout << "mVert " << mVert << " nline " << nline << std::endl;
  std::cout << "tiles_width " << tiles_width << " tiles_height " << tiles_height << std::endl;
  std::cout << "map_width:" << map_width << "; map_height:" << map_height <<std::endl;

  
  Uint32 voffset = background_screen->get_vertical_offset ();

  /* 7 => 0: 480/64 = 7.5 */
  background_screen->unlock_surface ();
  for (Sint32 det_Y = map_height - 1; det_Y >= 0; det_Y--)
    {
       //std::cout << "(" << det_Y << ")\n";
      /* 7 => 0: 512/64 = 8*/
      for (Sint32 det_X = map_width - 1; det_X >=0 ; det_X--)
        {
          //std::cout << "(" << det_Y << ", " << det_X << ")\n";
          hasard_val = hasard_val + h + k + 1 + keyboard->get_mouse_x ();
          h = h + countframe + det_Y;
          k = k + display->get_framepee ();
          src_X = hasard_val;
          src_X &= 0x0f;        //table index (0 to 15)
          src_X = t_pos[src_X]; //source position (0 to 4)
          src_X *= tiles_width;
          map_tiles[det_Y * map_height + det_X] = src_X;
          //printf("map_tiles[%i] = %i\n", det_Y * map_height + det_X, src_X);
          char *srcPT = current_tiles->get_pixel_data (baseX + src_X, baseY);
          char *detPT =
            background_screen->get_pixel_data (det_X * tiles_width, det_Y * tiles_height);
          switch (type_of_tiles)
            {
            case 0:
              display->buf_affx32 (srcPT, detPT, oSour, nline);
              break;
            case 1:
              break;
              current_tiles->blit_surface (
                background_screen, 
                src_X,
                0,
                det_X * tiles_width,
                det_Y * tiles_height + voffset,
                tiles_width,
                nline);


              break;
            case 2:
              display->buf_affx64 (srcPT, detPT, oSour, nline);
              break;
            }
        }
      nline = tiles_height;
    }
  background_screen->lock_surface ();
  /* draw top shadow */
  Uint32 hscreen = display->get_width () - (64 * resolution);
  for (Sint32 det_Y = 0; det_Y < (handler_display::SHADOWOFFY * resolution);
       det_Y++)
    {
      for (Uint32 det_X = 0; det_X < hscreen; det_X++)
        {
          char *detPT = background_screen->get_pixel_data (det_X, det_Y);
          *detPT |= handler_display::SHADOW_PIX;
        }
    }

  /* draw right shadow */
  Uint32 vscreen = display->get_height ();
  for (Uint32 det_Y = 0; det_Y < vscreen; det_Y++)
    {
      for (Sint32 det_X = (252 * resolution); det_X < (256 * resolution);
           det_X++)
        {
          char *detPT = background_screen->get_pixel_data (det_X, det_Y);
          *detPT |= handler_display::SHADOW_PIX;
        }
    }

}





//static Uint32 wwww = 0;


void
tiles_background::draw ()
{
  if (++map_ycoord > map_ymax - 1)
    {
      map_ycoord = 0;
    } 
  if (++map_xcoord > map_xmax - 1)
    {
      map_xcoord = 0;
    } 
 

  Uint32 modulo_x = map_xcoord % tiles_width;
  Uint32 modulo_y = map_ycoord % tiles_height;
  Uint32 first_width = tiles_width - modulo_x;
  Uint32 first_height = tiles_height - modulo_y;

  offscreen_surface *offscreen = game_screen;
  SDL_Surface *screen_surface = offscreen->get_surface ();
  SDL_Surface *tiles_surface = current_tiles->get_surface (); 
  Uint32 voffset = offscreen->get_vertical_offset ();
  //offscreen->clear (0, 0, voffset, 256 * resolution, 240 * resolution);
  offscreen->unlock_surface ();
  
  Uint32 height_box = offscreen->get_height () - voffset * 2;
  Uint32 width_box = offscreen->get_width() - 64 * resolution;
  
  SDL_Rect rect_src;
  SDL_Rect rect_dst;

  rect_dst.y = voffset;
  
  Uint32 vcount = (height_box - first_height) / tiles_height + 1;
  Uint32 last_height = (height_box - first_height) % tiles_height;
  
  Uint32 hcount = (width_box - first_width) / tiles_width + 1;

  for (Uint32 v = 0; v <= vcount; v++)
    {
      //printf("%i) : rect_dst.y:%i\n", v, rect_dst.y - voffset);
      if (v == 0)
        {
          rect_src.h = rect_dst.h = first_height;
          rect_src.y = modulo_y;
        }
      else
        {
          rect_src.y = 0;
          if (v == vcount && last_height > 0)
            {
              rect_src.h = last_height = last_height;
            }
          else
            {
              rect_src.h = rect_dst.h = tiles_height;
            }
        }
      rect_dst.x = 0;
      for (Uint32 h = 0; h < map_width; h++)
        {
          if (h == 0) 
            {
              rect_src.w = rect_dst.w = first_width;
              rect_src.x = modulo_x;
            }
          else
            {
              rect_src.w = rect_dst.w = tiles_width;
              rect_src.x = 0;
            }
          if (SDL_BlitSurface (tiles_surface, &rect_src, screen_surface, &rect_dst) < 0)
            {
              std::cerr << "surface_sdl::blit_surface() " <<
                "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
            }
          rect_dst.x += rect_dst.w;
        }
      rect_dst.y += rect_dst.h;
    }
  //printf("rect_dst.y:%i vcount:%i\n", rect_dst.y, vcount);



  offscreen->lock_surface ();
}

void
tiles_background::draw2 ()
{
  //if (++wwww < 5) return;
  // wwww = 0;

  offscreen_surface *offscreen = game_screen;

  if (++map_ycoord > map_ymax - 1)
    {
      map_ycoord = 0;
    } 
  
  offscreen->clear ();
  Uint32 voffset = offscreen->get_vertical_offset ();
  Uint32 hscreen = offscreen->get_height () - voffset * 2;
  Uint32 wscreen = offscreen->get_width ();

  offscreen->unlock_surface ();
  SDL_Rect src_rect;
  SDL_Rect dest_rect;

  SDL_Surface *screen_surface = offscreen->get_surface ();
  SDL_Surface *tiles_surface = current_tiles->get_surface (); 


  /*
   * x = 10
   * w = 54
   *
   */

  Uint32 modulox = map_xcoord % tiles_width;
  Uint32 moduloy = map_ycoord % tiles_height;
  Uint32 width = tiles_width - modulox;
  Uint32 height = tiles_height - moduloy;
  

  /*
   * top-left corner
   */
  Uint32 y = map_ycoord;
  Uint32 x = map_xcoord;
  src_rect.x = map_tiles[y / tiles_height * map_width + x / tiles_width];
  src_rect.x = modulox;
  src_rect.y = moduloy;
  src_rect.w = width;
  src_rect.h = height;
  dest_rect.x = 0;
  dest_rect.y = voffset;
  dest_rect.w = width;
  dest_rect.h = height;
  if (SDL_BlitSurface (tiles_surface, &src_rect, screen_surface, &dest_rect) < 0)
    {
      std::cerr << "tiles_background::draw() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }


  /*
   * left column
   */
  y = map_ycoord + tiles_height;
  src_rect.h = tiles_height;
  dest_rect.h = tiles_height;
  src_rect.y = 0;
  Uint32 ycoord = voffset + height;
  Uint32 vcount = (hscreen - height) / tiles_height;
  for (Uint32 v = 1; v <= vcount; v++)
    {
      src_rect.x = map_tiles[y / tiles_height * map_width + x / tiles_width];
      src_rect.x = 0;
      dest_rect.y = ycoord;
      if (SDL_BlitSurface (tiles_surface, &src_rect, screen_surface, &dest_rect) < 0)
        {
          std::cerr << "tiles_background::draw() " <<  v <<
            " SDL_BlitSurface() return " << SDL_GetError () << std::endl;
        }
      if (y+= tiles_height > map_ymax)
        {
          y-= map_ymax;
        }
      ycoord+= tiles_height;
    }

  /*
   * bottom-left
   */
  vcount = (hscreen - height) % tiles_height;
  if (vcount > 0)
    {
      dest_rect.y = ycoord;
      src_rect.h = vcount;
      dest_rect.h = vcount;
      src_rect.x = map_tiles[y / tiles_height * map_width + x / tiles_width];
      src_rect.x = 0;
      if (SDL_BlitSurface (tiles_surface, &src_rect, screen_surface, &dest_rect) < 0)
        {
          std::cerr << "tiles_background::draw() " <<
            "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
        }
    }
  
  /*
   * top line
   */
  y = map_ycoord + tiles_height;
  src_rect.y = moduloy;
  src_rect.w = tiles_width;
  src_rect.h = height;
  dest_rect.y = voffset + height;;
  dest_rect.w = tiles_width;
  dest_rect.h = height;
  Uint32 wcount = (wscreen - width) / tiles_width;
  Uint32 xcoord = width;
  for (Uint32 v = 1; v <= wcount; v++)
    {
      src_rect.x = map_tiles[y / tiles_height * map_width + x / tiles_width];
      src_rect.x = 0;
      dest_rect.x = xcoord;
      if (SDL_BlitSurface (tiles_surface, &src_rect, screen_surface, &dest_rect) < 0)
        {
          std::cerr << "tiles_background::draw() " <<  v <<
            " SDL_BlitSurface() return " << SDL_GetError () << std::endl;
        }
      if (x+= tiles_width > map_xmax)
        {
          x-= map_xmax;
        }
      xcoord+= tiles_width;
    }


  
  offscreen->lock_surface ();
  return;
  for (Uint32 v = 0; v < map_height; v++)
    {
         for (Uint32 h = 0; h < map_height; v++)
           {
               Uint32 x_source = map_tiles[y / tiles_height * map_width + x / tiles_width];  
               if (SDL_BlitSurface (tiles_surface, &src_rect, screen_surface, &dest_rect) < 0)
                 {
                   std::cerr << "surface_sdl::blit_surface() " <<
                     "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
                 }

           }
    }
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


void
tiles_background::set_palette ()
{
  /* initialize color palette */
  switch (type_of_tiles)
    {
      case TILES_32x32_WITH_4_COLORS:
        set_4_color_palette ();
        break;
    
      case TILES_64x64_WITH_16_COLORS:
      default:
        unsigned char *colPT = current_tiles->get_palette ();
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
}


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

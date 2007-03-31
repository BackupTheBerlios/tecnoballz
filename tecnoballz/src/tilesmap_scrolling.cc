/** 
 * @file tilesmap_scrolling.cc 
 * @brief Vertical scrolling tiles map in the main menu
 *        and the guardians levels
 * @date 2007-03-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tilesmap_scrolling.cc,v 1.7 2007/03/31 10:55:03 gurumeditation Exp $
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
#include "../include/tilesmap_scrolling.h"
#include "../include/handler_resources.h"
#include "../include/handler_memory.h"

/**
 * Create the scrolling tiles map object
 */
tilesmap_scrolling::tilesmap_scrolling ()
{
  object_init ();
  map_tiles = (Uint16 *) NULL;
  tiles_bitmap = (bitmap_data *) NULL;
  tile_width = 16 * resolution;
  tile_height = 16 * resolution;
}

/**
 * Release the scrolling tiles map object
 */
tilesmap_scrolling::~tilesmap_scrolling ()
{
  if (NULL != tiles_bitmap)
  {
    delete tiles_bitmap;
    tiles_bitmap = NULL;
  }
  if (NULL != map_tiles)
    {
      delete[]map_tiles;
      map_tiles = NULL;
    }
  if (mapAddress)
    {
      memory->release ((char *)mapAddress);
      mapAddress = NULL;
    }
  tiles_bitmap = (bitmap_data *) NULL;
  map_tiles = (Uint16 *) NULL;
  object_free ();
}

/** 
 * Return bitmap object
 * @return a pointer to the bitmap which contains all tiles
 */
bitmap_data *
tilesmap_scrolling::get_bitmap ()
{
  return tiles_bitmap;
}

/**
 * Perform some initializations
 * @param pal_id palette number identifier
 * @param map_id map number identifier 
 */
void
tilesmap_scrolling::initialize (Uint32 pal_id, Uint32 map_id)
{
  if (is_verbose)
    {
      std::cout << ">tilesmap_scrolling::initialise() Start!"
        << std::endl;
    }

  /* load the bitamp of tiles im memory */
  tiles_bitmap = new bitmap_data ();
  tiles_bitmap->load (handler_resources::BITMAP_TILESMAP);

  /* load the map file in memory */
  load_map (map_id);

  /** 20 tiles per row */
  tiles_per_row = game_screen->get_width () / tile_width;

   initMapAdr (); 
source_mod = tiles_bitmap->get_row_size ();

  y_coord = 0;

  /* Draw all tiles */
  scrolling1 (0);

  /* initialize color palette */
  enable_palette (pal_id);

  if (is_verbose)
    {
      std::cout << ">tilesmap_scrolling::initialise() End!"
        << std::endl;
    }
}

Sint32
tilesmap_scrolling::initMapAdr ()
{
  Sint32 error = 0;
  /* 320 or 640 pixels width */
  Uint32 l = tiles_bitmap->get_width ();
  /* 624 or 1248 lines height */
  Uint32 h = tiles_bitmap->get_height ();
  number_of_different_tiles = (l / tile_width) * (h / tile_height);     //780 maps
  mapAddress = (char **) memory->alloc (number_of_different_tiles * sizeof (char *));
  if (!mapAddress)
    return (memory->retour_err ());

  char **m = mapAddress;
  Sint32 nbMap = 0;
  for (Uint32 y = 0; y < h; y += tile_height)
    {
      for (Uint32 x = 0; x < l; x += tile_width)
        {
          char *p = tiles_bitmap->get_pixel_data (x, y);
          *(m++) = p;
          nbMap++;
        }
    }
  return error;
}


/**
 * Return width of a tile in pixels
 * @return width of a tile in pixels
 */
Uint32
tilesmap_scrolling::get_tiles_width ()
{
  return tile_width;
}

/**
 * Return y coordinate in the map
 * @return top left map coordinate
 */
Sint32
tilesmap_scrolling::get_y_coord ()
{
  return y_coord;
}

/*
 * Load a new map
 * @param pal_id palette number identifier
 * @param map_id map number identifier 
 */ 
void
tilesmap_scrolling::switch_map (Uint32 pal_id, Uint32 map_id)
{

  if (NULL != map_tiles)
    {
      delete[]map_tiles;
      map_tiles = NULL;
    }
  load_map (map_id);
  enable_palette (pal_id);
  y_coord = 0;
}

/** 
 * Initialize color palette
 * @param pal_id palette number identifier
 */
void
tilesmap_scrolling::enable_palette (Uint32 pal_id)
{
  SDL_Color *palPT = display->get_palette ();
  SDL_Color *palP1 = palPT;
  SDL_Color *palP2 = palP1 + 128;
  const unsigned char *colPT = colors_map;
  for (Sint32 i = 0; i < 16; i++)
    {
      unsigned char r, b, g;

      switch (pal_id)
        {
        default:
          r = *(colPT++);
          g = *(colPT++);
          b = *(colPT++);
          break;
        case 1:
          r = *(colPT++);
          b = *(colPT++);
          g = *(colPT++);
          break;
        case 2:
          b = *(colPT++);
          g = *(colPT++);
          r = *(colPT++);
          break;
        }

      palP1->r = r;
      palP1->g = g;
      palP1->b = b;
      palP1->g = g;
      palP1->b = b;
      r >>= 1;
      g >>= 1;
      b >>= 1;
      palP2->r = r;
      palP2->g = g;
      palP2->b = b;
      palP1++;
      palP2++;
    }
  display->enable_palette (palPT);
  game_screen->set_palette (tiles_bitmap);

}

//------------------------------------------------------------------------------
// color palette (16 colors)
//------------------------------------------------------------------------------
const unsigned char
  tilesmap_scrolling::colors_map[48] = { 2, 2, 2,
  17, 33, 50,
  66, 98, 130,
  19, 50, 81,
  27, 58, 90,
  85, 101, 133,
  36, 66, 98,
  39, 70, 102,
  146, 146, 178,
  82, 82, 114,
  129, 129, 161,
  59, 90, 122,
  98, 98, 130,
  42, 74, 106,
  50, 82, 114,
  111, 114, 146
};


//------------------------------------------------------------------------------
// scrolling
//------------------------------------------------------------------------------
void
tilesmap_scrolling::scrolling1 (Sint32 index)
{
  Sint32 i = y_coord + index;
  Sint32 j = MAP_HEIGHT * tile_height;   // 273 * 32 = 8736
  if (i < 0)
    i += j;
  if (i > j)
    i -= j;
  y_coord = i;
  if (resolution == 1)
    j = i & 15;
  else
    j = i & 31;
  offset_aff = j;
  i /= tile_height;
  i *= MAP_WIDTH;
  map_top_screen = map_tiles + i;
  draw ();
}

/** 
 * Draw all tiles map
 */
void
tilesmap_scrolling::draw ()
{
  offscreen_surface *offscreen = game_screen;
  SDL_Surface *screen_surface = offscreen->get_surface ();
  SDL_Surface *tiles_surface = tiles_bitmap->get_surface ();
  Uint32 voffset = offscreen->get_vertical_offset ();
  Uint32 height_box = offscreen->get_height () - voffset * 2;
  
  /*
  offscreen->unlock_surface ();
  offscreen->unlock_surface ();
  tiles_bitmap->unlock_surface ();
  */
  
  /* calculate the height of the tiles of the first line */
  Uint32 modulo_y = y_coord % tile_height;
  Uint32 first_height = tile_height - modulo_y;
  /* calculate the height of the tiles of the last line, 
   * zero value is * possible */
  Uint32 last_height = (height_box - first_height) % tile_height;
 

  SDL_Rect rect_src;
  SDL_Rect rect_dst;

  rect_dst.y = voffset;

  Uint32 vcount = (height_box - first_height) / tile_height + 1;
  rect_src.w = rect_dst.w = tile_width;
  
  Uint16 *map = map_top_screen;
  
  for (Uint32 v = 0; v <= vcount; v++)
    {
      Uint32 yoffset;
      if (v == 0)
        {
          rect_src.h = rect_dst.h = first_height;
          yoffset = modulo_y;
        }
      else
        {
          yoffset = 0;
          if (v < vcount)
            {
              rect_src.h = rect_dst.h = tile_height;
            }
          else
            {
              if (last_height > 0)
                {
                  rect_src.h = rect_dst.h = last_height;
                }
              else
                {
                 continue;
                }
            }
        }
      rect_dst.x = 0;
      for (Uint32 h = 0; h < tiles_per_row; h++)
        {
           Uint32 offset = *(map++); 
           rect_src.y = offset / MAP_WIDTH;
           rect_src.x = (offset - rect_src.y * MAP_WIDTH) * tile_width; 
           rect_src.y = rect_src.y * tile_height + yoffset;
          if (SDL_BlitSurface
              (tiles_surface, &rect_src, screen_surface, &rect_dst) < 0)
            {
              std::cerr << "(!)tilesmap_scrolling::draw_tiles() " <<
                "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
            }
          rect_dst.x += tile_width;
        }
      rect_dst.y += rect_dst.h;
    }
  //offscreen->lock_surface ();
  //tiles_bitmap->lock_surface ();
}

/**
 * Alloc a brush bitmap for the map editor
 * @param map
 * @param num_of_cols the number of tiles per lines
 * @param num_of_lines the number of lines
 * @return a bitmap data object
 */
bitmap_data*
tilesmap_scrolling::alloc_brush (Uint16 *map, Uint32 num_of_cols, Uint32 num_of_lines) 
{
  bitmap_data *brush = new bitmap_data ();
  brush->create_surface (num_of_cols * tile_width, num_of_lines * tile_height);
  game_screen->set_palette (brush);
  SDL_Surface *brush_surface = brush->get_surface ();
  SDL_Surface *tiles_surface = tiles_bitmap->get_surface ();
  SDL_Rect rect_src;
  SDL_Rect rect_dst;
  rect_src.h = rect_dst.h = tile_height;
  rect_src.w = rect_dst.w = tile_width;
  if (is_verbose) 
    {
       std::cout << "tilesmap_scrolling::alloc_brush num_of_cols:" <<
         num_of_cols << " num_of_lines:" <<  num_of_lines << std::endl;
    }
  rect_dst.y = 0;
  for (Uint32 v = 0; v < num_of_lines; v++)
    {
      rect_dst.x = 0;
      for (Uint32 h = 0; h < num_of_cols; h++)
        {
           Uint32 offset = *(map++); 
           rect_src.y = offset / MAP_WIDTH;
           rect_src.x = (offset - rect_src.y * MAP_WIDTH) * tile_width; 
           rect_src.y = rect_src.y * tile_height;
          if (SDL_BlitSurface
              (tiles_surface, &rect_src, brush_surface, &rect_dst) < 0)
            {
              std::cerr << "(!)tilesmap_scrolling::draw_tiles() " <<
                "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
            }
          rect_dst.x += tile_width;
        }
      rect_dst.y += rect_dst.h;
    }
  return brush;
}

/**
 * load and convert the map file
 * @param map_id map number identifier 
 */
void
tilesmap_scrolling::load_map (Uint32 map_id)
{
  switch (map_id)
    {
    case MAPED_GARD:
    default:
      map_id = handler_resources::RESEDMAP01;
      break;
    case MAPED_MENU:
      map_id = handler_resources::RESEDMAP03;
      break;
    case MAPED_CONG:
      map_id = handler_resources::RESEDMAP02;
      break;
    }

  /*
   * load the map of tiles
   * size of file: 10,920 bytes <=> 5,460 map tiles
   * number of column: 20
   * number of raw: 273
   * size of map code: 2 bytes 
   */
  Uint16 *file_data = (Uint16 *) resources->load_data (map_id);

  /* allocate memory for the map of tiles */
  Uint32 add_rows = display->get_height () / tile_height * 2;
  Uint32 map_size = (add_rows + MAP_HEIGHT) * MAP_WIDTH;
  try
  {
    map_tiles = new Uint16[map_size];
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)tilesmap_scrolling::load_map() "
      "not enough memory to allocate " <<
      map_size << " map elements!" << std::endl;
    throw;
  }
 
  /* convert "big-endian" to "little-endian" or "big-endian" */
  Sint32 i = 0;
  unsigned char *ptmap = (unsigned char *) file_data;
  for (Uint32 j = 0; j < (MAP_HEIGHT * MAP_WIDTH); j++)
    {
      Uint16 codem = 0;
      codem = (Uint16) ptmap[0];
      codem = codem << 8;
      codem = codem | ptmap[1];
      /* divide by 4, because increment pointer = 4 bytes */
      codem = codem >> 2;
      map_tiles[i++] = codem;
      ptmap = ptmap + 2;
    }
  delete[](char *) file_data;

  /* copy a height of the screen (for scrolling rotation) */
  for (Uint32 j = 0; j < (add_rows * MAP_WIDTH); j++)
    {
      map_tiles[i++] = map_tiles[j];
    }
  map_top_screen = map_tiles;
}

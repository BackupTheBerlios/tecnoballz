/** 
 * @file tilesmap_scrolling.h
 * @brief Vertical scrolling tiles map in the main menu
 *        and the guardians levels
 * @date 2007-02-07
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tilesmap_scrolling.h,v 1.2 2007/02/07 21:05:45 gurumeditation Exp $
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
#ifndef __TILESMAP_SCROLLING__
#define __TILESMAP_SCROLLING__

class tilesmap_scrolling;

#include "../include/handler_display.h"
#include "../include/bitmap_data.h"
#include "../include/tecnoballz.h"
#include "../include/supervisor_map_editor.h"

class tilesmap_scrolling:public virtual tecnoballz
{
  friend class supervisor_map_editor;

public:
  //static const Uint32 COLOR_GARD = 0;
  //static const Uint32 TECZ_COLOR_MENU = 1;
  //static const Uint32 COLOR_CONG = 2;
  static const Uint32 MAPED_GARD = 0;
  static const Uint32 MAPED_MENU = 1;
  static const Uint32 MAPED_CONG = 2;
  
  /** Number of tiles per row in the map */
  static const Uint32 MAP_WIDTH = 20;
  /** Number of tiles per column in the map */
  static const Uint32 MAP_HEIGHT = 273;

  typedef enum {
      TILES_COLOR_GUARDIANS,
      TILES_COLOR_MENU,
      TILES_COLOR_CONGRATULATIONS
  }
  TILES_COLOR;



private:

  Uint32 tile_width;
  Uint32 tile_height;

  /** Number of tiles per row at screen */
  Uint32 tiles_per_row;
  
  Sint32 offset_aff;
  
  /** The map of tiles */
  Uint16 *map_tiles;
  /** Fist line visible of the  map of tiles */
  Uint16 *map_top_screen;
  Sint32 y_coord;
  bitmap_data *tiles_bitmap;
  /** Number of differents tiles in the bitmap */
  Uint32 number_of_different_tiles;
  char *bitmap_adr;

  static const unsigned char colors_map[48];
public:
  Uint32 source_mod;
  char ** mapAddress;


public:
    tilesmap_scrolling ();
   ~tilesmap_scrolling ();
  bitmap_data *get_bitmap ();
  void initialize (Uint32 color_id = TILES_COLOR_GUARDIANS, Uint32 map_id = 0);
  Uint32 get_tiles_width ();
  Sint32 get_y_coord ();
  void enable_palette (Uint32 color_id = 0);
  void switch_map (Uint32 color_id, Uint32 map_id);
  void scrolling1 (Sint32 index = 0);

  Sint32 initMapAdr ();


private:
  void draw ();
  void load_map (Uint32 edmap = 0);
};
#endif
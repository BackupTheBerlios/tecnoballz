/** 
 * @file supervisor_map_editor.cc 
 * @brief The tile map editor for the menu and guardians levels 
 * @created 2004-09-13 
 * @date 2007-03-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_map_editor.h,v 1.7 2007/03/31 21:31:21 gurumeditation Exp $
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
#ifndef __SUPERVISOR_MAP_EDITOR__
#define __SUPERVISOR_MAP_EDITOR__

#include "../include/supervisor.h"
#include "../include/tilesmap_scrolling.h"
#include "../include/sprite_mouse_pointer.h"
#include "../include/bitmap_data.h"

typedef struct
{
  /** Left column of selection rectangle */
  Sint32 x1;
  /** Top row of selection rectangle */
  Sint32 x2;
  /** Right column of selection rectangle */
  Sint32 y1;
  /** Bottom row of selection rectangle */
  Sint32 y2;
  Uint32 number_of_cols;
  Uint32 number_of_raws;
  Sint32 boxOffsetY;
  Uint32 box_typeID;            //0 = d'ont display / 1 = current select / 2 = select finish
}
selected_region;


class supervisor_map_editor:public virtual supervisor
{
private:
  typedef enum 
    {
      SHOW_MAP,
      SHOW_TILES
    } SHOW_ENUM;
  static const Sint32 YCOORDNULL = -10240;
  tilesmap_scrolling *tiles_map;
  sprite_mouse_pointer *mouse_pointer;
  /** Height of the main window */
  Uint32 screen_height;
  /** Weight of the main window */
  Uint32 screen_width;
  /** SHOW_MAP or SHOW_TILES */
  Uint32 view_mode;
  /** True if the space key is pressed down */
  bool is_space_key_down;
  Sint32 titlesPosy;
  bitmap_data *tiles_bitmap;
  bool is_right_button_down;

  // brush handle
  Sint32 flagPress2;            //left button pressed
  Sint32 brush_posx;            //
  Sint32 brush_posy;            //
  Sint32 brushWidth;
  Sint32 brushHeigh;
  /** Brush composed of tiles copied from the main map */
  Uint16 *tiles_brush;
  /** Brush bitmap */
  bitmap_data *brush_bitmap;

  Sint32 box_colour;
  selected_region *current_selection;
  selected_region *map_selection;
  selected_region *tiles_selection;


  Sint32 tile_width;
  Sint32 tile_mask1;
  Sint32 tile_mask2;

  bool is_s_key_down;


public:
    supervisor_map_editor ();
   ~supervisor_map_editor ();
  Sint32 first_init ();
  Sint32 main_loop ();

private:
  void view_map_editor ();
  void view_tiles ();
  void tile2brush ();
  void map_to_brush ();
  void check_keys ();
  Sint32 get_speed ();
  void select_box ();
  void highlight_selection ();
  void alloc_brush ();
  void brush_draw ();
  Sint32 saveTheMap ();

  static const unsigned char cyclingtab[];
};
#endif

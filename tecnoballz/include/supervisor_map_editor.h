/** 
 * @file supervisor_map_editor.cc 
 * @brief The tile map editor for the menu and guardians levels 
 * @created 2004-09-13 
 * @date 2007-02-07
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_map_editor.h,v 1.4 2007/02/07 21:05:45 gurumeditation Exp $
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
  Sint32 box_pos_x1;
  Sint32 box_pos_x2;
  Sint32 box_pos_y1;
  Sint32 box_pos_y2;
  Sint32 box_widthT;
  Sint32 box_height;
  Sint32 boxOffsetY;
  Sint32 box_typeID;            //0 = d'ont display / 1 = current select / 2 = select finish
}
selectinfo;


class supervisor_map_editor:public virtual supervisor
{
private:
  static const Sint32 YCOORDNULL = -10240;
  tilesmap_scrolling *tiles_map;
  sprite_mouse_pointer *mouse_pointer;
  Sint32 screen_height;         //height of the main window
  Sint32 screen_width;          //weight of the main window
  Uint32 view_mode;            //0=show map / 1=show tiles
  Sint32 flagSpaceK;
  Sint32 titlesPosy;
  bitmap_data *ptrGBitMap;
  Sint32 flag_press;

  // brush handle
  Sint32 flagPress2;            //left button pressed
  Sint32 brush_posx;            //
  Sint32 brush_posy;            //
  Sint32 brushWidth;
  Sint32 brushHeigh;
  Uint16 *pBrushTile;
  bitmap_data *pBrush_bob;

  Sint32 box_colour;
  selectinfo *pt_select0;
  selectinfo *pt_select1;
  selectinfo *pt_select2;


  Sint32 tile_width;
  Sint32 tile_mask1;
  Sint32 tile_mask2;

  Sint32 keyS_press;


public:
    supervisor_map_editor ();
   ~supervisor_map_editor ();
  Sint32 first_init ();
  Sint32 main_loop ();

private:
  void view_map_editor ();
  void view_tiles ();
  void tile2brush ();
  void maps2brush ();
  void check_keys ();
  Sint32 get_speed ();
  void select_box ();
  void drawingBox ();
  void brushAlloc ();
  void brush_draw ();
  Sint32 saveTheMap ();

  static const unsigned char cyclingtab[];
};
#endif

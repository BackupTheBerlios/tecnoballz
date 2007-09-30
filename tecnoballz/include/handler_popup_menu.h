/** 
 * @file handler_popup_menu.h
 * @brief popup menu handler (When the [Esc] is pressed)
 * @created 2004-08-08 
 * @date 2007-09-30
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_popup_menu.h,v 1.7 2007/09/30 18:59:52 gurumeditation Exp $
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#ifndef __HANDLER_POPUP_MENU__
#define __HANDLER_POPUP_MENU__
class handler_popup_menu;

#include "../include/tecnoballz.h"
#include "../include/display_text_bitmap.h"
#include "../include/sprite_object.h"

class handler_popup_menu:public virtual display_text_bitmap,
  public virtual sprite_object
{
private:
  static const Sint32 NULL_YCOORD = -10240;

private:
  /** Pointer to the text file loaded */
  char **texts_of_menus;
  bitmap_data * screen_menu;
  /** Number of columns of the menu: 25 chars per line */
  Uint32 num_of_columns;
  /** Number of lines of the current menu: 3 or 4 lines */ 
  Uint32 num_of_lines;
  Uint32 vertical_space;
  Sint32 y_coord_left_down;
  Sint32 y_coord_right_down;
  Sint32 menu_color;
  /** Trur if restore background when leave menu */
  bool is_restore_background;
  Uint32 menu_number;

public:
  typedef enum
   {
    CONTINUE_PLAY_CURRENT = 1,
    CAUSE_GAME_OVER,
    QUIT_TO_MAIN_MENU,
    QUIT_TECNOBALLZ
  }
  MENU_ITEMS_ENUM;

  typedef enum
   {
    MENU_00,
    MENU_01 = 4,
    MAX_OF_LINES = 7,
   }
  MENU_TEXTS_ENUM;

  static const Uint32 MAX_OF_CHARS = 25;

public:
    handler_popup_menu ();
   ~handler_popup_menu ();

  void first_init (bitmap_data * bmp);
  Sint32 run ();

private:
  void initialize_palette();
  void build_menu_box(bitmap_data * bmp, Uint32 width);
  void load_text_file();
  void draw (char *ptSrc, Sint32 pos_x, Sint32 pos_y, Sint32 NxLine,
                   Sint32 width, Sint32 heigh);
  void display_640 ();
  void display_320 ();
  static const unsigned char cycling_table[];
  static char **menu_texts_pt[2];

};
#endif

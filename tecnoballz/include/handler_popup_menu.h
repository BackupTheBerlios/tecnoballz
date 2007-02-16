/** 
 * @file handler_popup_menu.h
 * @brief popup menu handler (When the [Esc] is pressed)
 * @created 2004-08-08 
 * @date 2007-02-08
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_popup_menu.h,v 1.4 2007/02/16 12:38:23 gurumeditation Exp $
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
  static const Sint32 YCOORDNULL = -10240;

private:
  bitmap_data * graphTexte;
  Uint32 num_of_columns;
  Uint32 num_of_lines;
  Uint32 vertical_space;
  Sint32 yPressLeft;
  Sint32 yPressRigh;
  Sint32 menu_color;
  bool is_restore_background;            //1: restore background when leave menu
  Uint32 menu_number;

public:
  typedef enum {
    WECONTINUE = 1,
    GOGAMEOVER,
    EXITTOMENU,
    WEQUITGAME
  }
  MENU_ITEMS_ENUM;

public:
    handler_popup_menu ();
   ~handler_popup_menu ();

  void first_init (bitmap_data * bmp, Uint32 menu_num, Uint32 w,
                     bool is_restore = true, bool is_palette = false);
  Sint32 execution1 ();

private:
  void displayBOB (char *ptSrc, Sint32 pos_x, Sint32 pos_y, Sint32 NxLine,
                   Sint32 width, Sint32 heigh);
  void display640 ();
  void display320 ();
  static const unsigned char cyclingtab[];
  static const char *menu_texts[2];
  static const char menu_text1[];
  static const char menu_text2[];

};
#endif

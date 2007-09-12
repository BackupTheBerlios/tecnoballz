/** 
 * @file supervisor_main_menu.h
 * @brief TecnoballZ's main menu supervisor 
 * @date 2007-04-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.10 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_main_menu.h,v 1.10 2007/09/12 06:32:48 gurumeditation Exp $
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
#ifndef __SUPERVISOR__MAIN_MENU__
#define __SUPERVISOR__MAIN_MENU__

#include "../include/tilesmap_scrolling.h"
#include "../include/supervisor.h"
#include "../include/handler_keyboard.h"
#include "../include/sprite_object.h"
#include "../include/list_sprites.h"
#include "../include/controller_fontes_menu.h"
#include "../include/display_text_bitmap.h"
#include "../include/sprite_display_menu.h"
#include "../include/sprite_mouse_pointer.h"
#include "../include/handler_audio.h"

class supervisor_main_menu:public virtual supervisor
{
public:
  static const Uint32 AREA_CODE_LENGTH = 10;

private:
  tilesmap_scrolling * tiles_map;
  sprite_object *tecnoballz_logo;
  controller_fontes_menu *fonts_scrolling;
  sprite_display_menu *text_menu;
  Sint32 offset_xx1;
  sprite_mouse_pointer *mouse_pointer;
  static const char area_codes[241];
  /** Current input area code used to jump
   * directly to the end of a area */
  static char current_area_code[AREA_CODE_LENGTH + 1];

public:
    supervisor_main_menu ();
   ~supervisor_main_menu ();
  void first_init ();
  Sint32 main_loop ();
  static const char *get_area_code (Uint32 aera_num, Uint32 difficulty);
  static char *get_current_area_code ();
  static void copy_current_area_code (char *destination);

private:
  void offset_inc ();
  void move_tecnoballz_logo ();
  Sint32 start_new_game ();
  Uint32 check_area_code ();
};
#endif

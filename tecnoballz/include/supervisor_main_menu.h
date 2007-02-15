/** 
 * @file supervisor_main_menu.h
 * @brief TecnoballZ's main menu supervisor 
 * @date 2007-02-14
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_main_menu.h,v 1.5 2007/02/15 17:12:24 gurumeditation Exp $
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
#ifndef __SUPERVISOR__MAIN_MENU__
#define __SUPERVISOR__MAIN_MENU__

#include "../include/tilesmap_scrolling.h"
#include "../include/supervisor.h"
#include "../include/handler_keyboard.h"
#include "../include/sprite_object.h"
#include "../include/list_sprites.h"
#include "../include/controller_fontes_menu.h"
#include "../include/print_text.h"
#include "../include/print_menu.h"
#include "../include/sprite_mouse_pointer.h"
#include "../include/handler_audio.h"

class supervisor_main_menu:public virtual supervisor
{
private:
  tilesmap_scrolling * tiles_map;
  sprite_object *tecnoballz_logo;
  controller_fontes_menu *BOB_defile;
  print_menu *menu_texte;
  Sint32 offset_xx1;
  sprite_mouse_pointer *mouse_pointer;
  static const char codesarea[241];

public:
    supervisor_main_menu ();
   ~supervisor_main_menu ();
  Sint32 first_init ();
  Sint32 main_loop ();
  static const char *getTheCode (Uint32 arean, Uint32 hardc);

private:
  void offset_inc ();
  void move_tecnoballz_logo ();
  Sint32 start_new_game ();
  Uint32 check_area_password ();
};
#endif

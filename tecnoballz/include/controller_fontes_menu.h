/** 
 * @file controller_fontes_menu.cc 
 * @brief Controller of the menu scroll text 
 * @date 2007-03-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_fontes_menu.h,v 1.3 2007/03/31 21:31:21 gurumeditation Exp $
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
#ifndef __CONTROLLER__FONTS_MENU__
#define __CONTROLLER__FONTS_MENU__

#include "../include/objects_list.h"
#include "../include/sprite_fonte_menu.h"
class controller_fontes_menu:public objects_list < sprite_fonte_menu, controller_fontes_menu >
{
private:
  static const Uint32 MAX_OF_FONTS = 14;  //numbers maximum characters
  //static char scrollText[];
  static char asciiToBob[128];

private:
  sprite_fonte_menu * characters_list[MAX_OF_FONTS + 2];
  sprite_fonte_menu *objectLeft;
  sprite_fonte_menu *objectRigh;
  sprite_fonte_menu **object_ptr;
  char *scroll_ptr;
  Sint32 offset_xx1;
  Sint32 offset_yy1;
  char* scrollText;
  char** scrolltexts;

public:
    controller_fontes_menu ();
   ~controller_fontes_menu ();
  void create_fontes_list ();
  void move_chars ();

};
#endif

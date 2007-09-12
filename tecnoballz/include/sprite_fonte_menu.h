/** 
 * @file sprite_fonte_menu.h
 * @brief The fonte sprite used in the main menu 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_fonte_menu.h,v 1.3 2007/09/12 06:32:48 gurumeditation Exp $
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
#ifndef __SPRITE_FONTE_MENU__
#define __SPRITE_FONTE_MENU__

#include "../include/sprite_object.h"

class sprite_fonte_menu:public virtual sprite_object
{
  friend class controller_fontes_menu;

protected:
  Sint32 zeCosValue;
  Sint32 zeSinValue;

public:
    sprite_fonte_menu ();
   ~sprite_fonte_menu ();
};
#endif

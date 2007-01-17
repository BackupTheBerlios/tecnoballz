/** 
 * @file list_sprites.h
 * @brief Call the drawing methods of all sprites
 * @date 2007-01-16
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: list_sprites.h,v 1.1 2007/01/16 21:27:13 gurumeditation Exp $
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
#ifndef __LIST_SPRITES__
#define __LIST_SPRITES__

class list_sprites;
#include "../include/sprite_object.h"
#include "../include/handler_memory.h"
#include "../include/mentatCode.h"
//const Sint32 BOBNumMaxi = 200;

class list_sprites:public mentatCode
{
private:
  Sint32 num_of_shapes;
  Sint32 num_of_shadows;
  Sint32 max_of_shapes;
  sprite_object **shapes;
  sprite_object **shadows;

public:
    list_sprites ();
   ~list_sprites ();
  Sint32 init (Sint32 numof);
  void reset ();
  void add (sprite_object * sprite);
  void draw ();
  void clear ();
};

extern list_sprites *sprites;

#endif
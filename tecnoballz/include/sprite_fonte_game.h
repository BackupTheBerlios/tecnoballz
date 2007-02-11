/** 
 * @file sprite_fonte_game.h
 * @brief The sprite fonte used to display the text "LEVEL COMPLETED" 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_fonte_game.h,v 1.2 2007/02/11 16:04:44 gurumeditation Exp $
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
#ifndef __SPRITE_FONTE_GAME__
#define __SPRITE_FONTE_GAME__
class sprite_fonte_game;
#include "../include/sprite_object.h"

class sprite_fonte_game:public sprite_object
{
  friend class controller_fontes_game;

private:
   Sint32 angleValue;          //radius value 0 to 511
  Sint32 posiYStart;            //start Y coordinate
  Sint32 posiX_stop;            //stop X coordinate
  Sint32 xIncrement;            //X move offset(1, -1 or 0)
  Sint32 yIncrement;            //Y move offset (1)
  Sint32 posiY_stop;            //stop Y coordinate

public:
    sprite_fonte_game ();
   ~sprite_fonte_game ();
  void littleInit (Sint32 a, Sint32 x, Sint32 y, Sint32 i,
                   Sint32 l, Sint32 s);
  void moveCaract ();
};
#endif

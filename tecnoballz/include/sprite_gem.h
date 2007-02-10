/** 
 * @file sprite_gem.h
 * @brief The gem sprite 
 * @created 2004-04-12 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_gem.h,v 1.3 2007/02/10 09:57:16 gurumeditation Exp $
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
#ifndef __SPRITE_GEM__
#define __SPRITE_GEM__
class sprite_gem;

#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/sprite_projectile.h"

enum
{
  GREYSQUARE,
  GREENSPHER,
  YELLOWRING,
  BLUETRIANG,
  RHOMBUGOLD,
  PENTABRONZ
};

class sprite_gem:public sprite_object
{
  friend class controller_moneys;

private:
  Sint32 directionX;
  Sint32 la_vitesse;
  Sint32 indicator1;
  Sint32 typeof_gem;
  sprite_paddle *raquettePT;
  Sint32 blinkcount;
  Sint32 rand_count;

public:
    sprite_gem ();
   ~sprite_gem ();
  void littleInit ();
  Sint32 disponible (sprite_ball * pball);
  Sint32 disponible (sprite_projectile * pfire);
  void initialGem (Sint32 pos_x, Sint32 pos_y, sprite_paddle * raket);
  void gemcollect (Sint32 ztype);
  void activBlink ();
  Sint32 move ();

private:
  static const Sint32 gem_random[8];
};
#endif

/** 
 * @file sprite_money.h
 * @brief The money sprite 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_money.h,v 1.4 2007/02/10 20:22:17 gurumeditation Exp $
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
#ifndef __SPRITE_MONEY__
#define __SPRITE_MONEY__
class sprite_money;

#include "../include/sprite_object.h"
#include "../include/sprite_paddle.h"
#include "../include/right_panel_score.h"
#include "../include/handler_display.h"
#include "../include/controller_bricks.h"
#include "../include/sprite_ball.h"
#include "../include/sprite_projectile.h"

class sprite_money:public sprite_object
{
  friend class controller_moneys;

private:
  Sint32 directionX;
  Sint32 valeurFric;
  Sint32 la_vitesse;
  sprite_paddle *raquettePT;
  Sint32 area_multi;

public:
    sprite_money ();
   ~sprite_money ();
  void littleInit ();
  Sint32 disponible (brickClear * briPT);
  Sint32 disponible (sprite_ball * pball);
  Sint32 disponible (sprite_projectile * pfire);

  void init_money (Sint32 pos_x, Sint32 pos_y, sprite_paddle * raket);

  Sint32 move ();
  Sint32 deplaceMe2 ();
};
#endif

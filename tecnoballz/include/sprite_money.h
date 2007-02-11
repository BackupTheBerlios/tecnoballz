/** 
 * @file sprite_money.h
 * @brief The money sprite 
 * @date 2007-02-11
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_money.h,v 1.6 2007/02/11 12:35:11 gurumeditation Exp $
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
  /** */
  Uint32 money_amount;
  Uint32 speed_of_moving;
  sprite_paddle *paddle;
  /** multiply the amount of money collected in the capsule
   * by the paddle */ 
  Uint32 money_multiplier;

public:
    sprite_money ();
   ~sprite_money ();
  void littleInit ();
  bool disponible (brickClear * briPT);
  bool disponible (sprite_ball * pball);
  bool disponible (sprite_projectile * pfire);

  void init_money (Uint32 xcoord, Uint32 ycoord, sprite_paddle * raket);

  Uint32 move ();
  Uint32 move_bottom ();
};
#endif

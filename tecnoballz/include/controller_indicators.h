/** 
 * @file controller_indicators.h
 * @brief Controller of money amount, reverse penalty   
 * @created 2002-11-28 
 * @date 2007-02-11
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_indicators.h,v 1.2 2007/02/13 17:11:02 gurumeditation Exp $
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
#ifndef __CONTROLLER_INDICATORS__
#define __CONTROLLER_INDICATORS__
class controller_indicators;

#include "../include/objects_list.h"
#include "../include/handler_players.h"
#include "../include/controller_paddles.h"
#include "../include/sprite_capsule.h"
#include "../include/sprite_money.h"

class controller_indicators:public objects_list < sprite_object, controller_indicators >
{
private:
  static const Sint32 YINVERSEUR = 232;
  Sint32 y_position;
  controller_paddles *ptZraquett;
  Sint32 money_posy;
  /** Sprite of the money capsule to the left of amount money */
  sprite_money *money_sprite;
  sprite_capsule *reverse_sprite;
  sprite_capsule *life_sprite;

public:
    controller_indicators ();
   ~controller_indicators ();
  void create_indicators_sprites (controller_paddles * pads, sprite_money *money, sprite_capsule *reverse, sprite_capsule *life);
  void execution1 (Sint32 value);
  void execution2 (Sint32, Sint32);
  void increase_money_amount (Uint32 amount);

private:
  void init_money ();
  void exec_money (Sint32);
};
#endif

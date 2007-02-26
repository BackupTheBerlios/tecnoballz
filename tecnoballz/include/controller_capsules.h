/** 
 * @file controller_capsules.h
 * @brief Capsules controller 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.8 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_capsules.h,v 1.8 2007/02/26 09:01:03 gurumeditation Exp $
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
#ifndef __CONTROLLER_CAPSULES__
#define __CONTROLLER_CAPSULES__
class controller_capsules;

#include "../include/objects_list.h"
#include "../include/sprite_capsule.h"
#include "../include/short_info_messages.h"
#include "../include/controller_paddles.h"
#include "../include/controller_balls.h"

class controller_capsules:public objects_list < sprite_capsule, controller_capsules >
{
public:
  static const Sint32 SGADGET_X1 = 146; //abscissa of the first bonus
  static const Sint32 SGADGET_Y1 = 2;   //ordinate of the first bonus 
  static const Sint32 SGADGET_X2 = 280; //abscissa of the bonus indicator
  static const Sint32 SGADGET_Y2 = 204; //ordinate of the bonus indicator

private:
  sprite_paddle * paddle_selected; //bump select (cheat mode)
  short_info_messages *ptMiniMess;       //use for display minis messages
  controller_balls *ptNewBalls;
  sprite_object *ptBob_wall;
  Sint32 frame_delay;
  Sint32 frame_period;
  Sint32 frame_index;

  /** Overview capsule of a option in the shop */
  sprite_capsule *overview_capsule;

  // maluses
  Sint32 malus_step;            // drop malus counter
  Sint32 malus_frek;            // drop frequency malus
  const Sint16 *random_list;     // table of the maluses

  /** Counter number of bricks breaked before release a new
   * bonus caspule bought in the shop */ 
  Uint32 bricks_breaked_count;
  /* number of bonus capsules buy */
  Uint32 num_of_caspules_bought;
  /** Current number of bonus capsules bought in the shop released */
  Uint32 capsules_released_count;
  Uint32 bonus_step;            // drop frequency bonuses
  /** index of the next bonus capsule of the shopping cart */
  Uint32 shopping_cart_index;
  //Sint16 shopping_cart[MAX_OF_CAPSULES_BOUGHT + 1];
  Sint32 *shopping_cart;

  static Sint16 keysTriche[];
  static const Uint16 randomlist[128];

public:
    controller_capsules ();
   ~controller_capsules ();
  void initialize (Sint32 mStep, const Sint16 * table, short_info_messages * ptMes, controller_balls * pBall, sprite_object *);
  void send_capsule_from_bricks (brickClear * briPT);
  void send_malus (sprite_ball *);
  void send_malus (sprite_projectile *);
  void check_if_send_capsule (sprite_ball * pball);
  void create_shop_sprites_list ();
  void set_overview_capsule (Uint32 id);
  void move_in_bricks_levels ();
  void move_in_guardians_levels ();
  void play_animation_in_shop (Uint32 value = 0);
  void cheat_keys ();


private:
  void gadget_run (sprite_paddle * paddle, Sint32 nuGad);
  void gadgetrun2 (sprite_paddle * paddle, Sint32 nuGad);

};
#endif

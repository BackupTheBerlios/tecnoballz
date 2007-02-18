/** 
 * @file controller_capsules.h
 * @brief Capsules controller 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_capsules.h,v 1.6 2007/02/18 21:07:00 gurumeditation Exp $
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
#include "../include/zeMiniMess.h"
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
  zeMiniMess *ptMiniMess;       //use for display minis messages
  controller_balls *ptNewBalls;
  sprite_object *ptBob_wall;
  Sint32 frame_delay;
  Sint32 frame_period;
  Sint32 frame_index;

  // shop
  sprite_capsule *temoin_gad;   // bonus indicator

  // maluses
  Sint32 malus_step;            // drop malus counter
  Sint32 malus_frek;            // drop frequency malus
  const Sint16 *malusTable;     // table of the maluses

  // bonuses
  Sint32 brick_kass;            // counter of bricks breaks
  Sint32 bonusAchet;            // number of bonuses buy
  Sint32 bonusTombe;            // number of bonuses droped
  Sint32 bonus_step;            // drop frequency bonuses
  Sint32 course_ptr;            // pointer to "courseList"
  Sint16 courseList[NB_OPTIONS + 1];
  static Sint16 keysTriche[];
  static const Uint16 randomlist[128];

public:
    controller_capsules ();
   ~controller_capsules ();
  void initialize (Sint32 mStep, Sint32 mKauf, Sint32 bStep,
                   const Sint16 * table, Sint32 * cours,
                   zeMiniMess * ptMes,
                   controller_balls * pBall, 
                   sprite_object *);
  void envoieGads (brickClear * briPT);
  void send_malus (sprite_ball *);
  void send_malus (sprite_projectile *);
  void envoieGads (sprite_ball * pball);
  void create_shop_sprites_list ();
  void gadgetShop (Sint32 nuGad);
  void move_in_bricks_levels ();
  void move_in_guardians_levels ();
  void animations (Sint32 value = 1);
  void cheat_keys ();


private:
  void gadget_run (sprite_paddle * paddle, Sint32 nuGad);
  void gadgetrun2 (sprite_paddle * paddle, Sint32 nuGad);

};
#endif

/** 
 * @file sprite_capsule.h
 * @brief The capsule sprite which contains a bonus or a penalty 
 * @date 2007-04-13
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.9 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_capsule.h,v 1.9 2007/10/04 05:54:41 gurumeditation Exp $
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
#ifndef __SPRITE_CAPSULE__
#define __SPRITE_CAPSULE__
class sprite_capsule;

#include "../include/sprite_object.h"
#include "../include/sprite_paddle.h"
#include "../include/right_panel_score.h"
#include "../include/handler_display.h"
#include "../include/sprite_projectile.h"

const Sint32 XXX_IMAGES = 7;
const Sint16 XXX_VIDE00 = -1;
const Sint16 XXX_SIZE_P = 0 * XXX_IMAGES;
const Sint16 XXX_SIZE_M = 1 * XXX_IMAGES;
const Sint16 XXX_LIFE_P = 2 * XXX_IMAGES;
const Sint16 XXX_LIFE_M = 3 * XXX_IMAGES;
const Sint16 XXX_POWER1 = 4 * XXX_IMAGES;
const Sint16 XXX_POWER2 = 5 * XXX_IMAGES;
const Sint16 XXX_BALLE2 = 6 * XXX_IMAGES;
const Sint16 XXX_BALLE3 = 7 * XXX_IMAGES;
const Sint16 XXX_FIRE01 = 8 * XXX_IMAGES;
const Sint16 XXX_GLUE00 = 9 * XXX_IMAGES;
const Sint16 XXX_INVERS = 10 * XXX_IMAGES;
const Sint16 XXX_FIRE02 = 11 * XXX_IMAGES;
const Sint16 XXX_SIZE01 = 12 * XXX_IMAGES;
const Sint16 XXX_SIZE02 = 13 * XXX_IMAGES;
const Sint16 XXX_RANDOM = 14 * XXX_IMAGES;
const Sint16 XXX_MEGA00 = 15 * XXX_IMAGES;
const Sint16 XXX_WALL01 = 16 * XXX_IMAGES;
const Sint16 XXX_ROBOT1 = 17 * XXX_IMAGES;
const Sint16 XXX_CONTRO = 18 * XXX_IMAGES;
const Sint16 XXX_PROTEC = 19 * XXX_IMAGES;

class sprite_capsule:public sprite_object
{
  friend class controller_capsules;
  friend class supervisor_shop;


public:
typedef enum 
{
  GAD_VIDE00 = 0,
  GLUE = 2,
  NEXT_LEVEL = 4,
  /** Paddle invincibility capsule is available only
   * in the guardians level */
  PADDLE_INVINCIBILITY = 4,
  FIRE_POWER_1 = 6,
  FIRE_POWER_2 = 8,
  SHRINK_PADDLE = 10,
  EXPAND_PADDLE = 12,
  LOSE_A_LIFE = 14,
  EXTRA_LIFE = 16,
  EXTRA_BALLS = 18,
  MULTI_BALLS = 20,
  POWER_BALL_1 = 22,
  POWER_BALL_2 = 24,
  INVERSE_CONTROL = 26,
  /** The "rebuild wall" option is not associated with a capsule,
   * but it is available only in the shop */
  REBUILD_THE_WALL = 28,
  /** The "maximum speed of balls" option is not associated with
   * a capsule, but is only available as a part of chance capsule */
  MAXIMUM_SPEED_OF_BALLS = 30,
  ENABLE_BOTTOM_PADDLE = 32,
  ENABLE_RIGHT_PADDLE = 34,
  ENABLE_TOP_PADDLE = 36,
  ENABLE_LEFT_PADDLE = 38,
  BALL_SIZE_2 = 40,
  BALL_SIZE_3 = 42,
  CHANCE = 44,
  ENABLE_HUGELY_OPTIONS = 46,
  /** The "set to the prices to 1" option is not associated with a
   * capsule, but is only available as a part of chance capsule */
  SET_THE_PRICES_TO_1 = 48,
  BOTTOM_WALL = 50,
  ROBOT_PADDLE = 52,
  BALLS_CONTROL = 54,
  MAGNETIC_EYE = 56,
  LESS_BRICKS = 58,
  SOME_INFOS = 60,
  LEAVE_SHOP = 62,
}
CASPULES_ENUM;




private:
  /** Capsule direction: BOTTOM_PADDLE, RIGHT_PADDLE, TOP_PADDLE,
   * or LEFT_PADDLE */
  Uint32 towards;
  Uint32 capsule_identifier;
  sprite_paddle *paddle;
  static const Sint16 gagdetBrik[];
  static const Sint16 gagdetGuar[];

public:
    sprite_capsule ();
   ~sprite_capsule ();
  void init_members ();
  void enable_indicator_capsule (Uint32 id);
  void enable_capsule (brick_redraw * brick, Uint32 id);
  void enable_capsule (sprite_ball * ball, Uint32 id);
  void enable_capsule (sprite_projectile * blast, Uint32 id);
  void enable_guardian_capsule (sprite_ball * ball, Uint32 id);
  void set_in_shop (Uint32 id);
  void clone_from_capsule (sprite_capsule * capsule);
  Uint32 get_id ();
  sprite_paddle *move ();
private:
  void set_new_capsule (Uint32 id, const Sint16 *frames, Uint32 xcoord, Uint32 ycoord, sprite_paddle *pad);
  void enable_capsule (Uint32 index);
};
#endif

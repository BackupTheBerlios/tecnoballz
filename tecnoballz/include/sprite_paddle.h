/** 
 * @file sprite_paddle.h
 * @brief A paddle sprite 
 * @date 2007-10-02
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.19 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_paddle.h,v 1.19 2007/10/07 14:22:12 gurumeditation Exp $
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
#ifndef __SPRITE_PADDLE__
#define __SPRITE_PADDLE__

class sprite_paddle;

#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/controller_projectiles.h"
#include "../include/bitmap_data.h"
#include "../include/sprite_bullet.h"
#include "../include/controller_gigablitz.h"

class sprite_paddle:public sprite_object
{
  friend class controller_paddles;
  friend class controller_balls;
  friend class controller_projectiles;
  friend class sprite_bullet;
  friend class controller_viewfinders;
  friend class controller_gigablitz;
public:
  typedef enum
   {
     NOT_OWN_GUN,
     OWN_GUN,
     FIRE = 3
   } FIRE_PADDLE_STATUS;

   typedef enum 
   {
    NOT_STICKY_PADDLE,
    FREE_STICKY_PADDLE,
    BUSY_STICKY_PADDLE
   } STICKY_PADDLE_STATUS;


private:
  /** Paddle is enabled if the counter is greater than zero */
  Sint32 enable_counter;
  /** True if the paddle is vertical, otherwise horizontal */
  bool is_vertical;
  /** Paddle number from 0 to 5 */
  Uint32 paddle_number;
  /** Paddle length in pixels */
  Sint32 length;
  /** Minimum with of the paddle 32 or 64 pixels */
  Sint32 width_mini;
  /** Maximum with of the paddle 64 or 128 pixels */
  Sint32 width_maxi;
  /** Shift binary right value (3 or 4) used to convert the length to
   * convert the paddle's length in a interger from 0 to 6 */
  Sint32 shift_width;
  /** Fire state: NOT_OWN_GUN, OWN_GUN, or FIRE */
  Sint32 fire_state;
  /** Stick paddle state: NOT_STICKY_PADDLE, FREE_STICKY_PADDLE, or
   * BUSY_STICKY_PADDLE */
  Uint32 sticky_state;
  const Sint32 **rebonds_Ga;    //ball rebounds table (move on the left)
  const Sint32 **rebonds_Dr;    //ball rebounds table (move on the right)
  /** Current directions used for bounce a ball */
  const Sint32 *current_bounces;
  Sint32 *direct_tab;           // table direction si la balle collee
  /** Current stuck ball, NULL if not */
  sprite_ball *stuck_ball;
  /** If true the paddle touched a ball */
  bool is_hit_ball;
  /** Paddle is invinciblei f  the counter is greater than zero
   * available only in the guadians phase */
  Sint32 invincible_counter;
  /** Counter used to blink the padde when it is invincible */
  Sint32 blink_counter;
  /** Used for fire power 1 or fire power 2 */
  controller_projectiles *projectiles;
  Sint32 bumper_FX0;            // tir inc0. X
  Sint32 bumper_FY0;            // tir inc0. Y
  Sint32 bumper_FX1;            // tir inc1. X
  Sint32 bumper_FY1;            // tir inc1. Y
  Sint32 bump_TFIRE;            // tir 0, 1, 2
  Sint32 bumper_FX2;            // tir inc2. X
  Sint32 bumper_FY2;            // tir inc2. Y
  Sint32 bump_Xscie;            // tir X centre scie
  Sint32 bump_Yscie;            // tir Y centre scie
  Sint32 bump_xdeca;            // decalage tir 6
  Sint32 bump_ydeca;            // decalage tir 6
  Sint32 bump_speed;            // vitesse deplacement (maximum 10)

public:
    sprite_paddle (bool has_projectiles = true);
   ~sprite_paddle ();
  void create_projectiles_list ();
  void fire_projectiles ();
  void move_projectiles ();
  void enable_if_ok (Sint32 rTeam, Sint32 large, Sint32 actif);
  void set_width (Sint32 w);
  void set_height (Sint32 h);
  void select_image (Sint32 l);
  void select_image ();
  Uint32 get_paddle_number ();
  void set_glue ();
  void set_fire_1 ();
  void set_fire_2 ();
  void release_ball ();
  void stick_ball (sprite_ball * ball);
  void directBall ();
  Uint32 get_length ();
  bool is_invincible ();
  void set_invincibility (Sint32 delay);
  void blink ();
};
#endif

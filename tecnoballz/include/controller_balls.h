/** 
 * @file controller_balls.h
 * @brief Control the balls. Move and collisions 
 * @date 2007-09-13
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.23 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_balls.h,v 1.23 2007/09/13 15:51:53 gurumeditation Exp $
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
#ifndef __CONTROLLER_BALLS__
#define __CONTROLLER_BALLS__

class controller_balls;

#include "../include/sprite_ball.h"
#include "../include/objects_list.h"
#include "../include/sprite_paddle.h"
#include "../include/controller_bricks.h"
#include "../include/controller_ships.h"
#include "../include/right_panel_score.h"
#include "../include/controller_guardians.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/controller_paddles.h"
#include "../include/controller_magnetic_eyes.h"
#include "../include/sprite_eye.h"

class controller_balls:public objects_list < sprite_ball, controller_balls >
{
  friend class controller_ejectors;
  friend class sprite_ball;

private:
  Sint32 num_erreur;
  Uint32 glue_delay;            //duration of the glue
  Sint16 *balleVites;           // table premiere vitesse
  Sint32 tempoVites;            // temps avant changement vitesse
  Sint32 startCount;            // Temps avant que la balle ne parte
  Sint32 balle_tilt;            // Temps avant que le tilt soit possible
  /** True if ball controlled by the left mouse button */
  bool balls_are_controlled;
  sprite_paddle *paddle_bottom;
  sprite_paddle *paddle_right;
  sprite_paddle *paddle_top;
  sprite_paddle *paddle_left;
  sprite_paddle *tec_robot0;
  sprite_object *ptBob_wall;

private:
  static Sint32 ball_eject1[];
  static Sint32 ball_eject2[];
  static Sint32 ball_eject3[];
  static Sint32 ball_eject4[];
  static Sint32 rb0[16];
  static Sint32 rb1[16];
  static Sint32 rb2[16];
  static Sint32 rb3[16];
  static Sint32 rb4[16];
  static Sint32 rb5[16];
  static Sint32 rb6[16];
  static Sint32 rb7[16];
  static Sint32 rb8[16];
  static Sint32 *brick_jump[15];

public:
    controller_balls (sprite_object * pwall);
    controller_balls ();
   ~controller_balls ();
  void init (Uint32 start, Uint32 glueC,
                   Uint32 speed, Uint32 tiltC, Uint32 table);
  void run_in_bricks_levels ();
  void run_in_guardians_level ();
  sprite_ball *first_ball ();

private:
  void check_outside_balls ();
  void vitussort2 ();
  void activate_tilt ();
  void vitus_move ();
  void accelerate ();
  void vitusmove2 ();
  void check_collisions_with_paddles ();
  void vitusbump2 ();
  void check_collisions_with_robot ();
  void handle_ejectors ();
  void collision_with_walls ();
  void vituscote2 ();
  void vitusbound ();
  void check_bricks_collision ();
  void check_collisions_with_ships ();
  void check_collisions_with_eyes ();
  void vitusGuard ();
  void controll_balls ();


public:
  void enable_balls_control ();
  void run_2balls ();
  void run_nballs (Uint32 nball = 0);
  void run_3balls ();
  void set_power_1 ();
  void set_power_2 ();
  void set_size_2 ();
  void set_size_3 ();
  void maxi_speed ();
  void time_2tilt ();
  void time2tilt2 ();
  Sint32 least_glue ();

};
#endif

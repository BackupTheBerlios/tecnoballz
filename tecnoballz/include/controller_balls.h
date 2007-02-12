/** 
 * @file controller_balls.h
 * @brief Control the balls. Move and collisions 
 * @date 2007-02-12
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.13 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_balls.h,v 1.13 2007/02/12 16:28:19 gurumeditation Exp $
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
#ifndef __CONTROLLER_BALLS__
#define __CONTROLLER_BALLS__

class controller_balls;

#include "../include/sprite_ball.h"
#include "../include/objects_list.h"
#include "../include/sprite_paddle.h"
#include "../include/controller_ejectors.h"
#include "../include/controller_bricks.h"
#include "../include/controller_sides_bricks.h"
#include "../include/head_animation.h"
#include "../include/controller_ships.h"
#include "../include/right_panel_score.h"
#include "../include/controller_guardians.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/zeMiniMess.h"
#include "../include/controller_paddles.h"
#include "../include/controller_magnetic_eyes.h"
#include "../include/sprite_eye.h"

class controller_balls:public objects_list < sprite_ball >
{
  friend class controller_ejectors;
  friend class sprite_ball;

private:
  Sint32 num_erreur;
  Sint32 balle_glue;            //duration of the glue
  Sint16 *balleVites;           // table premiere vitesse
  Sint32 tempoVites;            // temps avant changement vitesse
  Sint32 startCount;            // Temps avant que la balle ne parte
  Sint32 balle_tilt;            // Temps avant que le tilt soit possible
  /* true if ball controlled by the left mouse button */
  bool balls_are_controlled;
  sprite_paddle *paddle_bottom;
  sprite_paddle *paddle_right;
  sprite_paddle *paddle_top;
  sprite_paddle *paddle_left;
  sprite_paddle *tec_robot0;
  controller_ejectors *ejectObjet;
  controller_bricks *bricks;
  controller_sides_bricks *sides_bricks;
  head_animation *head_anim;
  controller_ships *ptBouiBoui;
  right_panel_score *ptBarreScr;
  controller_guardians *ptguardian;
  controller_moneys *ptCapsules;
  controller_capsules *pt_gadgets;
  sprite_object *ptBob_wall;
  zeMiniMess *ptMiniMess;
  controller_magnetic_eyes *pt_magneye;

private:
  static Sint32 ballEject1[];
  static Sint32 ballEject2[];
  static Sint32 ballEject3[];
  static Sint32 ballEject4[];
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
    controller_balls (controller_ejectors * eject, controller_bricks * brick,
                      controller_sides_bricks * brico, head_animation * gugus,
                      controller_ships * atoms, right_panel_score * score,
                      sprite_object * pwall, zeMiniMess *,
                      controller_magnetic_eyes * pEyes);
    controller_balls (controller_guardians *, controller_moneys *,
                      controller_capsules *);
   ~controller_balls ();
  void init_balle (controller_paddles * raket, Sint32 start, Sint32 glueC,
                   Sint32 speed, Sint32 tiltC, Sint32 table);
  void vitusBalle ();
  void vitusBall2 ();
  sprite_ball *first_ball ();

private:
  void vitus_sort ();
  void vitussort2 ();
  void activate_tilt ();
  void vitus_move ();
  void accelerate ();
  void vitusmove2 ();
  void vitus_bump ();
  void vitusbump2 ();
  void vitusrobot ();
  void vitusEject ();
  void collision_with_walls ();
  void vituscote2 ();
  void vitusbound ();
  void check_bricks_collision ();
  void vitusAtoms ();
  void vitus_eyes ();
  void vitusGuard ();
  void controll_balls ();


public:
  void enable_balls_control ();
  void run_2balls ();
  void run_nballs (Uint32 nball = 0);
  void run_3balls ();
  void run_power1 ();
  void run_power2 ();
  void run_size01 ();
  void run_size02 ();
  void maxi_speed ();
  void time_2tilt ();
  void time2tilt2 ();
  Sint32 least_glue ();

};
#endif

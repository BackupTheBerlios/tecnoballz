/** 
 * @file sprite_ball.h
 * @brief The ball sprite
 * @date 2007-09-29
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.19 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_ball.h,v 1.19 2007/09/29 08:53:47 gurumeditation Exp $
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
#ifndef __SPRITE_BALL__
#define __SPRITE_BALL__
class sprite_ball;
#include "../include/sprite_object.h"

/** Coordinates of the ball on the ejectors */
typedef struct
{
  Sint32 x_coord;
  Sint32 y_coord;
} ball_ejector_coords;


#include "../include/sprite_paddle.h"
#include "../include/controller_bricks.h"
#include "../include/controller_viewfinders.h"
#include "../include/controller_ejectors.h"

class sprite_ball:public sprite_object
{
  friend class controller_balls;
  friend class controller_viewfinders;
  friend class controller_paddles;
  friend class sprite_money;
  friend class sprite_gem;
  friend class controller_guardians;
  friend class direction;

public:
  static const Sint32 BALLNORMAL = 0;
  static const Sint32 BALLPOWER1 = 1;
  static const Sint32 BALLPOWER2 = 2;
  static const Sint32 BALL_SIZE1 = 0;
  static const Sint32 BALL_SIZE2 = 3;
  static const Sint32 BALL_SIZE3 = 6;

  // width and height of collision of the three sizes of ball (low-res)
  static const Sint32 BALLWIDTH1 = 5;
  static const Sint32 BALLWIDTH2 = 7;
  static const Sint32 BALLWIDTH3 = 9;

  // balls moving limits (low-res)
  static const Sint32 MINIMUM_PX = 5;   //left limit
  static const Sint32 MAXIMUM_PX = 250; //right limit
  static const Sint32 MINIMUM_PY = 0;   //top limit
  static const Sint32 MAXIMUM_PY = 230; //bottom limit


private:
  /** Ball direction from 0 to 64 step 4
   * 64 = immobile ball */
  Sint32 direction;
  /** Previous ball direction */
  Sint32 previous_direction;
  /** Delay counter before change ball direction
   * Avoid the horizontal blockings */
  Sint32 change_direction_count;
  /** The last paddle touched by the ball */
  sprite_paddle *paddle_touched;
  /** Padlle on witch is stuck the ball */
  sprite_paddle *stick_paddle;
  /** Paddle number on which the ball is stuck */
  Uint32 sticky_paddle_num;
  /** Pointer to the current velocity table */
  Sint16 *velocity;
  /** Pointer to the current velocity table */
  Sint16 *initial_velocity;
  Sint32 *collisionT;           //pt/bricks collisions table
  Sint32 powerBall1;            //value decreasing bouiboui strength
  Sint32 powerBall2;            //value decreasing brick strength (2 4 or 6) 

  /** Identify in which ejector is the ball */
  /** Time delay befor ejection of the ball */
  Sint32 ejector_delay;
  /** Table of directions possible that a ball can
   * set when it leave a ejector */
  Sint32 *ejector_table;
  /** counter before a tilt is possible */
  Sint32 tilt_delay_counter;
  /** Counter delay before accelerating the ball */
  Sint32 accelerate_delay_counter;
  /** Time delay before accelerating the ball */
  Sint32 accelerate_delay;
  /** Delay counter before the ball leaves the paddle */
  Sint32 start_delay_counter;
  Sint32 start_init;            //temps avant que la balle parte (valeur initiale)
  Sint32 balle_rota;            //pointeur sur table ballePets 
  Sint32 tempo_rota;            //Tempo rotation du point autour de la balle
  Sint32 ball_sizeX;            //Pointeur Gfx 
  Sint32 ballPowerX;            //Pointeur Gfx
  Sint32 oeilRotate;            //1=rotation autour de l'oeil  
  /** Brick's width in pixels 16 or 32 */
  Sint32 brick_width;
  Sint32 colli_wall;            //collision with one wall

  /** True if collision point of ball with bricks were corrected */
  static bool is_collisions_point_initialized;
  static Sint32 brikPoint1[8];  //Points collision balle taille 1
  static Sint32 brikPoint2[8];  //Points collision balle taille 2
  static Sint32 brikPoint3[8];  //Points collision balle taille 3
  static Sint16 ballSpeed1[];   //Table deplacement balle vitesse 1
  static Sint16 ballSpeed2[];   //Table deplacement balle vitesse 2
  static Sint16 ballSpeed3[];   //Table deplacement balle vitesse 3
  static Sint16 ballSpeed4[];   //Table deplacement balle vitesse 4
  /** Coordinates of the balls on the ejectors */
  static ball_ejector_coords ejector_coords[];
  static const Sint32 tilt_table[16][16];
  static Sint32 ball_eject1[];
  static Sint32 ball_eject2[];
  static Sint32 ball_eject3[];
  static Sint32 ball_eject4[];
  static Sint32* ball_ejectors[];

public:
    sprite_ball ();
   ~sprite_ball ();
  static void init_collisions_points ();
  void once_init (Sint32 start, Sint32 speed,
                   sprite_paddle * paddle, Sint16 * table, Sint32 w);
  void starts_again (sprite_paddle * paddle);
  void remove (sprite_paddle * paddle);
  void init_first_ball (Sint32 h);
  void duplicate_from (sprite_ball * ball, Uint32 angle);
  static Sint16 *get_speed_table (Sint32 speed);
  sprite_paddle *get_last_paddle_touched ();
  void set_power_1 ();
  void set_power_2 ();
  void set_size_2 ();
  void set_size_3 ();
  void set_maximum_speed ();
  void enbale_on_ejector (Uint32 eject_id, Uint32 otime);
  void set_on_ejector (Uint32 eject_id, Uint32 otime = 1);
  void disable_stick ();
  void accelerate ();

private:
  void set_initial_values (sprite_paddle * paddle);
  void select_image ();
};
#endif

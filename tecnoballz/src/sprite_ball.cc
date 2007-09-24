/** 
 * @file sprite_ball.cc 
 * @brief The ball sprite
 * @date 2007-09-20
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.16 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_ball.cc,v 1.16 2007/09/24 16:00:01 gurumeditation Exp $
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
#include "../include/sprite_ball.h"

/** 
 * Create the ball sprite
 */
sprite_ball::sprite_ball ()
{
  clear_sprite_members ();
  once_init (0, 0, (sprite_paddle *) NULL, ballSpeed1, 0);
}

/**
 * Release the ball sprite
 */
sprite_ball::~sprite_ball ()
{
}

/**
 * perform some initializations
 * @param start Time before the ball leaves the bumper (first time)
 * @param speed Time before the ball accelerates
 * @param paddle Default paddle
 * @param table Speed of the ball from 1 to 4
 * @param w Width of a brick in pixels 
*/
void
sprite_ball::once_init (Sint32 start, Sint32 speed,
                         sprite_paddle * paddle, Sint16 * table, Sint32 w)
{
  start_init = start;
  accelerate_delay = speed;
  initial_velocity = table;
  disable ();
  sticky_paddle_num = 0;
  startCount = 0;
  brick_width = w;
  set_initial_values (paddle);
}

/**
 * Restarts the ball 
 * @param paddle pointer to a paddle sprite
 */
void
sprite_ball::starts_again (sprite_paddle * paddle)
{
  enable ();
  startCount = start_init;
  sticky_paddle_num = paddle->get_paddle_number ();
  set_initial_values (paddle);
  select_image ();
}

/**
 * Remove the ball
 */
void
sprite_ball::remove (sprite_paddle * paddle)
{
  disable ();
  sticky_paddle_num = 0;
  startCount = 0;
  set_initial_values (paddle);
  select_image ();
}

/**
 * Set initial values of the ball
 * @param paddle pointer to a paddle sprite
 */
void
sprite_ball::set_initial_values (sprite_paddle * paddle)
{
  accelerate_delay_counter = accelerate_delay;
  collision_width = BALLWIDTH1 * resolution;
  collision_height = BALLWIDTH1 * resolution;
  direction = 0;
  previous_direction = direction;
  change_direction_count = 0;
  paddle_touched = paddle;
  stick_paddle = paddle;
  velocity = initial_velocity;
  collisionT = brikPoint1;
  powerBall1 = 1;
  powerBall2 = brick_width;
  eject_ball[0] = 0;
  eject_ball[1] = 0;
  eject_ball[2] = 0;
  eject_ball[3] = 0;
  tilt_delay = 0;
  balle_rota = 1;
  tempo_rota = 1;
  ball_sizeX = BALL_SIZE1;
  ballPowerX = BALLNORMAL;
  oeilRotate = 0;
  colli_wall = 0;
}

/**
 * Initialize a first ball in a level sticked on the paddle
 * @param w Width of the paddle
 */
void
sprite_ball::init_first_ball (Sint32 w)
{
  enable ();
  sprite_paddle *paddle = stick_paddle;
  starts_again (paddle);
  x_coord =
    paddle->get_x_coord () + ((w >> 1) - ((collision_width >> 1) + 1));
  y_coord = paddle->get_y_coord () - collision_height - 1;
}

/**
 * Disable sticked ball
 */
void
sprite_ball::disable_stick ()
{
  sticky_paddle_num = 0;
  startCount = 0;
}

/**
 * Return speed ball
 * @param speed Speed of the ball from 1 to 4
 * @return Pointer to speed ball table
 */
Sint16 *
sprite_ball::get_speed_table (Sint32 speed)
{
  switch (speed)
    {
    case 1:
      return ballSpeed1;
    case 2:
      return ballSpeed2;
    case 3:
      return ballSpeed3;
    case 4:
      return ballSpeed4;
    }
  return ballSpeed1;
}

/**
 * Return the last touched paddle
 * @return a pointer to a paddle sprite
 */
sprite_paddle *
sprite_ball::get_last_paddle_touched ()
{
  return paddle_touched;
}

/**
 * Duplicate this ball from another ball
 * @param ball ball template
 * @param angle angle from 0 to 63
 */
void
sprite_ball::duplicate_from (sprite_ball * ball, Uint32 angle)
{
  is_enabled = true;
  x_coord = ball->x_coord;
  y_coord = ball->y_coord;
  direction = angle;
  sticky_paddle_num = 0;
  tilt_delay = 0;
  ball_sizeX = ball->ball_sizeX;
  ballPowerX = ball->ballPowerX;
  collision_width = ball->collision_width;
  collision_height = ball->collision_height;
  paddle_touched = ball->paddle_touched;
  stick_paddle = ball->stick_paddle;
  velocity = ball->velocity;
  collisionT = ball->collisionT;
  powerBall1 = ball->powerBall1;
  powerBall2 = ball->powerBall2;
  oeilRotate = 0;
  select_image ();
}

/**
 * Enable the ball power 1
 */
void
sprite_ball::set_power_1 ()
{
  ballPowerX = BALLPOWER1;
  select_image ();
}

/**
 * Enable the ball power 2
 */
void
sprite_ball::set_power_2 ()
{
  ballPowerX = BALLPOWER2;
  select_image ();
}

/**
 * Enable the ball size 
 */
void
sprite_ball::set_size_2 ()
{
  powerBall1 = 2;
  powerBall2 = brick_width * 2;
  collision_width = BALLWIDTH2 * resolution;
  collision_height = BALLWIDTH2 * resolution;
  ball_sizeX = BALL_SIZE2;
  collisionT = brikPoint2;
  select_image ();
}

/**
 * Enable the ball size 3
 */
void
sprite_ball::set_size_3 ()
{
  powerBall1 = 3;
  powerBall2 = brick_width * 3;
  collision_width = BALLWIDTH3 * resolution;
  collision_height = BALLWIDTH3 * resolution;
  ball_sizeX = BALL_SIZE3;
  collisionT = brikPoint3;
  select_image ();
}

/**
 * Set the fastest speed
 */
void
sprite_ball::set_maximum_speed ()
{
  velocity = ballSpeed3;
}

/**
 * Enable a ball on a ejector
 * @param ejector_id ejector identifier from 0 to 3
 * @param delay before the ejection of the ball
 */
void
sprite_ball::enbale_on_ejector (Uint32 ejector_id, Uint32 delay)
{
  is_enabled = true;
  ejector_id &= 3;
  furaxEject *monPT = furaxTable + ejector_id;
  if (monPT->ejectBall1)
    {
      eject_ball[0] = delay;
    }
  else
    {
      eject_ball[0] = 0;
    }
  if (monPT->ejectBall2)
    {
      eject_ball[1] = delay;
    }
  else
    {
      eject_ball[1] = 0;
    }
  if (monPT->ejectBall3)
    {
      eject_ball[2] = delay;
    }
  else
    {
      eject_ball[2] = 0;
    }
  if (monPT->ejectBall4)
    {
      eject_ball[3] = delay;
    }
  else
    {
      eject_ball[3] = 0;
    }
  x_coord = monPT->x_coord;
  y_coord = monPT->y_coord;
  /* the ball's motionless */
  direction = 64;
}

/** 
 * Select the ball image
 */
void
sprite_ball::select_image ()
{
  frame_index = ball_sizeX + ballPowerX;
  set_image (frame_index);
}

/**
 * Accelerate the ball
 */
void
sprite_ball::accelerate ()
{
  if (--accelerate_delay_counter > 1)
    {
      return;
    }
  accelerate_delay_counter = accelerate_delay;
  if (velocity == ballSpeed1)
    {
      velocity = ballSpeed2;
      return;
    }
  if (velocity == ballSpeed2)
    {
      velocity = ballSpeed3;
      return;
    }
  if (velocity == ballSpeed3)
    {
      velocity = ballSpeed4;
    }
}

//-------------------------------------------------------------------------------
// Liste des points de collision de la balle (avec les briques) ================
//-------------------------------------------------------------------------------
Sint32
  sprite_ball::brikPoint1[8] = { 10, 6, //XMAXIM,MILIEU (balle1)
  6, 0,                         //MILIEU,YMINIM
  0, 6,                         //XMINIM,MILIEU
  6, 10                         //MILIEU,YMAXIM
};
Sint32
  sprite_ball::brikPoint2[8] = { 14, 8, // XMAXIM,MILIEU (balle2)
  8, 0,                         // MILIEU,YMINIM
  0, 8,                         // XMINIM,MILIEU
  8, 14                         // MILIEU,YMAXIM
};
Sint32
  sprite_ball::brikPoint3[8] = { 18, 10,        // XMAXIM,MILIEU (balle3)
  10, 0,                        // MILIEU,YMINIM
  0, 10,                        // XMINIM,MILIEU
  10, 18                        // MILIEU,YMAXIM
};

// Table des differentes vitesses de balle
//              16
//            20/\ 12
//          24  ||   08
//        28    ||     04
//      32<=====64=====> 00
//        36    ||    60
//          40  ||  56
//            44\/52 
//              48
//==============================================================================

Sint16
  sprite_ball::ballSpeed1[] = { +2, 0,  //00
  +2, -1,                       //04
  +2, -2,                       //08
  +1, -2,                       //12
  0, -2,                        //16
  -1, -2,                       //20
  -2, -2,                       //24
  -2, -1,                       //28
  -2, 0,                        //32
  -2, +1,                       //36
  -2, +2,                       //40
  -1, +2,                       //44
  0, +2,                        //48
  +1, +2,                       //52
  +2, +2,                       //56
  +2, +1,                       //60
  0, 0                          //64
};
Sint16
  sprite_ball::ballSpeed2[] =
  { 3, 0, 3, -1, 2, -2, 1, -3, 0, -3, -1, -3, -2, -2, -3, -1, -3,
  0, -3, 1, -2, 2, -1, 3, 0, 3, 1, 3, 2, 2, 3, 1, 0, 0
};

Sint16
  sprite_ball::ballSpeed3[] =
  { 4, 0, 4, -1, 3, -3, 1, -4, 0, -4, -1, -4, -3, -3, -4, -1, -4,
  0, -4, 1, -3, 3, -1, 4, 0, 4, 1, 4, 3, 3, 4, 1, 0, 0
};

Sint16
  sprite_ball::ballSpeed4[] =
  { 5, 0, 5, -2, 4, -4, 2, -5, 0, -5, -2, -5, -4, -4, -5, -2,
  -5, 0, -5, 2, -4, 4, -2, 5, 0, 5, 2, 5, 4, 4, 5, 2, 0, 0
};

furaxEject
sprite_ball::furaxTable[] = { {1, 0, 0, 0, 8, 8}
,                               //1 top-left
{0, 1, 0, 0, 8, 3}
,                               //3 bottom-left
{0, 0, 1, 0, 3, 3}
,                               //4 bottom-right
{0, 0, 0, 1, 3, 8}              //2 top-right
};

const Sint32
sprite_ball::tilt_table[16][16] = { {4, 4, 8, 12, 16, 20, 24, 28, 28, 36, 40, 44, 48, 52, 56, 60},      //0     32
{8, 8, 8, 12, 16, 20, 24, 28, 28, 40, 40, 44, 48, 52, 56, 60},  //4     36
{4, 4, 12, 12, 16, 20, 24, 28, 28, 36, 44, 44, 48, 52, 56, 60}, //8     40
{4, 4, 8, 16, 16, 20, 24, 28, 28, 36, 40, 48, 48, 52, 56, 60},  //12    44
{4, 4, 8, 12, 20, 20, 24, 28, 28, 36, 40, 44, 52, 52, 56, 60},  //16    48
{4, 4, 8, 12, 16, 24, 24, 28, 28, 36, 40, 44, 48, 56, 56, 60},  //20    52
{4, 4, 8, 12, 16, 20, 20, 28, 28, 36, 40, 44, 48, 52, 60, 60},  //24    56
{4, 4, 8, 12, 16, 20, 24, 24, 36, 36, 40, 44, 48, 52, 56, 56},  //28    60
{4, 4, 8, 12, 16, 20, 24, 28, 28, 36, 40, 44, 48, 52, 56, 60},  //32    0
{8, 8, 8, 12, 16, 20, 24, 28, 28, 40, 40, 44, 48, 52, 56, 60},  //36    4
{4, 4, 12, 12, 16, 20, 24, 28, 28, 36, 44, 44, 48, 52, 56, 60}, //40    8
{4, 4, 8, 16, 16, 20, 24, 28, 28, 36, 40, 40, 48, 52, 56, 60},  //44    12
{4, 4, 8, 12, 12, 20, 24, 28, 28, 36, 40, 44, 44, 52, 56, 60},  //48    16
{4, 4, 8, 12, 16, 24, 24, 28, 28, 36, 40, 44, 48, 56, 56, 60},  //52    20
{4, 4, 8, 12, 16, 20, 20, 28, 28, 36, 40, 44, 48, 52, 52, 60},  //56    24
{4, 4, 8, 12, 16, 20, 24, 24, 36, 36, 40, 44, 48, 52, 56, 56}   //60    28
};

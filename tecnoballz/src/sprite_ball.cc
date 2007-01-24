/** 
 * @file sprite_ball.cc 
 * @brief The ball sprite
 * @date 2007-01-20
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_ball.cc,v 1.6 2007/01/24 20:48:22 gurumeditation Exp $
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
#include "../include/sprite_ball.h"

/** 
 * Create the ball sprite
 */
sprite_ball::sprite_ball ()
{
  clear_sprite_members ();
  littleInit (0, 0, (sprite_paddle *) NULL, ballSpeed1, 0);
}

/**
 * Release the ball sprite
 */
sprite_ball::~sprite_ball ()
{
}

/**
 * perform some initializations
 * @param start time before the ball leaves the bumper (first time)
 * @param speed time before the ball accelerates
 * @param raket default bumper
 * @param table speed ball (1 to 4)
 * @param bwgth width of brick in pixels 
*/
void
sprite_ball::littleInit (Sint32 start, Sint32 speed,
                         sprite_paddle * raket, Sint16 * table, Sint32 bwght)
{
  start_init = start;
  speed_init = speed;
  speedBallZ = table;
  disable ();
  colleBallF = 0;
  startCount = 0;
  brickWidth = bwght;
  razingBal0 (raket);
}

//-----------------------------------------------------------------------------
// initialize again
//-----------------------------------------------------------------------------
void
sprite_ball::reStarting (sprite_paddle * raket)
{
  enable ();
  startCount = start_init;
  colleBallF = raket->bumper_num ();
  razingBall (raket);
}

//-----------------------------------------------------------------------------
// disable the ball
//-----------------------------------------------------------------------------
void
sprite_ball::goSleeping (sprite_paddle * raket)
{
  disable ();
  colleBallF = 0;
  startCount = 0;
  razingBall (raket);
}

//-----------------------------------------------------------------------------
// reset some members
//-----------------------------------------------------------------------------
void
sprite_ball::razingBall (sprite_paddle * raket)
{
  razingBal0 (raket);
  pointe_GFX ();
}

//-----------------------------------------------------------------------------
// reset some members
//-----------------------------------------------------------------------------
void
sprite_ball::razingBal0 (sprite_paddle * raket)
{
  speedCount = speed_init;
  collision_width = BALLWIDTH1 * resolution;
  collision_height = BALLWIDTH1 * resolution;
  directBall = 0;
  save_Dball = 0;
  countDball = 0;
  raket_ball = raket;
  raket_glue = raket;
  speedBallT = speedBallZ;
  collisionT = brikPoint1;
  powerBall1 = 1;
  powerBall2 = brickWidth;
  eject_ball[0] = 0;
  eject_ball[1] = 0;
  eject_ball[2] = 0;
  eject_ball[3] = 0;
  tiltCompte = 0;
  ClBouiBoui = 0;
  balle_rota = 1;
  tempo_rota = 1;
  ball_sizeX = BALL_SIZE1;
  ballPowerX = BALLNORMAL;
  oeilRotate = 0;
  colli_wall = 0;
}

//-----------------------------------------------------------------------------
// initialize a simple ball (sticked on the bumper)
//-----------------------------------------------------------------------------
void
sprite_ball::startBalle (Sint32 large)
{
  enable ();
  sprite_paddle *raket = raket_glue;
  reStarting (raket);
  x_coord =
    raket->get_x_coord () + ((large >> 1) - ((collision_width >> 1) + 1));
  y_coord = raket->get_y_coord () - collision_height - 1;
}

//-------------------------------------------------------------------------------
// disbable sticked ball
//-------------------------------------------------------------------------------
void
sprite_ball::glueLibere ()
{
  colleBallF = 0;
  startCount = 0;
}

//-------------------------------------------------------------------------------
// get speed ball table
//-------------------------------------------------------------------------------
Sint16 *
sprite_ball::donneSpeed (Sint32 speed)
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

//-------------------------------------------------------------------------------
// return the last touched bumper
//-------------------------------------------------------------------------------
sprite_paddle *
sprite_ball::donne_bump ()
{
  return (raket_ball);
}

//-------------------------------------------------------------------------------
// clone ball
//-------------------------------------------------------------------------------
void
sprite_ball::duplicate3 (sprite_ball * balle, Sint32 angle)
{
  is_enabled = 1;
  x_coord = balle->x_coord;
  y_coord = balle->y_coord;
  directBall = angle;
  colleBallF = 0;
  tiltCompte = 0;
  ball_sizeX = balle->ball_sizeX;
  ballPowerX = balle->ballPowerX;
  collision_width = balle->collision_width;
  collision_height = balle->collision_height;
  raket_ball = balle->raket_ball;
  raket_glue = balle->raket_glue;
  speedBallT = balle->speedBallT;
  collisionT = balle->collisionT;
  powerBall1 = balle->powerBall1;
  powerBall2 = balle->powerBall2;
  oeilRotate = 0;
  pointe_GFX ();
}

//-------------------------------------------------------------------------------
// enable the ball power 1
//-------------------------------------------------------------------------------
void
sprite_ball::ballPower1 ()
{
  ballPowerX = BALLPOWER1;
  pointe_GFX ();
}

//-------------------------------------------------------------------------------
// enable the ball power 2
//-------------------------------------------------------------------------------
void
sprite_ball::ballPower2 ()
{
  ballPowerX = BALLPOWER2;
  pointe_GFX ();
}

//-------------------------------------------------------------------------------
// enable the ball size 2
//-------------------------------------------------------------------------------
void
sprite_ball::ball_size2 ()
{
  powerBall1 = 2;
  powerBall2 = brickWidth * 2;
  collision_width = BALLWIDTH2 * resolution;
  collision_height = BALLWIDTH2 * resolution;
  ball_sizeX = BALL_SIZE2;
  collisionT = brikPoint2;
  pointe_GFX ();
}

//-------------------------------------------------------------------------------
// enable the ball size 3
//-------------------------------------------------------------------------------
void
sprite_ball::ball_size3 ()
{
  powerBall1 = 3;
  powerBall2 = brickWidth * 3;
  collision_width = BALLWIDTH3 * resolution;
  collision_height = BALLWIDTH3 * resolution;
  ball_sizeX = BALL_SIZE3;
  collisionT = brikPoint3;
  pointe_GFX ();
}

//-------------------------------------------------------------------------------
// set the fastest speed 
//-------------------------------------------------------------------------------
void
sprite_ball::very_speed ()
{
  speedBallT = ballSpeed3;
}

//-------------------------------------------------------------------------------
// bricks levels: enable the ball in a ejector 
//-------------------------------------------------------------------------------
void
sprite_ball::ball2eject (Sint32 index, Sint32 otime)
{
  is_enabled = 1;
  index &= 3;
  furaxEject *monPT = furaxTable + index;
  if (monPT->ejectBall1)
    eject_ball[0] = otime;
  else
    eject_ball[0] = 0;
  if (monPT->ejectBall2)
    eject_ball[1] = otime;
  else
    eject_ball[1] = 0;
  if (monPT->ejectBall3)
    eject_ball[2] = otime;
  else
    eject_ball[2] = 0;
  if (monPT->ejectBall4)
    eject_ball[3] = otime;
  else
    eject_ball[3] = 0;
  x_coord = monPT->x_coord;
  y_coord = monPT->y_coord;
  directBall = 64;              //the ball's motionless 
}

//-------------------------------------------------------------------------------
// initialize ball image
//-------------------------------------------------------------------------------
void
sprite_ball::pointe_GFX ()
{
  frame_index = ball_sizeX + ballPowerX;
  set_image (frame_index);
}

//-------------------------------------------------------------------------------
// accelerate the ball
//-------------------------------------------------------------------------------
void
sprite_ball::accelerate ()
{
  if (--speedCount > 1)
    return;
  speedCount = speed_init;
  if (speedBallT == ballSpeed1)
    {
      speedBallT = ballSpeed2;
      //printf("sprite_ball::accelerate() : ballSpeed2\n");
      return;
    }
  if (speedBallT == ballSpeed2)
    {
      speedBallT = ballSpeed3;
      //printf("sprite_ball::accelerate() : ballSpeed3\n");
      return;
    }
  if (speedBallT == ballSpeed3)
    {
      speedBallT = ballSpeed4;
      //printf("sprite_ball::accelerate() : ballSpeed4\n");
      return;
    }
  //printf("sprite_ball::accelerate() : maxi speed\n");
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

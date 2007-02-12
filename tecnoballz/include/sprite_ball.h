/** 
 * @file sprite_ball.h
 * @brief The ball sprite
 * @date 2007-01-26
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.11 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_ball.h,v 1.11 2007/02/12 16:28:19 gurumeditation Exp $
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
#ifndef __SPRITE_BALL__
#define __SPRITE_BALL__
class sprite_ball;
#include "../include/sprite_object.h"
//-------------------------------------------------------------------------------
// structure of ball start coordinates (ejectors)
//-------------------------------------------------------------------------------
typedef struct
{
  Sint32 ejectBall1;
  Sint32 ejectBall2;
  Sint32 ejectBall3;
  Sint32 ejectBall4;
  Sint32 x_coord;
  Sint32 y_coord;
} furaxEject;


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
  friend class directBall;

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
  Sint32 directBall;          //ball direction (0 to 56 step 4)
  Sint32 save_Dball;            //last directBall (0 to 56 step 4)
  Sint32 countDball;            //counter before rebound forcing
  sprite_paddle *raket_ball;    //pointer to the bumper touched
  sprite_paddle *raket_glue;    //pointer to the bumper sticked
  Sint16 *speedBallT;           //pt/speed table
  Sint16 *speedBallZ;           //pt/speed table
  Sint32 *collisionT;           //pt/bricks collisions table
  Sint32 powerBall1;            //value decreasing bouiboui strength
  Sint32 powerBall2;            //value decreasing brick strength (2 4 or 6) 
  Sint32 eject_ball[4];         //flag pour coins haut-gauche/bas-gauche/bas_droite/haut-droite
  Sint32 colleBallF;            //sticky ball flag
  Sint32 tilt_delay;            //counter before a tilt is possible 
  Sint32 ClBouiBoui;            //0=no bouibouis collisions
  Sint32 speedCount;            //temps que dure la vitesse
  Sint32 speed_init;            //temps que dure la vitesse (valeur initiale)
  Sint32 startCount;            //temps avant que la balle parte
  Sint32 start_init;            //temps avant que la balle parte (valeur initiale)
  Sint32 balle_rota;            //pointeur sur table ballePets 
  Sint32 tempo_rota;            //Tempo rotation du point autour de la balle
  Sint32 ball_sizeX;            //Pointeur Gfx 
  Sint32 ballPowerX;            //Pointeur Gfx
  Sint32 oeilRotate;            //1=rotation autour de l'oeil  
  /** Brick's width in pixels 16 or 32 */
  Sint32 brick_width;
  Sint32 colli_wall;            //collision with one wall

  static Sint32 brikPoint1[8];  //Points collision balle taille 1
  static Sint32 brikPoint2[8];  //Points collision balle taille 2
  static Sint32 brikPoint3[8];  //Points collision balle taille 3
  static Sint16 ballSpeed1[];   //Table deplacement balle vitesse 1
  static Sint16 ballSpeed2[];   //Table deplacement balle vitesse 2
  static Sint16 ballSpeed3[];   //Table deplacement balle vitesse 3
  static Sint16 ballSpeed4[];   //Table deplacement balle vitesse 4
  static furaxEject furaxTable[];       //Table d'apparition des balles dans les ejecteurs
  static const Sint32 tilt_table[16][16];

public:
    sprite_ball ();
   ~sprite_ball ();
  void once_init (Sint32 start, Sint32 speed,
                   sprite_paddle * raket, Sint16 * table, Sint32 bwght);
  void reStarting (sprite_paddle * raket);
  void goSleeping (sprite_paddle * raket);
  void startBalle (Sint32 large);
  //void                          razingBall(sprite_paddle *raket);
  void duplicate3 (sprite_ball * balle, Sint32 angle);
  static short *donneSpeed (Sint32 speed);
  sprite_paddle *donne_bump ();
  void ballPower1 ();
  void ballPower2 ();
  void ball_size2 ();
  void ball_size3 ();
  void very_speed ();
  void ball2eject (Sint32 index, Sint32 otime = 1);
  void glueLibere ();
  void accelerate ();

private:
  void set_initial_values (sprite_paddle * paddle);
  void select_image ();
};
#endif

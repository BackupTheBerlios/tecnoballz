/** 
 * @file sprite_paddle.h
 * @brief A paddle sprite 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_paddle.h,v 1.7 2007/02/05 21:02:11 gurumeditation Exp $
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
#ifndef __SPRITE_PADDLE__
#define __SPRITE_PADDLE__
class sprite_paddle;
#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/zeFireBump.h"
#include "../include/bitmap_data.h"
#include "../include/tecno_miss.h"
#include "../include/zeGigaBlit.h"
class sprite_paddle:public sprite_object
{
  friend class controller_paddles;
  friend class controller_balls;
  friend class zeFireBump;
  friend class tecno_miss;
  friend class ballDirect;
  friend class zeGigaBlit;

private:
  Sint32 bump_actif;          // flag : raquette active
  Sint32 bumperType;            // flag : type 0=horizontal / 1=vertical                
  /** Paddle number from 0 to 5 */
  Uint32 paddle_number;
  Sint32 length;            //bumper's width (in pixels)
  Sint32 width_mini;            //bumper mini width (32 or 64)
  Sint32 width_maxi;            //bumper maxi width (64 or 128)
  Sint32 width_deca;            //3 or 4; offset between 2 size (2^3)

  Sint32 bumperNorm;            // flag : raquette normale
  Sint32 bumperFire;            // flag : raquette tireuse
  Sint32 is_glue;            // flag : 1=raquette colle libre

  const Sint32 **rebonds_Ga;    //ball rebounds table (move on the left)
  const Sint32 **rebonds_Dr;    //ball rebounds table (move on the right)
  const Sint32 *rebonds_GD;     //ball rebounds table (left and right)

  Sint32 *direct_tab;           // table direction si la balle collee

  sprite_ball *ball_glued;      // ptr  : l'objet balle collee
  Sint32 balleTouch;            // flag : balle touche raquette
  Sint32 invincible;            // > 0 bumper is invincible (guards phase)
  Sint32 flickerval;            // flag flicker

  // weapons's bumper
  zeFireBump *bumperTirs;       // ptr  : objet tir du Bumper
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
    sprite_paddle (zeFireBump * fBump);
   ~sprite_paddle ();
  void create_projectiles_list ();
  void fire_projectiles ();
  void deplaceTir ();
  void bumpActive (Sint32 rTeam, Sint32 large, Sint32 actif);
  void set_width (Sint32 w);
  void set_height (Sint32 h);
  void select_image (Sint32 l);
  void select_image ();
  Uint32 get_paddle_number ();
  void set_glue ();
  void set_fire_1 ();
  void set_fire_2 ();
  void lacheBalle ();
  void attachBall (sprite_ball * xBall);
  void directBall ();
  Sint32 get_length ();
  bool is_invincible ();
  void set_invincibility (Sint32 delay);
  void flickerRun ();
};
#endif

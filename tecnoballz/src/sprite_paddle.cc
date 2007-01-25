/** 
 * @file sprite_paddle.cc 
 * @brief A paddle sprite 
 * @date 2007-01-24
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_paddle.cc,v 1.3 2007/01/25 20:33:51 gurumeditation Exp $
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
#include "../include/sprite_paddle.h"

/*
 * Create a paddle sprite object
 */
sprite_paddle::sprite_paddle (zeFireBump * fBump)
{
  clear_sprite_members ();
  length = 32 * resolution;
  bumperType = 0;
  bumperNorm = 2;
  bumperFire = 0;
  is_glue = 0;
  bump_speed = 0;
  bump_actif = 0;
  balleTouch = 0;
  bumpNumero = 0;
  ball_glued = (sprite_ball *) NULL;
  bumperTirs = fBump;
  invincible = 0;
  flickerval = 0;
  width_mini = 0;
  width_maxi = 0;
  width_deca = 0;
  if (resolution == 1)
    width_deca = 3;
  else
    width_deca = 4;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
sprite_paddle::~sprite_paddle ()
{
  if (bumperTirs)               //bumper has fires ? (robot has not fires)
    bumperTirs->release_sprites_list ();        //release fires
}

//------------------------------------------------------------------------------
// initialize bumper's fires
//------------------------------------------------------------------------------
Sint32
sprite_paddle::init_fires ()
{
  Sint32 error = bumperTirs->installBOB (this);
  return (error);
}

/**
 * Paddle fires projectiles
 */
void
sprite_paddle::fire_projectiles ()
{
  if (bumperFire != 0)
    {
      bumperTirs->disponible ();  //zeFireBump::disponible();
    }
}

//------------------------------------------------------------------------------
// move bumper's fires
//------------------------------------------------------------------------------
void
sprite_paddle::deplaceTir ()
{
  bumperTirs->nouveauTir ();    //zeFireBump::nouveauTir();
  bumperTirs->deplaceTir ();    //zeFireBump::deplaceTir();
}

//------------------------------------------------------------------------------
// determine if the bumper must to be activated
// input => rTeam: 1 = mode team
//       => large: bumper's width (8,16,24,32,40,48,56 or 64)
//       => actif: if >0 bumper's active
//------------------------------------------------------------------------------
void
sprite_paddle::bumpActive (Sint32 rTeam, Sint32 large, Sint32 actif)
{
  length = large;
  is_enabled = 0;
  bump_actif = actif;
  if (bump_actif > 0)
    {
      bump_actif--;
      is_enabled = 1;
    }
  if (bumpNumero == 1 || (bumpNumero == 4 && rTeam == 1))
    {
      bump_actif = 1;
      is_enabled = 1;
    }
  select_image (large);
}

/**
 * Set the width and image of a horizontal paddle
 * @param w width of the paddle in pixels
 */
void
sprite_paddle::set_width (Sint32 w)
{
  length = w;
  collision_width = length;
  select_image (length);
  bumperTirs->disable_sprites ();
}

/**
 * Set the height and image of a vertical paddle
 * @param h height of the paddle in pixels
 */
void
sprite_paddle::set_height (Sint32 h)
{
  length = h;
  collision_height = length;
  select_image (length);
  bumperTirs->disable_sprites ();
}

/**
 * select the sprite image of the paddle
 * @param l length of the paddle
 */
void
sprite_paddle::select_image (Sint32 l)
{
  Sint32 i = (l >> width_deca) - 2;
  if (bumperFire)
    {
      i += 7;
    }
  if (is_glue)
    {
      i += 14;
    }
  set_image (i);
}

/**
 * select the sprite image of the paddle
 */
void
sprite_paddle::select_image ()
{
  select_image (length);
}

//------------------------------------------------------------------------------
// return the number of the bumper
// output       <= bumpNumero: 1 (bottom), 2 (right), 3 (top), 4 (left) or 5 (robot)
//------------------------------------------------------------------------------
Sint32
sprite_paddle::bumper_num ()
{
  return bumpNumero;
}

//------------------------------------------------------------------------------
// bricks levels: transform into bumper glue 
//------------------------------------------------------------------------------
void
sprite_paddle::bumpGoGlue ()
{
  is_glue = 1;
  select_image ();
}

//------------------------------------------------------------------------------
// bricks levels: transform into bumper fire 1 
//------------------------------------------------------------------------------
void
sprite_paddle::bump_fire1 ()
{
  bumperFire = 1;
  select_image ();
  bumperTirs->fire1RunOn ();
}

//------------------------------------------------------------------------------
// bricks levels: transform into bumper fire 2
//------------------------------------------------------------------------------
void
sprite_paddle::bump_fire2 ()
{
  bumperFire = 1;
  select_image ();
  bumperTirs->fire2RunOn ();
}

//------------------------------------------------------------------------------
// bricks levels: release the ball (if a ball's glued)
//------------------------------------------------------------------------------
void
sprite_paddle::lacheBalle ()
{
  if (is_glue > 1)           //is the ball glued on bumper?
    is_glue = 1;             //bumper's free 
  sprite_ball *balle = ball_glued;
  if (balle)
    {
      ball_glued = (sprite_ball *) NULL;
      balle->glueLibere ();
    }
}

/*
 * Glue a ball to the paddle into the bricks level
 * @param ball pointer to sprite_ball object
 */
void
sprite_paddle::attachBall (sprite_ball * ball)
{
  if (ball_glued != NULL)
    {
      ball_glued->glueLibere ();
    }
  ball_glued = ball;
  if (is_glue)
    {
      is_glue = 2;
    }
}


/**
 * Return paddle's length
 * @return length of the paddle in pixels
 */
Sint32
sprite_paddle::get_length ()
{
  return length;
}

/**
 * Check if the paddle is invincible (into guards levels)
 * @return true the paddle is invincible, false otherwise
 */
bool
sprite_paddle::is_invincible ()
{
  return invincible > 0 ? true : false;
}

/**
 * Set the temporary invincibility of the paddle
 * @param delay time delay of the invincibility
 */
void
sprite_paddle::set_invincibility (Sint32 delay)
{
  invincible = delay;
}

//------------------------------------------------------------------------------
// guards levels: handle invincible bumper
//------------------------------------------------------------------------------
void
sprite_paddle::flickerRun ()
{
  if (invincible > 0)
    {
      invincible--;
      if (flickerval > 0)
        {
          is_enabled = 0;
          flickerval = 0;
        }
      else
        {
          is_enabled = 1;
          flickerval = 1;
        }
    }
  else
    is_enabled = 1;
}

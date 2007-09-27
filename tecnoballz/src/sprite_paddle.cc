/** 
 * @file sprite_paddle.cc 
 * @brief A paddle sprite 
 * @date 2007-09-27
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.13 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_paddle.cc,v 1.13 2007/09/27 10:51:33 gurumeditation Exp $
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
#include "../include/sprite_paddle.h"

/**
 * Create a paddle sprite object
 * @param has_projectiles true if the paddle can fire
 */
//sprite_paddle::sprite_paddle (controller_projectiles * fBump)
sprite_paddle::sprite_paddle (bool has_projectiles)
{
  clear_sprite_members ();
  length = 32 * resolution;
  is_vertical = false;
  bumperNorm = 2;
  fire_state = NOT_OWN_GUN;
  is_glue = 0;
  bump_speed = 0;
  bump_actif = 0;
  touch_ball = false;
  paddle_number = 0;
  ball_glued = (sprite_ball *) NULL;
  if (has_projectiles)
    {
      projectiles = new controller_projectiles (); 
    }
  else
    {
      projectiles = NULL;
    }
  invincible = 0;
  flickerval = 0;
  width_mini = 0;
  width_maxi = 0;
  width_deca = 0;
  if (resolution == 1)
    {
      width_deca = 3;
    }
  else
    {
      width_deca = 4;
    }
}

/**
 * Release a paddle sprite object
 */
sprite_paddle::~sprite_paddle ()
{
  /* Paddle had projectiles */
  if (NULL != projectiles)
    {
      delete projectiles;
      projectiles = NULL;
    }
}

/**
 * Create and initializes the list of the projectiles for this paddle
 */ 
void
sprite_paddle::create_projectiles_list ()
{
  projectiles->create_projectiles_list (this);
}

/**
 * Paddle fires projectiles
 */
void
sprite_paddle::fire_projectiles ()
{
  if (fire_state != NOT_OWN_GUN)
    {
      projectiles->disponible ();
    }
}

/**
 * Move the paddle's projectiles 
 */
void
sprite_paddle::deplaceTir ()
{
  projectiles->fire ();
  projectiles->move ();
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
  is_enabled = false;
  bump_actif = actif;
  if (bump_actif > 0)
    {
      bump_actif--;
      is_enabled = true;
    }
  if (paddle_number == 1 || (paddle_number == 4 && rTeam == 1))
    {
      bump_actif = 1;
      is_enabled = true;
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
  projectiles->disable_sprites ();
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
  projectiles->disable_sprites ();
}

/**
 * select the sprite image of the paddle
 * @param l length of the paddle
 */
void
sprite_paddle::select_image (Sint32 l)
{
  Sint32 i = (l >> width_deca) - 2;
  if (fire_state != NOT_OWN_GUN)
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

/**
 * Return the paddle number
 * @return 1  1 (bottom), 2 (right), 3 (top), 4 (left) or 5 (robot)
 */
Uint32
sprite_paddle::get_paddle_number ()
{
  return paddle_number;
}

/**
 * Transform into paddle glue in the bricks levels
 */
void
sprite_paddle::set_glue ()
{
  is_glue = 1;
  select_image ();
}

/**
 * Transform into paddle fire 1 in the bricks levels
 */ 
void
sprite_paddle::set_fire_1 ()
{
  fire_state = OWN_GUN;
  select_image ();
  projectiles->fire1RunOn ();
}

/**
 * Transform into paddle fire 2 in the bricks levels
 */
void
sprite_paddle::set_fire_2 ()
{
  fire_state = OWN_GUN;
  select_image ();
  projectiles->fire2RunOn ();
}

/**
 * Release ball if glued in the bricks levels
 */
void
sprite_paddle::release_ball ()
{
  /* is the ball glued on bumper? */
  if (is_glue > 1)
    {
      /* paddle is free */
      is_glue = 1;
    }
  sprite_ball *ball = ball_glued;
  if (NULL != ball)
    {
      ball_glued = (sprite_ball *) NULL;
      ball->disable_stick ();
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
      ball_glued->disable_stick ();
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
Uint32
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
          is_enabled = false;
          flickerval = 0;
        }
      else
        {
          is_enabled = true;
          flickerval = 1;
        }
    }
  else
    {
      is_enabled = true;
    }
}
